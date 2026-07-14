# HANNAH Hexapod

HANNAH ist ein selbst entwickelter Hexapod-Laufroboter mit 18 Freiheitsgraden.
Das Projekt verwendet einen Teensy 4.1, Dynamixel AX-12 Servos und eine
modulare C++-Architektur mit ServoBus, IKSolver, GaitController,
InputController und SystemStatus.

## Architektur

Die Komponenten sollen sauber getrennt bleiben:

- **Hexapod** koordiniert den Gesamtablauf als Dirigent
- **ServoBus** spricht nur mit den Dynamixel-Servos
- **IKSolver** berechnet nur inverse Kinematik
- **GaitController** erzeugt Fußzielpositionen und Schrittbewegungen
- **InputController** interpretiert die Funksteuerung
- **SystemStatus** sammelt Fehlerflags

## Entwicklungsziel

Die Software ist in C++ mit PlatformIO aufgebaut. Der Fokus liegt auf kleinen,
sicheren Schritten, damit die Architektur verständlich und wartbar bleibt.

## Vorgesehene GitHub Topics

- `hexapod`
- `robotics`
- `teensy`
- `platformio`
- `cpp`
- `dynamixel`
- `inverse-kinematics`
- `gait-controller`
- `walking-robot`
- `18dof`
