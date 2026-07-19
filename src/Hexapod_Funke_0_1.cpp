#include <spi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//
// #include <Adafruit_GFX_2.h>
// #include <Adafruit_SSD1306_2.h>

#include <SdFat.h>

#define OLED_RESET 4 // not used / nicht genutzt bei diesem Display

#define DRAW_DELAY 118
#define D_NUM 47
Adafruit_SSD1306 display1(OLED_RESET); //	SSD1306_128_64  128x64 pixel display

Adafruit_SSD1306 display2(OLED_RESET); //	SSD1306_128_64  128x64 pixel display

//[][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][]
//[][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][]

#define VIBRATOR 7 // pin 7 mit pwm

#define HL_UD 0
#define HR_UD 2
#define HL_LR 1
#define HR_LR 3
#define Poti 4
#define Fader 5
#define Flap 6
#define Batterie 15

#define HL_UD_Offset 0
#define HR_UD_Offset -4
#define HL_LR_Offset -3
#define HR_LR_Offset 0
#define Poti_Offset 0
#define Flap_Offset 4

#define HR1 22
#define HR2 24
#define HR3 27

#define HL1 25
#define HL2 28
#define HL3 29

#define TRAINER 23

void FunkeAuswerten();

uint8_t HL_UD_tmp;
uint8_t HR_UD_tmp;
uint8_t HL_LR_tmp;
uint8_t HR_LR_tmp;

int Poti_tmp;

uint8_t HR1_tmp;
uint8_t HR2_tmp;
uint8_t HR3_tmp;

uint8_t HL1_tmp;
uint8_t HL2_tmp;
uint8_t HL3_tmp;

uint8_t TRAINER_tmp;
uint8_t FLAP_tmp;

//[][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][]
//[][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][] [][][][][][][][]

// const uint8_t SOFT_MISO_PIN = 50;			// Pin numbers in templates must be constants.
// const uint8_t SOFT_MOSI_PIN = 51;
// const uint8_t SOFT_SCK_PIN = 52;
// const uint8_t SD_CHIP_SELECT_PIN = 53;		// Chip select may be constant or RAM variable.

// Set DISABLE_CHIP_SELECT to disable a second SPI device.
// For example, with the Ethernet shield, set DISABLE_CHIP_SELECT
// to 10 to disable the Ethernet controller.
const int8_t DISABLE_CHIP_SELECT = -1;
//
// Test with reduced SPI speed for breadboards.
// Change spiSpeed to SPI_FULL_SPEED for better performance
// Use SPI_QUARTER_SPEED for even slower SPI bus speed
/*const uint8_t spiSpeed = SPI_HALF_SPEED;*/

// File system object.
SdFat sd;

// Serial streams
ArduinoOutStream cout(Serial);

// input buffer for line
char cinBuf[40];
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));

// SD card chip select
int chipSelect = 53;

String Zahl = "0"; // Zahl in string var

unsigned long temp_millis = millis();
unsigned long temp_volt_millis = millis() + 10000;

int n = 0;
int raupe = 0;
float volt = 0;

int speicher[10] = {0};
uint8_t tempRotation;

uint8_t tempCursorX;
uint8_t tempCursorY;

String info = "mirwal pro";
// String info = "1234567890";
//  ############################### ############################### ###############################

static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
int pulses, A_SIG = 0, B_SIG = 1;

// ############################### ############################### ###############################

void setup()
{
	display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);

	display1.clearDisplay();
	display1.setTextColor(WHITE);
	display1.setTextSize(1);
	display1.setCursor(1, 0);
	display1.println("OLED - Display - Test");
	display1.setCursor(14, 56);
	display1.println("mirwal.de");
	display1.setTextSize(2);
	display1.setCursor(34, 15);
	display1.println("online");
	display1.setCursor(30, 34);
	display1.println("HEXAPOD");
	display1.display();

	pinMode(A0, INPUT); // Steuerkn�ppel rechts		up	 down
	pinMode(A1, INPUT); // Steuerkn�ppel rechts		left right
	pinMode(A2, INPUT); // Steuerkn�ppel links		up	 down
	pinMode(A3, INPUT); // Steuerkn�ppel links		left right
	pinMode(A4, INPUT); // Poti orginal  aux2
	pinMode(A5, INPUT); // Taster dreier
	pinMode(A6, INPUT); // Faider

	// pinMode(A7, INPUT);
	// pinMode(A8, INPUT);
	// pinMode(A9, INPUT);
	// pinMode(A10, INPUT);
	// pinMode(A11, INPUT);
	// pinMode(A12, INPUT);
	// pinMode(A13, INPUT);
	// pinMode(A14, INPUT);

	pinMode(A15, INPUT); // read Volt Batterie

	pinMode(22, INPUT); // Taster CH[#] nutzung
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(25, INPUT);
	pinMode(26, INPUT);
	pinMode(27, INPUT);
	pinMode(28, INPUT);
	pinMode(29, INPUT);

	pinMode(37, INPUT);	 // encoder Taster
	pinMode(36, OUTPUT); // encoder VCC
	digitalWrite(36, HIGH);

	pinMode(40, INPUT_PULLUP); // Taster (jstirung)
	pinMode(41, INPUT_PULLUP);

	pinMode(42, INPUT_PULLUP);
	pinMode(43, INPUT_PULLUP);

	pinMode(44, INPUT_PULLUP);
	pinMode(45, INPUT_PULLUP);

	pinMode(46, INPUT_PULLUP);
	pinMode(47, INPUT_PULLUP);

	pinMode(48, INPUT_PULLUP);
	pinMode(49, INPUT_PULLUP);

	pinMode(VIBRATOR, OUTPUT); // Vipration
	digitalWrite(VIBRATOR, LOW);

	// ################# Serial #####################

	Serial.begin(9600);
	Serial1.begin(9600);

	// while (!Serial) { SysCall::yield();} // Wait for USB Serial

	// ################# Serial ende ################

	// ################# SD #########################

	/*	if (!sd.begin(chipSelect, spiSpeed)) {
			if (sd.card()->errorCode()) {
				for (int i = 0; i < 255; i++) {
					analogWrite(VIBRATOR, i);
					delay(2);
				}
				delay(500);
				digitalWrite(VIBRATOR, LOW);
			}
			cout << F("\nCard successfully initialized.\n");
			if (sd.vol()->fatType() == 0) {
				cout << F("Can't find a valid FAT16/FAT32 partition.\n");

			}
			if (!sd.vwd()->isOpen()) {
				cout << F("Can't open root directory.\n");
			}
			cout << F("Can't determine error type\n");
		}

		cout << F("\nCard successfully initialized.\n");
		cout << endl;

		uint32_t size = sd.card()->cardSize();
		if (size == 0) {
			cout << F("Can't determine the card size.\n");
		}
		*/
	// ################# SD ende ####################

	volt = (analogRead(A15)) * (5.00 / 1023.00) * 2;

	display2.clearDisplay();
	display2.setTextColor(WHITE);
	display2.setTextSize(1);
	display2.setCursor(1, 0);
	display2.println("OLED - Display - Test");
	display2.setCursor(14, 56);
	display2.println("mirwal.de");
	display2.setTextSize(2);
	display2.setCursor(34, 15);
	display2.println("x1");
	display2.setCursor(30, 34);
	display2.println("World!");
	display2.display();

	// Encoder

	pinMode(pinA, INPUT); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	pinMode(pinB, INPUT); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
	attachInterrupt(0, A_RISE, RISING);
	attachInterrupt(1, B_RISE, RISING);
}

void loop()
{

	/* Schauen ob was rein kommt tx/rx */
	if (Serial1.available() > 0)
	{
		if (Serial1.read() == 0xF1)
		{

			int temp = Serial1.read();

			switch (temp)
			{

			case 0x90:
				info = "online... ";
				break;

			case 0x91:
				info = "AkkuObjekt";
				break;

			case 0x92:
				info = "  mirwal  ";
				break;

			case 0x93:
				info = "  TESTEN  ";
				break;

			default:
				info = "          ";
			}

			//// display rechts

			// display2.clearDisplay();
			// tempCursorX = display2.getCursorX();	tempCursorY = display2.getCursorY();	tempRotation = display2.getRotation();
			// display2.setRotation(1);	display2.setCursor(0, 0);
			// display2.print(info);
			// display2.print(temp);
			// display2.setCursor(tempCursorX, tempCursorY); display2.setRotation(tempRotation);
			//
			//

			// display2.display();
		}
	}

	/* Schauen ob was rein kommt tx/rx */

	if (temp_millis < millis())
	{
		temp_millis = millis() + 200;
		FunkeAuswerten();
		Serial1.write(0xF1); // Start Bits
		Serial1.write(0x7E);

		senden(analogRead(A0));
		senden(analogRead(A1));
		senden(analogRead(A2));
		senden(analogRead(A3));
		senden(analogRead(A4));
		senden(analogRead(A5));
		senden(analogRead(A6));

		// senden(analogRead(A7));
		// senden(analogRead(A8));
		// senden(analogRead(A9));
		// senden(analogRead(A10));
		// senden(analogRead(A11));
		// senden(analogRead(A12));
		// senden(analogRead(A13));
		// senden(analogRead(A14));
		// senden(analogRead(A15));

		Serial1.print(HR1_tmp);
		Serial1.print(HR2_tmp);
		Serial1.print(HR3_tmp);
		Serial1.print(HL1_tmp);
		Serial1.print(HL2_tmp);
		Serial1.print(HL3_tmp);
		Serial1.print(TRAINER_tmp);
	} // Serial1.print('\n');

	if (digitalRead(HL3))
	{

		display1.clearDisplay();
		display1.setCursor(1, 1);

		display1.println(analogRead(Poti, Poti_Offset));
		display1.println(analogRead(HL_UD, HL_UD_Offset));
		display1.println(analogRead(HL_LR, HL_LR_Offset));
		display1.println(analogRead(HR_UD, HR_UD_Offset));
		display1.println(analogRead(HR_LR, HR_LR_Offset));
		display1.println(analogRead(Flap, Flap_Offset));
		display2.println(analogRead(Fader, 0));

		int T = (analogRead(Poti, Poti_Offset) / 20);
		display1.drawRect(20 - T, 20 - T, 30 + (T * 2), 30 + (T * 2), WHITE);

		display1.drawPixel(0 + analogRead(Poti, 0), 0, WHITE);
		display1.drawPixel(0 + analogRead(HR_UD, HR_UD_Offset), 3, WHITE);
		display1.drawPixel(0 + analogRead(HR_LR, HR_LR_Offset), 4, WHITE);
		display1.drawPixel(0 + analogRead(Fader, 0), 5, WHITE);

		display1.drawCircle(66 - (analogRead(HL_LR, HL_LR_Offset) / 2), 66 - (analogRead(HL_UD, HL_UD_Offset) / 2), (1 + analogRead(Flap, 0) / 20), WHITE);

		tempCursorX = display1.getCursorX();
		tempCursorY = display1.getCursorY();
		tempRotation = display1.getRotation();
		display1.setRotation(1);
		display1.setCursor(0, 0);
		display1.println(info);

		if (temp_volt_millis < millis())
		{
			volt = (analogRead(A15)) * (5.00 / 1023.00) * 2;
			temp_volt_millis = millis() + 10000;
		}
		if (volt < 7.4)
			display1.setTextSize(2);
		display1.println(volt);
		display1.setTextSize(1);
		display1.setCursor(tempCursorX, tempCursorY);
		display1.setRotation(tempRotation);

		display1.display();

		display2.clearDisplay();
		display2.setTextColor(WHITE);
		display2.setTextSize(1);
		display2.setCursor(12, 1);
		display2.println(info);
		display2.print("-> ");
		display2.println(pulses);
		display2.display();

		if (!digitalRead(41))
		{
			// display rechts
			display2.clearDisplay();
			display2.display();
		}
	}
	else
	{
	}

	if (digitalRead(TRAINER))
		display1.clearDisplay();
}

void senden(int analog)
{
	uint16_t MSB = (analog / 256);
	uint16_t LSB = analog - MSB;
	Serial1.write(LSB);
	Serial1.write(MSB);
}

int codieren(int analog, uint16_t *MSB, uint16_t *LSB)
{
	*MSB = (analog / 256);
	*LSB = analog - *MSB;
}

int decodiren(uint16_t MSB, uint16_t LSB)
{
	return (LSB + (MSB * 256) + MSB);
}

void FunkeAuswerten()
{

	Poti_tmp = analogRead(Poti, Poti_Offset);
	HL_UD_tmp = analogRead(HL_UD);
	HL_LR_tmp = analogRead(HL_LR);
	HR_UD_tmp = analogRead(HR_UD);
	HR_LR_tmp = analogRead(HR_LR);
	HL1_tmp = digitalRead(HL1);
	HL2_tmp = digitalRead(HL2);
	HL3_tmp = digitalRead(HL3);
	HR1_tmp = digitalRead(HR1);
	HR2_tmp = digitalRead(HR2);
	HR3_tmp = digitalRead(HR3);

	FLAP_tmp = digitalRead(FLAP_tmp);
	TRAINER_tmp = digitalRead(TRAINER);
}

int analogRead(int aR, int offset)
{
	if (offset > 0 || offset < 0)
		return map(analogRead(aR), 0, 1024, 0, 128) - offset;
	if (offset == 0)
		return map(analogRead(aR), 0, 1024, 0, 128);
	return 404;
}

void A_RISE()
{
	detachInterrupt(0);
	A_SIG = 1;

	if (B_SIG == 0)
		pulses++; // moving forward
	if (B_SIG == 1)
		pulses--; // moving reverse
	attachInterrupt(0, A_FALL, FALLING);
}

void A_FALL()
{
	detachInterrupt(0);
	A_SIG = 0;

	if (B_SIG == 1)
		pulses++; // moving forward
	if (B_SIG == 0)
		pulses--; // moving reverse
	attachInterrupt(0, A_RISE, RISING);
}

void B_RISE()
{
	detachInterrupt(1);
	B_SIG = 1;

	if (A_SIG == 1)
		pulses++; // moving forward
	if (A_SIG == 0)
		pulses--; // moving reverse
	attachInterrupt(1, B_FALL, FALLING);
}

void B_FALL()
{
	detachInterrupt(1);
	B_SIG = 0;

	if (A_SIG == 0)
		pulses++; // moving forward
	if (A_SIG == 1)
		pulses--; // moving reverse
	attachInterrupt(1, B_RISE, RISING);
}