// "C:\Users\Jericho\AppData\Local\Temp\arduino\sketches\64DF8B51099D20B103D56A9A01531AB3"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <HX711.h>

#define opDelay 200 // LOWER VALUES TO COMPENSATE FOR SIMULATION SLOWDOWNS

// KEYPAD SETUP
const int rowNum = 4;
const int colNum = 3;
char keys[rowNum][colNum] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[rowNum] = {A0,A1,A2,A3};
byte pin_cols[colNum] = {8,9,A5};
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_cols, rowNum, colNum);

// LCD SETUP
int rs = 6;
int en = 7;
int d4 = 2;
int d5 = 3;
int d6 = 4;
int d7 = 5;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

// PIN NAMES
int h2oPump = 11;
int coffeeGrounds = 12;
int basketLid = 13;
int powerRelay = 10;

// STATUS FLAGS
int waterPumping = 0;
int groundDispensing = 0;
int lidOpen = 0;

// WEIGHTS
int maxWaterWeight = 180;     // FOR 1 CUP AT 1:16.67
int maxGroundsWeight = 10.80; // FOR 1 CUP AT 1:16.67
int cups = 1;

// HX711 -- UNUSED
int loadCell_SCK = 0;
int loadCell_DOUT = 1;
HX711 scale;

// FOR DEMONSTRATION MODULE
int demoPin = A4;

int waterPumpOn(int on)
{
 if (on)
 {
  digitalWrite(h2oPump, HIGH);
  return 1;
 }
 else
 {
  digitalWrite(h2oPump, LOW);
  return 0;
 }
}

int lidState(int state)
{
 int i;
 switch(state)
 {
  case 0:              // CLOSE AND ROTATE TO 0 DEG
    for(i=0;i<50;i++)
    {
      
      digitalWrite(basketLid, HIGH);
      delay(1.0);
      digitalWrite(basketLid, LOW);
      delay(19.0);
    }
    return 0;
  case 1:             // OPEN AND ROTATE TO 90 DEG
    for(i=0;i<50;i++) 
    {
      digitalWrite(basketLid, HIGH);
      delay(1.5);
      digitalWrite(basketLid, LOW);
      delay(18.5);
    }
    return 1;
 }
}

int groundState(int state)
{
 int i;
 switch(state)
 {
  case 0:              // CLOSE
    for(i=0;i<50;i++)  // ROTATE TO 0 DEG
    {
      digitalWrite(coffeeGrounds, HIGH);
      delay(1);
      digitalWrite(coffeeGrounds, LOW);
      delay(19);
    }
    return 0;
  case 1:             // OPEN
    for(i=0;i<50;i++) // ROTATE TO 90 DEG
    {
      digitalWrite(coffeeGrounds, HIGH);
      delayMicroseconds(1500);
      digitalWrite(coffeeGrounds, LOW);
      delayMicroseconds(18500);
    }
    return 1;
 }
}

unsigned long readScale()
{
  unsigned long count;
  unsigned char i;
  pinMode(loadCell_DOUT, OUTPUT);
  digitalWrite(loadCell_DOUT, HIGH);
  digitalWrite(loadCell_SCK, LOW);
  count = 0;
  pinMode(loadCell_DOUT, INPUT);
  while(digitalRead(loadCell_DOUT));
  for (i = 0; i < 24; i++)
  {
    digitalWrite(loadCell_SCK, HIGH);
    count = count << 1;
    digitalWrite(loadCell_SCK, LOW);
    if (digitalRead(loadCell_DOUT))
    {
      count++;
    }
  }
  digitalWrite(loadCell_SCK, HIGH);
  count = count ^ 0x800000;
  digitalWrite(loadCell_SCK, LOW);
  return count;
}

int setCoffeeWeight(int ratio, int waterWeight)
{
  return ratio * waterWeight;
}

float roundToTwoDec(float x)
{
  float value = (int) (x * 100 + .5);
  return (value/100);
}

void setup() 
{
  pinMode(loadCell_DOUT, INPUT);
  pinMode(powerRelay, OUTPUT);
  pinMode(h2oPump, OUTPUT);
  pinMode(coffeeGrounds, OUTPUT);
  pinMode(basketLid, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.println("SELECT CUPS NO.: ");
  lcd.setCursor(0,1);
  lcd.println("Press NumKey.");
}

void loop() 
{
  int sensorADC;
  float sensorFinal;
  sensorADC = analogRead(demoPin);
  sensorFinal = (sensorADC * 5.00) / 10;

  int waterWeight = maxWaterWeight;
  int groundsWeight = maxGroundsWeight;
  int tempCups = cups;
  // GET CUPS
  char key = keypad.waitForKey();
  if (key != '*' || key != '#' || key != '0')
  {
    tempCups = key - '0';
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cups: ");
    lcd.println(key);
    delay(opDelay);
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.println("Consult Manual.");
  }
  // GET COFFEE TO H2O RATIO
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("COFY/H2O RATIO?");
  lcd.setCursor(0,1);
  lcd.println("1=M; 2=N; 3=L");
  key = keypad.waitForKey();
  if (key != '*' || key != '#' || key != '0')
  {
    int ratio;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("COFY/H2O RATIO:");
    switch(key)
    {
      case 1:
        ratio = 0.056;
        groundsWeight = setCoffeeWeight(ratio, waterWeight);
        lcd.setCursor(0,1);
        lcd.println("NORMAL");
        break;
      case 2:
        ratio = 0.06;
        groundsWeight = setCoffeeWeight(ratio, waterWeight);
        lcd.setCursor(0,1);
        lcd.println("NORMAL");
        break;
      case 3:
        ratio = 0.067;
        groundsWeight = setCoffeeWeight(ratio, waterWeight);
        lcd.setCursor(0,1);
        lcd.println("STRONG");
        break;
      default:
        break;
    }
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.println("Consult Manual.");
  }
  // Modify according to cup
  waterWeight *= tempCups;
  groundsWeight *= tempCups;
  // DO THE THING
  // OPEN THE LID
  if (!lidOpen){
    lidOpen = lidState(1);
  }
  delay(opDelay);
  sensorFinal = 0;
  // POUR THE WATER
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("POUR H2O: ");
  while(sensorFinal <= 50) // sensorFinal <= waterWeight IF suitable sensor used
  {
    sensorADC = analogRead(demoPin);
    sensorFinal = (sensorADC * 5.00) / 10;
    waterPumping = waterPumpOn(1);
    if (waterPumping)
    {
      lcd.setCursor(0,1);
      lcd.print(sensorFinal);
      lcd.println(" ml");
    }
    else
    {
      lcd.setCursor(0,1);
      lcd.println("PROG FAIL!");
    }
  }
  waterPumping = waterPumpOn(0);
  delay(opDelay);
  // UNLEASH THE GROUNDS
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("DISP. COFY: ");
  while (sensorFinal <= 100) // sensorFinal <= (waterWeight + groundsWeight) if suitable sensor used
  {
    sensorADC = analogRead(demoPin);
    sensorFinal = (sensorADC * 5.00) / 10;
    groundDispensing = groundState(1);
    if (groundDispensing)
    {
      lcd.setCursor(0,1);
      lcd.print(sensorFinal);
      lcd.println(" g");
    }
    else
    {
      lcd.setCursor(0,1);
      lcd.println("PROG FAIL!");
    }
  }
  lidOpen = lidState(0);
  delay(opDelay);
  if (!lidOpen)
  {
    digitalWrite(powerRelay, HIGH); //  TURN ON DRIP MACHINE
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("BREWING.");
    lcd.setCursor(0,1);
    lcd.println("ALERT: HOT!");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("ERROR!");
    lcd.setCursor(0,1);
    lcd.println("PROG FAIL!");
  }
  delay(opDelay); // REPLACE WITH EXPECTED TIME TO FINISH
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("COFY DONE!");
  lcd.setCursor(0,1);
  lcd.println("ANYKEY=OFF");
  key = keypad.waitForKey();
  if (key)
  {
    powerRelay = 0;
  }
}
