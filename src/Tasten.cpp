// Tasten.cpp
#if defined(ARDUINO) && ARDUINO >= 100 // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Tasten.h"

// Macro for the selection of the Serial Port

#define sendData(args) (Serial4.write(args))  // Write Over Serial
#define availableData() (Serial4.available()) // Check Serial Data Available
#define readData() (Serial4.read())			  // Read Serial Data
#define peekData() (Serial4.peek())			  // Peek Serial Data
#define beginCom(args) (Serial4.begin(args))  // Begin Serial Comunication
#define endCom() (Serial4.end())			  // End Serial Comunication
#define sendDebug(args) (Serial.print(args))
#define sendDebugln(args) (Serial.println(args))
// Macro for Timing

#define delayus(args) (delayMicroseconds(args)) // Delay Microseconds

Tasten::Tasten()
{
}

Tasten::~Tasten()
{
}
void Tasten::begin(long baud)
{
	beginCom(baud);
}

bool Tasten::lesen()
{
	bool packetReceived = false;

	offline_Counter++;
	if (offline_Counter > 20)
	{
		setOnlineStatus(false);
		updateFunkPacket();
	}

	if (availableData() < 50)
		return false;

	while (availableData() > 0)
	{

		Incoming_Byte = readData();
		if ((Incoming_Byte == 0x0A) && (readData() == 0x7E))
		{
			if (availableData() < 22)
			{
				return packetReceived;
			}
			int count = 0;
			uint8_t speicher[12];

			do
			{ // Hebel und Poti und Feader lesen
				MSB = readData();
				LSB = readData();
				speicher[count] = decodiren(MSB, LSB);
				if (speicher[count++] > 128)
					count = 9;
			} while ((count <= 7) && (readData() == 0xFF));

			Incoming_Byte = readData();
			if (Incoming_Byte < 128)
				speicher[count++] = Incoming_Byte;

			if (count == 8)
			{
				// Incoming_Byte = speicher[7];

				int i = 0;
				for (i = 0; i < count; i++)
				{
					setTasteWert(i, speicher[i]);
				}
				// Taster lesen
				for (int j = 7; j > 0; j--)
				{
					if (((Incoming_Byte >> 6) % 2) == 0)
					{
						setTasteStatus(14 - j, false);
					}
					else
					{
						setTasteStatus(14 - j, true);
					}
					Incoming_Byte <<= 1;
				}
			}
			Incoming_Byte = readData();
			if (Incoming_Byte < 255)
			{
				speicher[count++] = Incoming_Byte;
			}

			// Taster T1 bis T8 lesen
			if (count == 9)
			{
				for (int t = 8; t > 0; t--)
				{
					if (((Incoming_Byte >> 7) % 2) != 0)
					{
						setTasteStatus(29 - t, true);
					}
					else
					{
						setTasteStatus(29 - t, false);
					}
					Incoming_Byte <<= 1;
				}
			}
			count = 0;
			/**/
			offline_Counter = 0;
			setOnlineStatus(true);
			packetReceived = true;
			updateFunkPacket();
		}
	}
	return packetReceived;
}

const FunkPacket &Tasten::getFunkPacket() const
{
	return funkPacket;
}

bool Tasten::setTasteWert(unsigned char ID, uint8_t wert)
{
	if (wert > 128)
		return false;
	TasteWert[ID] = wert;
	PotiWert[ID] = wert;
	return true;
	// sendDebug(wert);sendDebug("\t");
}
uint8_t Tasten::getTasteWert(unsigned char ID)
{
	if (TasteWert[ID] < 255)
		return TasteWert[ID];
	return TasteWert[ID];
}
int Tasten::getTasteWert(unsigned char ID, bool t)
{
	if (PotiWert[ID] < 255)
		return PotiWert[ID];
	return PotiWert[ID];
}

bool Tasten::getTasteStatus(unsigned char ID)
{
	if (TasteStatus[ID])
		return true;
	return false;
}
bool Tasten::sendBefehl(unsigned char befehl)
{
	if (ist_befehl == befehl)
		return false;

	switch (befehl)
	{
	case (Akku):
		sendData(StartBit);
		delayus(1000);
		sendData(Akku); // "Akkuobjekt... "
		delayus(1000);
		ist_befehl = Akku;
		break;
	case (Online):
		sendData(StartBit);
		delayus(1000);
		sendData(Online); // "Akkuobjekt... "
		delayus(1000);
		ist_befehl = Online;
		break;

	default:
		ist_befehl = 0x0;
		return false;
	}
	return true;
}
float Tasten::getAxisNorm(uint8_t id, uint8_t rawMin, uint8_t rawMax)
{

	int raw = getTasteWert(id); // 0…255
	// Verschiebe und skaliere: Mitte → 0.0, Rand → ±1.0
	float mid = (rawMin + rawMax) * 0.5f;
	float halfR = (rawMax - rawMin) * 0.5f;
	float norm = (raw - mid) / halfR;
	// Sicherstellen, dass wir nicht out-of-bounds rauslaufen:
	if (norm > 1.0f)
		norm = 1.0f;
	else if (norm < -1.0f)
		norm = -1.0f;

	return norm;
}

void Tasten::setOnlineStatus(bool state)
{
	online = state;
}
void Tasten::updateFunkPacket()
{
	funkPacket.hl_ud = getTasteWert(HL_UD);
	funkPacket.hl_lr = getTasteWert(HL_LR);

	funkPacket.hr_ud = getTasteWert(HR_UD);
	funkPacket.hr_lr = getTasteWert(HR_LR);

	funkPacket.poti = getTasteWert(POTI);
	funkPacket.fader = getTasteWert(FADER);
	funkPacket.flap = getTasteWert(FLAP);

	funkPacket.hr1 = getTasteStatus(HR1);
	funkPacket.hr2 = getTasteStatus(HR2);
	funkPacket.hr3 = getTasteStatus(HR3);

	funkPacket.hl1 = getTasteStatus(HL1);
	funkPacket.hl2 = getTasteStatus(HL2);
	funkPacket.hl3 = getTasteStatus(HL3);

	funkPacket.trainer = getTasteStatus(TRAINER);

	funkPacket.t1 = getTasteStatus(T1);
	funkPacket.t2 = getTasteStatus(T2);
	funkPacket.t3 = getTasteStatus(T3);
	funkPacket.t4 = getTasteStatus(T4);
	funkPacket.t5 = getTasteStatus(T5);
	funkPacket.t6 = getTasteStatus(T6);
	funkPacket.t7 = getTasteStatus(T7);
	funkPacket.t8 = getTasteStatus(T8);
	funkPacket.online = getOnlineStatus();
}

bool Tasten::getOnlineStatus()
{
	return online;
}

bool Tasten::setTasteStatus(unsigned char ID, bool status)
{
	if (TasteStatus[ID] == status)
		return false;

	TasteStatus[ID] = status;
	return true;
}

Tasten Tast;

// 1011000