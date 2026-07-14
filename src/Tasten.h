// Tasten.h
#pragma once
#include <cstdint>
/// Schnittstelle zur Funksteuerung: liest die seriell empfangenen Steuerdaten ein,
/// dekodiert Kanäle, Taster und Schalter und stellt sie dem restlichen Programm
/// als Rohwerte, normierte Achsenwerte und Statusinformationen zur Verfügung.

#define HL_UD 0
#define HL_LR 1
#define HR_UD 2
#define HR_LR 3
#define POTI 4
#define FADER 5
#define FLAP 6
#define HR1 7
#define HR2 8
#define HR3 9
#define HL1 10
#define HL2 11
#define HL3 12
#define TRAINER 13

#define T1 21
#define T2 22
#define T3 23
#define T4 24
#define T5 25
#define T6 26
#define T7 27
#define T8 28
struct FunkPacket
{
	uint8_t hl_ud = 0;
	uint8_t hl_lr = 0;

	uint8_t hr_ud = 0;
	uint8_t hr_lr = 0;

	uint8_t poti = 0;
	uint8_t fader = 0;
	uint8_t flap = 0;

	uint8_t hr1 = 0;
	uint8_t hr2 = 0;
	uint8_t hr3 = 0;

	uint8_t hl1 = 0;
	uint8_t hl2 = 0;
	uint8_t hl3 = 0;

	bool trainer = false;

	bool t1 = false;
	bool t2 = false;
	bool t3 = false;
	bool t4 = false;
	bool t5 = false;
	bool t6 = false;
	bool t7 = false;
	bool t8 = false;
	bool online = false;
};

class Tasten
{
public:
	Tasten();
	~Tasten();
	void begin(long baud);
	bool lesen();
	const FunkPacket &getFunkPacket() const;

private:
	int offline_Counter = 100;
	uint8_t getTasteWert(unsigned char ID);
	int getTasteWert(unsigned char ID, bool t);
	bool getTasteStatus(unsigned char ID);
	bool sendBefehl(unsigned char befehl);
	float getDirX() { return getAxisNorm(HL_LR, 30, 102); }
	float getDirY() { return getAxisNorm(HL_UD, 34, 92); }
	float getRotation() { return getAxisNorm(HR_LR, 29, 100); }
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
	bool TasteStatus[29] = {false};
	uint8_t TasteWert[8]; // unsigned char	1 byte	0 to 255
	int PotiWert[8];	  // unsigned char	1 byte	0 to 255

	uint8_t MSB = 0; // HIGH Bit und LOW Bit ... zwei daten auslesen
	uint8_t LSB = 0;
	bool setTasteWert(unsigned char ID, uint8_t wert);
	uint8_t decodiren(uint8_t MSB, uint8_t LSB)
	{
		//	return (LSB + (MSB * 256) + MSB);
		return (MSB << 8) + LSB;
	}
	bool setTasteStatus(unsigned char ID, bool Status);
	/// Liefert dir für einen Hebel-/Poti-Kanal eine normierte Achsen-Wert ∈ [–1, 1].
	/// id: Kanal-ID (z.B. HL_LR, HL_UD, HR_LR)
	/// rawMin/rawMax: untere/obere Grenze, die dein Empfänger in Ruhe liefert (z.B. 30…102)
	float getAxisNorm(uint8_t id, uint8_t rawMin, uint8_t rawMax);
	bool online = false;
	void setOnlineStatus(bool state);
	FunkPacket funkPacket;
	void updateFunkPacket();
};

extern Tasten Tast;