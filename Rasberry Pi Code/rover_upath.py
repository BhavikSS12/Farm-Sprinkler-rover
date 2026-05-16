# ============================================================
# ROVER U-PATH NAVIGATION + SPRAY — Raspberry Pi
# Calibrated for Bhavik's Rover:
#   DIST_PER_PULSE = 0.073638 cm
#   WHEEL_BASE_CM  = 36.0
#
# Pump sprays ONLY during STRAIGHT legs.
# Pump is OFF during turns and stopped state.
# ============================================================

import serial, time, math

# ── Serial Config ─────────────────────────────────────────────
PORT = "/dev/serial0"
BAUD = 115200

# ── Calibrated Motion Constants ───────────────────────────────
DIST_PER_PULSE = 0.073638
WHEEL_BASE_CM  = 36.0
ARC_PER_DEGREE = (math.pi * WHEEL_BASE_CM) / 360   # 0.3142 cm/deg

# ── Drive Speeds ──────────────────────────────────────────────
DRIVE_SPEED = 120    # PWM for straight legs
TURN_SPEED  = 120     # PWM for turns
POLL_S      = 0.05   # encoder poll interval in seconds

# ── Pump Config ───────────────────────────────────────────────
PUMP_PWM    = 255    # pump speed during spraying (0–255)
PUMP_PRIME_S = 0.3   # seconds to let pump prime before rover moves forward
PUMP_TAIL_S  = 0.2   # seconds to keep pump on after leg ends (clears nozzle)

# ── U-Shape Path Parameters ───────────────────────────────────
LEG1_CM = 100
LEG2_CM = 40
LEG3_CM = 100

PATH = [
    ("STRAIGHT",   LEG1_CM),
    ("TURN_RIGHT",  90),
    ("STRAIGHT",   LEG2_CM),
    ("TURN_RIGHT",  90),
    ("STRAIGHT",   LEG3_CM),
    ("STOP",        0),
]

# ── Serial Init ───────────────────────────────────────────────
ser = serial.Serial(PORT, BAUD, timeout=2)
time.sleep(2)

print("Waiting for STM32...")
start     = time.time()
connected = False

while time.time() - start < 6:
    line = ser.readline().decode(errors='ignore').strip()
    if line:
        print(f"  STM32 says: {line}")
        if "STM32_READY" in line:
            connected = True
            break

if not connected:
    print("Boot message missed — pinging STM32...")
    ser.write(b"E\n")
    time.sleep(0.3)
    resp = ser.read_all().decode(errors='ignore').strip()
    if resp:
        print(f"  STM32 alive: {resp}")
        connected = True
    else:
        print("ERROR: STM32 not responding. Check TX/RX wiring and GND.")
        ser.close()
        exit()

print("STM32 Ready!")
print(f"DIST_PER_PULSE  = {DIST_PER_PULSE} cm")
print(f"ARC_PER_DEGREE  = {ARC_PER_DEGREE:.4f} cm")
print(f"Pulses for 90°  = {(ARC_PER_DEGREE * 90) / DIST_PER_PULSE:.1f}")
print(f"Pump PWM        = {PUMP_PWM}")

# ── Helpers ───────────────────────────────────────────────────
def send(cmd):
    ser.write((cmd + "\n").encode())
    time.sleep(0.02)

def get_encoders():
    # Drain any pending replies (e.g. "PUMP_ON", "OK") before sending "E",
    # so we don't mistake them for encoder data.
    ser.reset_input_buffer()
    for attempt in range(5):
        send("E")
        deadline = time.time() + 0.15   # wait up to 150 ms for the reply
        while time.time() < deadline:
            line = ser.readline().decode(errors='ignore').strip()
            if not line:
                continue
            if "A:" in line and "B:" in line:
                try:
                    a = int(line.split(",")[0].split(":")[1])
                    b = int(line.split(",")[1].split(":")[1])
                    return a, b
                except Exception:
                    break   # malformed — retry outer loop
            # Any other reply (PUMP_ON, OK, etc.) — discard and keep waiting
        time.sleep(0.02)
    return 0, 0

def reset_enc():
    send("RESET")
    time.sleep(0.1)

def pump_on(pwm=PUMP_PWM):
    ser.reset_input_buffer()
    send(f"PUMP_ON:{pwm}")
    # Drain the "PUMP_ON\n" ack so it doesn't land in the encoder read loop
    time.sleep(0.05)
    ser.reset_input_buffer()
    print(f"  [PUMP] ON @ PWM {pwm}")

def pump_off():
    ser.reset_input_buffer()
    send("PUMP_OFF")
    # Drain the "PUMP_OFF\n" ack
    time.sleep(0.05)
    ser.reset_input_buffer()
    print("  [PUMP] OFF")

# ── Motion Primitives ─────────────────────────────────────────
def go_straight(dist_cm, spray=True):
    """Drive straight dist_cm. If spray=True, pump runs for the whole leg."""
    target = dist_cm / DIST_PER_PULSE
    reset_enc()

    if spray:
        pump_on()
        time.sleep(PUMP_PRIME_S)   # prime pump before moving

    send(f"F:{DRIVE_SPEED}")
    print(f"  → STRAIGHT {dist_cm}cm (target: {target:.0f} pulses)"
          + (" [SPRAYING]" if spray else ""))

    while True:
        a, b = get_encoders()
        avg  = (a + b) / 2.0
        print(f"    A:{a:5d} B:{b:5d} avg:{avg:7.1f} / {target:.0f}", end="\r")
        if avg >= target:
            break
        time.sleep(POLL_S)

    send("S")

    if spray:
        time.sleep(PUMP_TAIL_S)    # let pump clear nozzle
        pump_off()

    time.sleep(0.4)
    print(f"\n  ✓ STRAIGHT {dist_cm}cm done")


def turn_right(angle_deg):
    arc_cm = ARC_PER_DEGREE * angle_deg
    target  = arc_cm / DIST_PER_PULSE
    reset_enc()
    # Pump is already off — turns are dry
    send(f"T:{TURN_SPEED}")
    print(f"  → TURN RIGHT {angle_deg}° (target: {target:.0f} pulses)")

    while True:
        a, b = get_encoders()
        avg  = (a + b) / 2.0
        print(f"    A:{a:5d} B:{b:5d} avg:{avg:7.1f} / {target:.0f}", end="\r")
        if avg >= target:
            break
        time.sleep(POLL_S)

    send("S")
    time.sleep(0.5)
    print(f"\n  ✓ TURN RIGHT {angle_deg}° done")


def turn_left(angle_deg):
    arc_cm = ARC_PER_DEGREE * angle_deg
    target  = arc_cm / DIST_PER_PULSE
    reset_enc()
    send(f"L:{TURN_SPEED}")
    print(f"  → TURN LEFT {angle_deg}° (target: {target:.0f} pulses)")

    while True:
        a, b = get_encoders()
        avg  = (a + b) / 2.0
        print(f"    A:{a:5d} B:{b:5d} avg:{avg:7.1f} / {target:.0f}", end="\r")
        if avg >= target:
            break
        time.sleep(POLL_S)

    send("S")
    time.sleep(0.5)
    print(f"\n  ✓ TURN LEFT {angle_deg}° done")


# ── Path Executor ─────────────────────────────────────────────
def run_path(path):
    print()
    print("=" * 60)
    print("  U-SHAPE PATH + SPRAY STARTING")
    print(f"  LEG1={LEG1_CM}cm [SPRAY] → TURN_R 90° → "
          f"LEG2={LEG2_CM}cm [SPRAY] → TURN_R 90° → LEG3={LEG3_CM}cm [SPRAY]")
    print("=" * 60)

    # Safety: ensure pump is off at start
    pump_off()
    time.sleep(0.2)

    for i, (action, value) in enumerate(path):
        print(f"\n[Step {i+1}/{len(path)}] {action} {value}")

        if action == "STRAIGHT":
            go_straight(value, spray=True)

        elif action == "TURN_RIGHT":
            turn_right(value)

        elif action == "TURN_LEFT":
            turn_left(value)

        elif action == "STOP":
            send("S")
            pump_off()
            print()
            print("=" * 60)
            print("  PATH COMPLETE — ROVER STOPPED — PUMP OFF")
            print("=" * 60)
            return

        time.sleep(0.3)


# ── Entry Point ───────────────────────────────────────────────
if __name__ == "__main__":
    try:
        run_path(PATH)
    except KeyboardInterrupt:
        send("S")
        pump_off()
        print("\n\n⚠ Emergency stop — Ctrl+C pressed")
    finally:
        ser.close()
        print("Serial port closed.")