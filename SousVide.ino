// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

float desired_temp_H = 56.0;
float desired_temp_L = 53.0;
int temp_switch = 0;
int bt1_preState = 0;
int bt2_preState = 0;
int bt3_preState = 0;
int count1 = 0;
int count2 = 0;
int count3 = 0;
long buttonTime = 0;
const int button1Pin = 3;
const int button2Pin = 4;
const int button3Pin = 5;
const int relayPin = 6;
DeviceAddress insideThermometer;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(115200);
  // Start up the library
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(relayPin, OUTPUT);
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
  sensors.setResolution(insideThermometer,9);
  sensors.begin();
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
}

/*
 * Main function, get and show the temperature
 */
long require_time = 0;

float temperature = 0.0;
void loop(void)
{ 
  if(millis() - require_time > 300)
  {
    require_time = millis();
    sensors.requestTemperatures();
    Serial.print("Temperature: ");
    temperature = sensors.getTempC(insideThermometer);
    Serial.println(temperature);
    Serial.print("temp_switch:  ");
    Serial.println(temp_switch);
    Serial.print("desired_temp_H:  ");
    Serial.println(desired_temp_H);
    Serial.print("desired_temp_L:  ");
    Serial.println(desired_temp_L);
  }
  detectButton();
  /*
  Serial.print("Count1:  ");
  Serial.println(count1);
  Serial.print("Count2:  ");
  Serial.println(count2); 
  Serial.print("Count3:  ");
  Serial.println(count3);
  */
  if(temperature>=desired_temp_H)  
  {
    digitalWrite(relayPin, LOW);
  }
  else if(temperature<=desired_temp_L)
  {
    digitalWrite(relayPin, HIGH);
  }
  /*
  else
  {
    digitalWrite(relayPin, LOW);
  }*/
  
}


void detectButton()
{
  if(bt1_preState == 0)
  {
    if(digitalRead(button1Pin) == HIGH)
    {
      bt1_preState = 1;
      buttonTime = millis();
    }
  }else if( bt1_preState == 1)
  {
    if(digitalRead(button1Pin) == HIGH & millis() - buttonTime > 150)
    {
      count1++;
      if(temp_switch ==1)
      {
        desired_temp_H-=0.5;
      }
      else
      {
        desired_temp_L-=0.5;
      }
      buttonTime = millis();
    }
  }

  if(bt2_preState == 0)
  {
    if(digitalRead(button2Pin) == HIGH)
    {
      bt2_preState = 1;
      buttonTime = millis();
    }
  }else if( bt2_preState == 1)
  {
    if(digitalRead(button2Pin) == HIGH & millis() - buttonTime > 150)
    {
      count2++;
      temp_switch=temp_switch==1?0:1;
      buttonTime = millis();
    }
  }

  if(bt3_preState == 0)
  {
    if(digitalRead(button3Pin) == HIGH)
    {
      bt3_preState = 1;
      buttonTime = millis();
    }
  }else if( bt3_preState == 1)
  {
    if(digitalRead(button3Pin) == HIGH & millis() - buttonTime > 150)
    {
      count3++;
      if(temp_switch ==1)
      {
        desired_temp_H+=0.5;
      }
      else
      {
        desired_temp_L+=0.5;
      }
      buttonTime = millis();
    }
  }
  
}





