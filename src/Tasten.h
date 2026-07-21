// Tasten.h
#pragma once
#include <Arduino.h>
#include <cstdint>
/// Schnittstelle zur Funksteuerung: liest die seriell empfangenen Steuerdaten ein,
/// dekodiert Kanäle, Taster und Schalter und stellt sie dem restlichen Programm
/// als Rohwerte, normierte Achsenwerte und Statusinformationen zur Verfügung.

// #define HL_UD 0
// #define HL_LR 1
// #define HR_UD 2
// #define HR_LR 3
// #define POTI 4
// #define FADER 5
// #define FLAP 6
// #define HR1 7
// #define HR2 8
// #define HR3 9
// #define HL1 10
// #define HL2 11
// #define HL3 12
// #define TRAINER 13

// #define T1 21
// #define T2 22
// #define T3 23
// #define T4 24
// #define T5 25
// #define T6 26
// #define T7 27
// #define T8 28
struct FunkPacket
{
	// Analogwerte 0–1023
	uint16_t hl_ud = 0;
	uint16_t hl_lr = 0;
	uint16_t hr_ud = 0;
	uint16_t hr_lr = 0;
	uint16_t poti = 0;
	uint16_t fader = 0;

	// Schalter / Sonderfunktionen
	uint8_t flap = 0;

	// Kompakte Bitmaske für Übertragung
	uint8_t schalter = 0;
	uint8_t taster = 0;
	uint8_t sonderTasten = 0;

	// Einzelne Tasten, gut lesbar im Code
	bool hr1 = false;
	bool hr2 = false;
	bool hr3 = false;

	bool hl1 = false;
	bool hl2 = false;
	bool hl3 = false;

	bool trainer = false;

	bool taster1 = false;
	bool taster2 = false;
	bool taster3 = false;
	bool taster4 = false;
	bool taster5 = false;
	bool taster6 = false;
	bool taster7 = false;
	bool taster8 = false;

	bool sonderTaste1 = false;
	bool sonderTaste2 = false;
	bool sonderTaste3 = false;
	bool sonderTaste4 = false;
	bool sonderTaste5 = false;
	bool sonderTaste6 = false;
	bool sonderTaste7 = false;
	bool sonderTaste8 = false;

	bool encoderTaster = false;
	bool clearTaster = false;
	bool backTaster = false;
	bool online = false;
};

class Tasten
{
public:
	Tasten();
	~Tasten();
	void begin(long baud);
	bool funkDatenEinlesen();
	const FunkPacket &getFunkPacket() const;

private:
	int offline_Counter = 100;
	bool sendBefehl(unsigned char befehl);

	bool getOnlineStatus();
	enum Befehl
	{
		ERROR = (0xFF),
		StartBit = (0xF1),
		Online = (0x90),
		Akku, // "Akkuobjekt... "
		mirwal,
		TESTEN,

	};
	bool TRAINER_Flag = false;
	unsigned char ist_befehl = 0x00;
	unsigned char Incoming_Byte;

	uint16_t readU16(const uint8_t *packet, uint8_t index) { return (uint16_t)packet[index] | ((uint16_t)packet[index + 1] << 8); }
	void writeU16(uint8_t *packet, uint8_t index, uint16_t value)
	{
		packet[index] = value & 0xFF;
		packet[index + 1] = (value >> 8) & 0xFF;
	}
	bool isBitSet(uint8_t value, uint8_t bit)
	{
		return (value & (1 << bit)) != 0;
	}
	bool online = false;
	void setOnlineStatus(bool state);
	FunkPacket funkPacket;
	bool FunkPacketIsChecksumValid(const uint8_t *data, size_t length);
	uint8_t calculateChecksum(const uint8_t *data, size_t length);
};

extern Tasten Tast;