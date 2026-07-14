# HANNAH Hexapod

HANNAH ist ein selbst entwickelter Hexapod-Laufroboter mit 18 Freiheitsgraden.

Als Hauptcontroller kommt ein Teensy 4.1 zum Einsatz. Die Beine werden mit Dynamixel AX-12 Servos angetrieben. Die Software entsteht in C++ mit PlatformIO und wird schrittweise als modulare Architektur aufgebaut.

## Architektur

Die einzelnen Komponenten haben klar getrennte Aufgaben:

* **Hexapod**           koordiniert als Dirigent den gesamten Programmablauf
* **Leg**               speichert alle Daten und Zustände eines einzelnen Beins
* **ServoBus**          übernimmt ausschließlich die Kommunikation mit den Dynamixel-Servos
* **IKSolver**          berechnet aus einer gewünschten Fußposition die benötigten Gelenkwinkel
* **GaitController**    erzeugt Fußzielpositionen und Schrittbewegungen
* **InputController**   interpretiert die Daten der Funksteuerung
* **SystemStatus**      ist als zentrale Sammlung von Status- und Fehlerflags vorgesehen

Die einzelnen Klassen sollen möglichst unabhängig voneinander bleiben. Der `Hexapod` übernimmt die Koordination und entscheidet, welche Komponente zu welchem Zeitpunkt ausgeführt wird.

## Entwicklungsziel

Der Fokus liegt auf kleinen und nachvollziehbaren Entwicklungsschritten.

Die Software soll:

* verständlich bleiben
* sicher erweitert werden können
* Hardwarezugriffe klar von Berechnungen trennen
* Bewegungsabläufe ohne blockierende Wartezeiten ausführen
* Fehler und unerreichbare Bewegungsziele zuverlässig erkennen

HANNAH wird nicht als fertiges Produkt entwickelt, sondern als langfristiges Lern- und Entwicklungsprojekt in den Bereichen Robotik, C++, Kinematik und Bewegungssteuerung.
