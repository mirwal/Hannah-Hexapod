// Tasten.cpp
#include "Tasten.h"

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

bool Tasten::funkDatenEinlesen()
{
	bool packetReceived = false;

	offline_Counter++;
	if (offline_Counter > 20)
	{
		setOnlineStatus(false);
	}

	// Wir brauchen mindestens Startbyte + zweites Startbyte
	while (availableData() >= 2)
	{
		byte first = readData();

		if (first != 0xF1)
		{
			continue;
		}
		byte second = readData(); // read and discard the second start byte
		if (second != 0x7E)
		{
			continue;
		}

		// Ab hier haben wir den Paketanfang gefunden.
		// Es fehlen noch 18 Byte: Byte 2 bis Byte 19.
		if (availableData() < 18)
		{
			// Paket ist noch nicht vollständig angekommen.
			return false;
		}

		uint8_t packet[20] = {0};

		packet[0] = 0xF1;
		packet[1] = 0x7E;
		for (uint8_t i = 2; i < 20; i++)
		{
			packet[i] = readData();
		}
		if (!FunkPacketIsChecksumValid(packet, 20))
		{
			return false;
		}

		// Paket ist gültig
		// Jetzt Daten aus packet[] übernehmen
		funkPacket.hl_ud = readU16(packet, 2);
		funkPacket.hl_lr = readU16(packet, 4);
		funkPacket.hr_ud = readU16(packet, 6);
		funkPacket.hr_lr = readU16(packet, 8);
		funkPacket.poti = readU16(packet, 10);
		funkPacket.flap = readU16(packet, 12);
		funkPacket.fader = readU16(packet, 14);

		funkPacket.schalter = packet[16];
		funkPacket.hl1 = isBitSet(funkPacket.schalter, 0);
		funkPacket.hl2 = isBitSet(funkPacket.schalter, 1);
		funkPacket.hl3 = isBitSet(funkPacket.schalter, 2);

		funkPacket.hr1 = isBitSet(funkPacket.schalter, 3);
		funkPacket.hr2 = isBitSet(funkPacket.schalter, 4);
		funkPacket.hr3 = isBitSet(funkPacket.schalter, 5);

		funkPacket.taster = packet[17];
		funkPacket.taster1 = isBitSet(funkPacket.taster, 0);
		funkPacket.taster2 = isBitSet(funkPacket.taster, 1);
		funkPacket.taster3 = isBitSet(funkPacket.taster, 2);
		funkPacket.taster4 = isBitSet(funkPacket.taster, 3);
		funkPacket.taster5 = isBitSet(funkPacket.taster, 4);
		funkPacket.taster6 = isBitSet(funkPacket.taster, 5);
		funkPacket.taster7 = isBitSet(funkPacket.taster, 6);
		funkPacket.taster8 = isBitSet(funkPacket.taster, 7);

		funkPacket.sonderTasten = packet[18];
		funkPacket.trainer = isBitSet(funkPacket.sonderTasten, 0);
		funkPacket.clearTaster = isBitSet(funkPacket.sonderTasten, 1);
		funkPacket.backTaster = isBitSet(funkPacket.sonderTasten, 2);
		funkPacket.encoderTaster = isBitSet(funkPacket.sonderTasten, 3);
		funkPacket.sonderTaste5 = isBitSet(funkPacket.sonderTasten, 4);
		funkPacket.sonderTaste6 = isBitSet(funkPacket.sonderTasten, 5);
		funkPacket.sonderTaste7 = isBitSet(funkPacket.sonderTasten, 6);
		funkPacket.sonderTaste8 = isBitSet(funkPacket.sonderTasten, 7);

		offline_Counter = 0;
		funkPacket.online = true;
		packetReceived = true;
	}
	return packetReceived;
}

const FunkPacket &Tasten::getFunkPacket() const
{
	return funkPacket;
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

void Tasten::setOnlineStatus(bool state)
{
	funkPacket.online = state;
}
uint8_t Tasten::calculateChecksum(const uint8_t *data, size_t length)
{
	uint8_t checksum = 0;

	for (size_t i = 0; i < length; i++)
	{
		checksum ^= data[i];
	}

	return checksum;
}

bool Tasten::FunkPacketIsChecksumValid(const uint8_t *data, size_t length)
{
	if (length < 1)
	{
		return false; // Not enough data to validate checksum
	}

	uint8_t receivedChecksum = data[length - 1];
	uint8_t calculatedChecksum = calculateChecksum(data, length - 1);

	return receivedChecksum == calculatedChecksum;
}

bool Tasten::getOnlineStatus()
{
	return funkPacket.online;
}

// Tasten Tast;

// 1011000