
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

// These are 'flexible' lines that can be changed
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset

#define button 3
#define select 4
#define down 5

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HX8357D Test!");

  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
  Serial.print("Display Power Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDMADCTL);
  Serial.print("MADCTL Mode: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDCOLMOD);
  Serial.print("Pixel Format: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDIM);
  Serial.print("Image Format: 0x");
  Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDSDR);
  Serial.print("Self Diagnostic: 0x");
  Serial.println(x, HEX);

  Serial.println(F("Benchmark                Time (microseconds)"));

  tft.setRotation(1);

  Serial.print(F("Text                     "));
  //  Serial.println(testText());
  delay(500);

  Serial.print(F("Lines                    "));
  //  Serial.println(testLines(HX8357_CYAN));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  //  Serial.println(testRects(HX8357_GREEN));
  delay(500);

  tft.fillScreen(HX8357_BLACK);
  Serial.print(F("Circles (outline)        "));
  //  Serial.println(testCircles(10, HX8357_RED));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  //  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  //  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  //  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  //  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F("Done!"));

  pinMode(button, INPUT);
}

bool hasDisplayUpdated;

int player1x = 70;
int player1y = 0;

int player2x = 260;
int player2y = 0;

int player3x = 70;
int player3y = 120;

int player4x = 260;
int player4y = 120;

int currentPlayer = 1;
int player1 = 40;
int player2 = 40;
int player3 = 40;
int player4 = 40;

int prevPlayer1Life = 40;
int prevPlayer2Life = 40;
int prevPlayer3Life = 40;
int prevPlayer4Life = 40;

int welcome = 1;

int yoffset = 30;
int xoffset = 10;

unsigned int p1color = HX8357_RED;
unsigned int p2color = HX8357_WHITE;
unsigned int p3color = HX8357_WHITE;
unsigned int p4color = HX8357_WHITE;
int buttonState;
int selectState;
int downState;

void loop()
{
  // put your main code here, to run repeatedly:
  if (welcome)
  {
    welcomeScreen();
    welcome = 0;
  }

  buttonState = digitalRead(button);
  selectState = digitalRead(select);
  downState = digitalRead(down);

  if (buttonState)
  {
    increaseLife();
    hasDisplayUpdated = false;
  }
  else if (selectState)
  {
    updateActivePlayer();
  }
  else if (downState)
  {
    decreaseLife();
    hasDisplayUpdated = false;
  }
  //updatePlayerDisplayColors();
  setupPlayers();
}

void updateActivePlayer()
{
  delay(200);
  if (currentPlayer == 4)
  {
    currentPlayer = 1;
  }
  else
  {
    currentPlayer++;
  }
  updatePlayerDisplayColors();
  return;
}

void updatePlayerDisplayColors()
{
  p1color = HX8357_WHITE;
  p2color = HX8357_WHITE;
  p3color = HX8357_WHITE;
  p4color = HX8357_WHITE;
  if (currentPlayer == 1)
  {
    p1color = HX8357_RED;
  }
  else if (currentPlayer == 2)
  {
    p2color = HX8357_RED;
  }
  else if (currentPlayer == 3)
  {
    p3color = HX8357_RED;
  }
  else
  {
    p4color = HX8357_RED;
  }
}

void welcomeScreen()
{
  tft.setTextSize(2);
  tft.setCursor(50, 50);
  tft.println("Welcome to Magic the Gathering - A Life Counter");

  tft.println("by Michaela Gartner && Mackenzie Bullock");

  delay(5000);
  tft.fillScreen(HX8357_BLACK);
}

void setupPlayers()
{

  blackOutDisplay();
  tft.setCursor(70, 0);
  tft.setTextColor(p1color);
  tft.setTextSize(2);
  tft.println("Player 1");
  tft.setCursor(70 + xoffset, 0 + yoffset);
  tft.setTextSize(6);
  tft.println(player1);

  tft.setCursor(260, 0);
  tft.setTextColor(p2color);
  tft.setTextSize(2);
  tft.println("Player 2");
  tft.setCursor(260 + xoffset, 0 + yoffset);
  tft.setTextSize(6);
  tft.println(player2);

  tft.setCursor(70, 120);
  tft.setTextColor(p3color);
  tft.setTextSize(2);
  tft.println("Player 3");
  tft.setCursor(70 + xoffset, 120 + yoffset);
  tft.setTextSize(6);
  tft.println(player3);

  tft.setCursor(260, 120);
  tft.setTextColor(p4color);
  tft.setTextSize(2);
  tft.println("Player 4");
  tft.setCursor(260 + xoffset, 120 + yoffset);
  tft.setTextSize(6);
  tft.println(player4);
}

void increaseLife()
{
  //tft.fillScreen(HX8357_BLACK);

  if (currentPlayer == 1)
  {
    prevPlayer1Life = player1;
    player1++;
  }
  else if (currentPlayer == 2)
  {
    prevPlayer2Life = player2;
    player2++;
  }
  else if (currentPlayer == 3)
  {
    prevPlayer3Life = player3;
    player3++;
  }
  else
  {
    prevPlayer4Life = player4;
    player4++;
  }
}

void decreaseLife()
{
  //  tft.fillScreen(HX8357_BLACK);
  //tft.fillRect(0, 0, 1000, 1000, HX8357_BLACK);

  if (currentPlayer == 1)
  {
    prevPlayer1Life = player1;
    player1--;
  }
  else if (currentPlayer == 2)
  {
    prevPlayer2Life = player2;
    player2--;
  }
  else if (currentPlayer == 3)
  {
    prevPlayer3Life = player3;
    player3--;
  }
  else
  {
    prevPlayer4Life = player4;
    player4--;
  }
}
void blackOutDisplay()
{
  if (!hasDisplayUpdated)
  {
    delay(200);

    if (currentPlayer == 1)
    {
      tft.setTextColor(HX8357_BLACK);
      tft.setCursor(70 + xoffset, 0 + yoffset);
      tft.setTextSize(6);
      tft.println(prevPlayer1Life);
    }
    else if (currentPlayer == 2)
    {
      tft.setTextColor(HX8357_BLACK);
      tft.setCursor(260 + xoffset, 0 + yoffset);
      tft.setTextSize(6);
      tft.println(prevPlayer2Life);
    }
    else if (currentPlayer == 3)
    {
      tft.setTextColor(HX8357_BLACK);
      tft.setCursor(70 + xoffset, 120 + yoffset);
      tft.setTextSize(6);
      tft.println(prevPlayer3Life);
    }
    else
    {
      tft.setTextColor(HX8357_BLACK);
      tft.setCursor(260 + xoffset, 120 + yoffset);
      tft.setTextSize(6);
      tft.println(prevPlayer4Life);
    }
    hasDisplayUpdated = true;
  }
}
