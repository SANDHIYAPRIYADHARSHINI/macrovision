# Edge AI Proximity Safety Monitor

## Overview

This project demonstrates a simulated Physical AI pipeline:

Sense → Edge Inference → Decision → Action

The prototype represents an industrial access-safety monitor. It detects a simulated person near restricted equipment and activates visual and audible alerts when the person is too close.

Because physical ESP32-CAM hardware was unavailable, the camera model output is simulated using a pushbutton in Wokwi.

## Project Scenario

The system monitors access to an industrial machine:

- The ESP32-CAM would detect whether a person is present.
- The HC-SR04 measures proximity to the machine.
- The ESP32 combines both outputs locally.
- LEDs and a buzzer indicate the safety condition.

## Hardware

- ESP32 DevKit
- ESP32-CAM function simulated by a pushbutton
- HC-SR04 ultrasonic sensor
- Green LED
- Yellow LED
- Red LED
- Piezo buzzer
- 220-ohm resistors

## Simulation Mapping

| Component | Role |
|---|---|
| Red pushbutton on GPIO 15 | Simulated ESP32-CAM prediction |
| Blue pushbutton on GPIO 4 | Selects hardcoded distance test value |
| HC-SR04 | Physical proximity sensor model retained for future use |
| Green LED | Normal state |
| Yellow LED | Warning state |
| Red LED | Critical state |
| Buzzer | Critical safety alert |

## Edge Inference

The intended physical system would run an image-classification model on the ESP32-CAM. The model would produce one of these outputs:

- `PERSON_DETECTED`
- `NO_PERSON`

In this Wokwi adaptation, the red pushbutton simulates that model output:

- Button released: `NO_PERSON`
- Button pressed: `PERSON_DETECTED`

The decision itself runs locally on the ESP32. No cloud service is used for the decision.

## Decision Logic

The system uses the camera prediction and proximity value:

| Camera result | Distance | State |
|---|---:|---|
| No person detected | Any distance | Normal |
| Person detected | Greater than 20 cm | Normal |
| Person detected | 20 cm | Warning |
| Person detected | 10 cm | Critical |

## Deterministic Test Mode

The Wokwi HC-SR04 input was not used as the primary demonstration input because repeatable sensor values were needed for the video. The code therefore includes a deterministic test mode:

- 40 cm: safe test distance.
- 20 cm: warning test distance.
- 10 cm: critical test distance.

The blue button cycles through these values. In a physical implementation, the hardcoded test function would be replaced with the real HC-SR04 measurement function.

## Output Behaviour

- Normal: green LED.
- Warning: yellow LED.
- Critical: red LED and buzzer.

## Architecture

See [architecture.md](architecture.md).

## Wokwi Simulation

[Open the Wokwi simulation]https://wokwi.com/projects/475422819290744833

## Video Demonstration


[Watch the demonstration video]https://drive.google.com/file/d/18qKjjd3PMr3s4xDWlQZg7YhFvbeqwTTt/view?usp=sharing

## Personal Contribution

I designed the industrial safety-monitoring scenario, created the Wokwi circuit, wired the simulated ESP32 system, implemented the camera-output simulation, implemented deterministic proximity test values, developed the local decision logic, connected the state outputs to LEDs and a buzzer, tested the normal/warning/critical states, and documented the limitations and physical extension.

## Extension to MacroVision Claw

A physical MacroVision Claw version could use an ESP32-CAM with an Edge Impulse image-classification model to detect people near restricted industrial equipment. The HC-SR04 could be replaced or supplemented with industrial proximity, vibration, temperature, current, and pressure sensors. The ESP32 could combine the camera prediction and sensor readings locally, trigger an alarm or relay during unsafe conditions, and send only important events to a monitoring dashboard. This would reduce reaction time and allow safety actions even when network connectivity is unavailable.

## Limitations

- The prototype is simulated in Wokwi.
- The ESP32-CAM image output is represented by a pushbutton.
- No live camera image is processed in the current simulation.
- The proximity values are deterministic test values rather than live HC-SR04 measurements.
- The Edge Impulse image model is proposed for the physical implementation but is not included in this simulation.
