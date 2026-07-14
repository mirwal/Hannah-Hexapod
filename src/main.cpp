// Projekt: Hexapod Steuerung mit Teensy 4.1 und Dynamixel AX-12A
#include <Arduino.h>
#include "Hexapod.h"
#include "ServoBus.h"
#include "Tasten.h"
#include "InputController.h"
#include "SystemStatus.h"

#define sendDebug(args) (Serial.print(args))
#define sendDebugln(args) (Serial.println(args))

ServoBus servoBus;
Tasten tast;
InputController inputController(tast);
SystemStatus systemStatus(ERROR_LED_PIN);
Hexapod hexapod(servoBus, systemStatus);

// Update-Intervall in Millisekunden (50 Hz)
static constexpr uint32_t INTERVAL_MS = 50; // 50 Hz
uint32_t lastUpdate = 0;

// Serial Debugging time
uint32_t debugTime = 0;
// offline Status melden
uint32_t offlineTime = 0;
// interval für tasten lesen
uint32_t tastTime = 0;
// ul_messTimeStart und ul_messTimeEnd für Messung der Zykluszeit
uint32_t messTimeStart = 0;
uint32_t messTimeEnd = 0;

uint32_t lastPrintTime = 0;
uint32_t lastPingTime = 0;

void setup()
{
  Serial.begin(115200);
  tast.begin(9600);     // Funkempfänger initialisieren
  servoBus.begin();     // Dynamixel UART
  systemStatus.begin(); // Systemstatus initialisieren
  // Warten, bis die serielle Verbindung hergestellt ist
  // 5 Sekunden Timeout
  uint32_t startTime = millis();

  while (!Serial && (millis() - startTime < 5000))
  {
    delay(10);
  }

  Serial.println("Hexapod Startet...");
  delay(2000); // Servos Zeit zum Hochfahren geben

  Serial.print(hexapod.createServoIdReport()); // Servos ausgeben
  for (uint8_t i = 1; i <= 18; i++)
  {
    servoBus.setSpeed(i, 80); // Geschwindigkeit für alle Servos setzen
    delay(20);
  }
  delay(500); // Warten, bis die Servos die Geschwindigkeit übernommen haben

  hexapod.begin(); // Hexapod initialisieren
}

void loop()
{

  messTimeStart = millis();
  inputController.update();
  hexapod.update(inputController.getControl());
  messTimeEnd = millis();
  debugTime = messTimeEnd - messTimeStart;
}
