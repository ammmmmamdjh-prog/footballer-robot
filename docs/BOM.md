# 🔩 Bill of Materials — Soccer Robot

| # | Component | Qty | Spec | Est. Price (USD) |
|---|-----------|-----|------|------------------|
| 1 | Arduino Mega 2560 (clone OK) | 1 | More pins for sensors | $8 – $12 |
| 2 | 360° IR ball seeker | 1 | 12× TSOP4838 DIY or RCJ-04 module | $10 – $25 |
| 3 | BTS7960 driver | 2 | High-current, handles pushing stalls | $4 – $6 each |
| 4 | DC gear motors | 2 | 12V, 300 RPM, high torque | $8 – $15 /pair |
| 5 | Wheels + encoders (optional) | 1 set | 80–100 mm | $5 |
| 6 | Solenoid (kicker) | 1 | 12V, pulled ≥ 10 mm | $6 – $12 |
| 7 | MOSFET module (IRF3205 / BTS555) | 1 | Drives the solenoid | $1 – $3 |
| 8 | Dribbler motor | 1 | Small brushed motor + roller | $4 – $8 |
| 9 | Line sensors TCRT5000 | 2 | Field border detection | $1 |
| 10 | Li-Po 3S 11.1V 2200–3300 mAh | 1 | 30C+ discharge for kicker | $12 – $20 |
| 11 | Fuse + switch + XT60 | 1 set | Safety first | $3 |
| 12 | Chassis / frame | 1 | Aluminum or 3D printed | $10 – $20 |

**Total: roughly $70 – $140**

## ⚠️ Safety Notes
- The solenoid pulls **huge current spikes** — never share its wire path with
  the logic ground loop; use a star ground.
- Charge Li-Po packs in a fire-safe bag, and storage-charge them after matches.
- Test the kicker **away from people** — a 25 ms pulse can launch the ball hard.
