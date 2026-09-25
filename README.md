# ⚽ Soccer Robot — Autonomous Striker

![Platform](https://img.shields.io/badge/platform-Arduino-00979D?style=flat-square&logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-00f5ff?style=flat-square)
![Competition](https://img.shields.io/badge/league-robot%20soccer-orange?style=flat-square)

An autonomous robot built for robot-soccer matches. It hunts the ball with an
infrared seeker, drives to it with a twin-motor differential drive, and shoots
with a solenoid kicker — full attack logic running on an Arduino.

---

## 📷 Demo

<!-- Add photos of the robot and match clips into /media and link them -->
<!-- ![Robot front view](media/robot-front.jpg) -->

---

## 🧠 Match Logic (State Machine)

```
        ┌──────────┐   ball visible    ┌───────────┐
        │  SEARCH  ├──────────────────►│   CHASE   │
        └────┬─────┘                   └─────┬─────┘
             │ timeout / no signal           │ ball close
             ▼                               ▼
        rotates slowly              ┌───────────┐
                                    │  ATTACK   │──► KICK when in range
                                    └─────┬─────┘
                                          │ line detected (field edge)
                                          ▼
                                    ┌───────────┐
                                    │  RETURN   │──► drive back to center
                                    └───────────┘
```

1. **SEARCH** — rotate in place until the IR seeker reports a ball bearing.
2. **CHASE** — drive toward the ball, adjusting heading every loop.
3. **ATTACK** — when the ball is captured by the dribbler, aim at the goal
   bearing and fire the kicker solenoid.
4. **RETURN** — the two line sensors under the chassis detect the white
   field border; the robot backs off and re-centers to avoid going out.

---

## 🔧 Hardware (BOM)

| Component | Qty | Notes |
|-----------|-----|-------|
| Arduino Mega / UNO | 1 | Mega if you need more pins |
| IR Ball Sensor (e.g. TSOP array / RCJ-04) | 1 | 360° ball bearing + distance |
| L298N × 1 or BTS7960 × 2 | 1–2 | Motor drivers |
| DC gear motors | 2 | High torque for pushing |
| Solenoid kicker + MOSFET driver | 1 | The "shoot" actuator |
| Dribbler motor | 1 | Optional — keeps ball attached |
| 2× line sensors (TCRT5000) | 2 | Detect white border |
| Li-Po battery 11.1V | 1 | 30C+ for kicker bursts |

Full list: [`docs/BOM.md`](docs/BOM.md)

---

## ⚙️ Key Parameters

| Parameter | Default | Effect |
|-----------|---------|--------|
| `KICK_RANGE` | 25 | Seeker distance value that triggers the shot |
| `KICK_PULSE_MS` | 25 | Solenoid pulse length (longer = stronger) |
| `CHASE_SPEED` | 180 | Cruise speed while chasing |
| `LINE_BACKOFF_MS` | 400 | Reverse time after seeing the border |

---

## 📂 Repository Structure

```
footballer-robot/
├── firmware/
│   └── soccer_bot/
│       └── soccer_bot.ino     ← main firmware (state machine)
├── docs/
│   ├── BOM.md
│   └── tactics.md             ← match strategy notes (add later)
├── media/                     ← robot & match photos/videos
└── README.md
```

---

## 🚀 Getting Started

```bash
git clone https://github.com/ammmmmamdjh-prog/footballer-robot.git
```

1. Open `firmware/soccer_bot/soccer_bot.ino` in the **Arduino IDE**.
2. Select the board + port → **Upload**.
3. Run the serial console at `115200` baud to watch the state machine live.
4. Test the kicker with the ball far away first — then calibrate.

---

## 🗺️ Roadmap

- [x] Ball seeking & chase
- [x] Kick on range
- [x] Field-border protection
- [ ] Goal-aimed shooting (compass/goal beacon)
- [ ] Omni-wheel drive upgrade
- [ ] Opponent avoidance

---

## 📄 License

Released under the [MIT License](LICENSE) — © 2026 AmirAli
