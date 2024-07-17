#include <Servo.h>
#include <LiquidCrystal.h>

Servo servo;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int pos = 0;
int buttonApin = 2;
int buttonBpin = 3;
int buttonCpin = 4;
int buttonDpin = 5;
int waitHours = 0;
int waitMinutes = 0;
int waitSeconds = 0;
long waitTime =  0; //Hours, Minutes, and Seconds variables are just for visuals, time is for actual wait time

void setup() 
{
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  
  pinMode(buttonCpin, INPUT_PULLUP);  
  pinMode(buttonDpin, INPUT_PULLUP);
  Serial.begin(9600);
  servo.attach(6);
  servo.write(0);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Enter time: ");
  lcd.setCursor(0, 1);
  lcd.print("00 H 00 M 00 S");
}

void updateTime()
{
  lcd.setCursor(0, 1);
  if(waitHours < 10) //Allows proper positioning for printing of time
  {
    lcd.print(0);
  }
  lcd.print(waitHours);
  
  lcd.setCursor(5, 1);
  if(waitMinutes < 10) //Allows proper positioning for printing of time
  {
    lcd.print(0); 
  }
  lcd.print(waitMinutes);
  lcd.setCursor(10, 1);
  if(waitSeconds < 10)
  {
    lcd.print(0);
  }
  lcd.print(waitSeconds);
}

void checkLimits()
{
  if(waitHours > 99) //limit at 99 hours
    {
      waitHours = 0;
      waitTime -= 360000000; //takes away the 100 hours of wait added to waitTime
      lcd.setCursor(0, 1);
      lcd.print(0);
    }
  updateTime();
}

void calculateTime()
{
  waitHours = (waitTime/3600000);
  waitMinutes = ((waitTime % 3600000)/60000);
  waitSeconds = ((waitTime % 60000)/1000);
  checkLimits();
  lcd.setCursor(2,1);
}

void loop() 
{
  if (digitalRead(buttonApin) == LOW)
  {
    waitTime += 3600000;
    calculateTime();
    delay(250); //Stops accidental holding of button causing huge numbers
  }
  if (digitalRead(buttonBpin) == LOW)
  {
    waitTime += 900000;
    calculateTime();
    delay(250);
  }
  if (digitalRead(buttonCpin) == LOW)
  {
    waitTime += 60000;
    calculateTime();
    delay(250);
  }
  if (digitalRead(buttonDpin) == LOW)
  {
    lcd.setCursor(0, 0);
    lcd.print("Locked!      ");
    servo.write(90);
    while(waitTime > 0)
    {
      delay(1000);
      waitTime -= 1000; //This wont be exact, but it will be close enough for the purpose of this product
      calculateTime();
    }
    lcd.setCursor(0, 0);
    lcd.print("Unlocked!");
    servo.write(0);
    delay(2000);
    lcd.setCursor(0, 0);
    lcd.print("Enter time: ");
  }
}
