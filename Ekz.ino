// Program to exercise the MD_MAX72XX library
//
// Uses most of the functions in the library
#include <MD_MAX72xx.h>
//#include <SPI.h>
#include <Adafruit_GFX.h> // эти 3 скачиваем из итернета
#include <Max72xxPanel.h>
#include <MD_Parola.h>

// Turn on debug statements to the serial output
#define  DEBUG  1

#if  DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)

#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)

#endif

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES  4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS


// SPI hardware interface
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary pins
// MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
const uint16_t WAIT_TIME = 1000;
// We always wait a bit between updates of the display
#define  DELAYTIME  50  // in milliseconds

const uint16_t forPrintHello = 100;
void scrollText(char *p)
{
  uint8_t charWidth;
  uint8_t cBuf[8];  // this should be ok for all built-in fonts

  mx.clear();
  char str[] = {'H', 'e', 'l', 'l', 'o', 0};  // с нулевым символом на конце
  while (*p != '\0')
  {
    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
    
   
    for (uint8_t i=0; i<=charWidth; i++)  // allow space between characters
    {
      Serial.write(str[i]);
      //delay(forPrintHello);
      mx.transform(MD_MAX72XX::TSL);
      if (i < charWidth)
      mx.setColumn(0, cBuf[i]);
      delay(DELAYTIME);
    }
    
  }
}


void intensity()
// Demonstrates the control of display intensity (brightness) across
// the full range.
{
  uint8_t row;

  PRINTS("\nVary intensity ");

  mx.clear();

  // Grow and get brighter
  row = 0;
  for (int8_t i=0; i<=MAX_INTENSITY; i++)
  {
    mx.control(MD_MAX72XX::INTENSITY, i);
    if (i%2 == 0)
      mx.setRow(row++, 0xff);
    delay(DELAYTIME*3);
  }

  mx.control(MD_MAX72XX::INTENSITY, 8);
}


void blinking()
// Uses the test function of the MAX72xx to blink the display on and off.
{
  int  nDelay = 1000;

  PRINTS("\nBlinking");
  mx.clear();

  while (nDelay > 0)
  {
    mx.control(MD_MAX72XX::TEST, MD_MAX72XX::ON);
    delay(nDelay);
    mx.control(MD_MAX72XX::TEST, MD_MAX72XX::OFF);
    delay(nDelay);

    nDelay -= DELAYTIME;
  }
}


void scanLimit(void)
// Uses scan limit function to restrict the number of rows displayed.
{
  PRINTS("\nScan Limit");
  mx.clear();

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t row=0; row<ROW_SIZE; row++)
    mx.setRow(row, 0xff);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  for (int8_t s=MAX_SCANLIMIT; s>=0; s--)
  {
    mx.control(MD_MAX72XX::SCANLIMIT, s);
    delay(DELAYTIME*5);
  }
  mx.control(MD_MAX72XX::SCANLIMIT, MAX_SCANLIMIT);
}

void transformation1()
// Demonstrates the use of transform() to move bitmaps on the display
// In this case a user defined bitmap is created and animated.
{
  uint8_t arrow[COL_SIZE] =
  {
    0b00001000,
    0b00011100,
    0b00111110,
    0b01111111,
    0b00011100,
    0b00011100,
    0b00111110,
    0b00000000
  };

  MD_MAX72XX::transformType_t  t[] =
  {
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TFLR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TRC,
    MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
    MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
    MD_MAX72XX::TFUD,
    MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
    MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
    MD_MAX72XX::TINV,
    MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
    MD_MAX72XX::TINV
  };

  PRINTS("\nTransformation1");
  mx.clear();

  // use the arrow bitmap
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t j=0; j<mx.getDeviceCount(); j++)
    mx.setBuffer(((j+1)*COL_SIZE)-1, COL_SIZE, arrow);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(DELAYTIME);

  // run through the transformations
  mx.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
  for (uint8_t i=0; i<(sizeof(t)/sizeof(t[0])); i++)
  {
    mx.transform(t[i]);
    delay(DELAYTIME*4);
  }
  mx.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);
}

void transformation2()
// Demonstrates the use of transform() to move bitmaps on the display
// In this case font characters are loaded into the display for animation.
{
  MD_MAX72XX::transformType_t  t[] =
  {
    MD_MAX72XX::TINV,
    MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
    MD_MAX72XX::TINV,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSD, MD_MAX72XX::TSU, MD_MAX72XX::TSD, MD_MAX72XX::TSU,
    MD_MAX72XX::TFLR, MD_MAX72XX::TFLR, MD_MAX72XX::TFUD, MD_MAX72XX::TFUD
  };

  PRINTS("\nTransformation2");
  mx.clear();
  mx.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);

  // draw something that will show changes
  for (uint8_t j=0; j<mx.getDeviceCount(); j++)
  {
    mx.setChar(((j+1)*COL_SIZE)-1, '0'+j);
  }
  delay(DELAYTIME*5);

  // run thru transformations
  for (uint8_t i=0; i<(sizeof(t)/sizeof(t[0])); i++)
  {
    mx.transform(t[i]);
    delay(DELAYTIME*3);
  }
}

void wrapText()
// Display text and animate scrolling using auto wraparound of the buffer
{
  PRINTS("\nwrapText");
  mx.clear();
  mx.wraparound(MD_MAX72XX::ON);

  // draw something that will show changes
  for (uint16_t j=0; j<mx.getDeviceCount(); j++)
  {
    mx.setChar(((j+1)*COL_SIZE)-1, (j&1 ? 'M' : 'W'));
  }
  delay(DELAYTIME*5);

  // run thru transformations
  for (uint16_t i=0; i<3*COL_SIZE*MAX_DEVICES; i++)
  {
    mx.transform(MD_MAX72XX::TSL);
    delay(DELAYTIME/2);
  }
  for (uint16_t i=0; i<3*COL_SIZE*MAX_DEVICES; i++)
  {
    mx.transform(MD_MAX72XX::TSR);
    delay(DELAYTIME/2);
  }
  for (uint8_t i=0; i<ROW_SIZE; i++)
  {
    mx.transform(MD_MAX72XX::TSU);
    delay(DELAYTIME*2);
  }
  for (uint8_t i=0; i<ROW_SIZE; i++)
  {
    mx.transform(MD_MAX72XX::TSD);
    delay(DELAYTIME*2);
  }

  mx.wraparound(MD_MAX72XX::OFF);
}

void showCharset(void)
// Run through display of the the entire font characters set
{
  mx.clear();
  mx.update(MD_MAX72XX::OFF);

  for (uint16_t i=0; i<256; i++)
  {
    mx.clear(0);
    mx.setChar(COL_SIZE-1, i);

    if (MAX_DEVICES >= 3)
    {
      char hex[3];

      sprintf(hex, "%02X", i);

      mx.clear(1);
      mx.setChar((2*COL_SIZE)-1,hex[1]);
      mx.clear(2);
      mx.setChar((3*COL_SIZE)-1,hex[0]);
    }

    mx.update();
    delay(DELAYTIME*2);
  }
  mx.update(MD_MAX72XX::ON);
}

void setup()
{
  mx.begin();
  P.begin();
#if  DEBUG
  Serial.begin(57600);
 
#endif
}

void loop()
{
#if 1

if(Serial.available()) {
   char receiveByte = Serial.read();
if (receiveByte == '1') {
  String str = "Text";
P.print(str);

Serial.println();
Serial.write('T');
Serial.write('e');
Serial.write('x');
Serial.write('t');


 delay(WAIT_TIME);
}


if (receiveByte == '2') {
  
  scrollText("Hello     ");

  
  }
  
  
if (receiveByte == '3') {
P.print(" 16:17 ");
Serial.write('1');
Serial.write('6');
Serial.write(':');
Serial.write('1');
Serial.write('7');
Serial.println();
  delay(1000);
P.print(" 16:18 ");
Serial.println();
Serial.write('1');
Serial.write('6');
Serial.write(':');
Serial.write('1');
Serial.write('8');
Serial.println();
  delay(1000);
P.print(" 16:19 ");
Serial.println();
Serial.write('1');
Serial.write('6');
Serial.write(':');
Serial.write('1');
Serial.write('9');
Serial.println();
  delay(1000);
P.print(" 16:20 ");
Serial.println();
Serial.write('1');
Serial.write('6');
Serial.write(':');
Serial.write('2');
Serial.write('0');
  delay(1000);
P.print(" 16:21 ");
Serial.println();
Serial.write('1');
Serial.write('6');
Serial.write(':');
Serial.write('2');
Serial.write('1');
Serial.println();
  delay(1000);
P.print(" 16:22 ");
Serial.println();
Serial.write('1');
Serial.write('6');
Serial.write(':');
Serial.write('2');
Serial.write('2');
Serial.println();
  delay(1000);
P.print(" 16:23 ");
  delay(1000);
P.print(" 16:24 ");
  delay(1000);
P.print(" 16:25 ");
  delay(1000);
P.print(" 16:26 ");
}
      
  }
    
//showCharset();

#endif
}
