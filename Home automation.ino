// #define BLYNK_TEMPLATE_ID "TMPLp9rXc1mb"
// #define BLYNK_DEVICE_NAME "Quickstart Template"
// #define BLYNK_AUTH_TOKEN "nAqcGs511MK9702RhPOAB1P08QVifn30"
// #define BLYNK_PRINT Serial

// #include <ESP8266_Lib.h>
// #include <BlynkSimpleShieldEsp8266.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
const int ldrPin = A0;
const int Sensora = D7;
const int sensorB = D8;
const int led = D6;
const int led2 = D4;
int threshold = 500;
unsigned int count = 0;
boolean sensorATriggered = false;
unsigned long sensorATime;
byte currStateA;
byte prevStateA = HIGH;
boolean sensorBTriggered = false;
unsigned long sensorBTime;
byte currStateB;
byte prevStateB = HIGH;
int vis = 0;
boolean blinkled1 = false;
boolean blinkled2 = false;
boolean useraccess = false;

unsigned long channelNumber = 1958916;
const char *apiKey = "VD369F6E537JK7XH";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "OPPO F11";     // replace with your wifi ssid and wpa2 key
const char *pass =  "110101h@rsh";
const char* server = "api.thingspeak.com";
WiFiClient client;

// #include <BlynkSimpleShieldEsp8266.h>

// char auth[] = BLYNK_AUTH_TOKEN;

//Your WiFi credentials.
//Set password to "" for open networks.
// char ssid[] = "Aa";
// char pass[] = "ajayajmera";

//Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

//or Software Serial on Uno, Nano...
// #include <SoftwareSerial.h>
// SoftwareSerial EspSerial(0,1); // RX, TX

// #define ESP8266_BAUD 9600

// ESP8266 wifi(&EspSerial);
// BlynkTimer timer;

// void myTimerEvent()
// {
//     Blynk.virtualWrite(V5, millis() / 1000);
// }

void setup()
{
    Serial.begin(9600);
    Serial.println("Connecting to ");
    //Set ESP8266 baud rate
    // EspSerial.begin(ESP8266_BAUD);
    delay(10);
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    ThingSpeak.begin(client);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    pinMode(Sensora, INPUT);
    pinMode(sensorB, INPUT);
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);

    // Blynk.begin(auth, wifi, ssid, pass);
    //timer.setInterval(1000L, myTimerEvent);
    // Blynk.virtualWrite(V0, 1);
    // Blynk.virtualWrite(V0, 0);
    // Blynk.virtualWrite(V1, 1);
    // Blynk.virtualWrite(V1, 0);
    // Blynk.virtualWrite(V3, count);
}

// BLYNK_WRITE(V0) 
// {
//     if (param.asInt() == 1)
//     {
//         blinkled1 = true;
//     }
//     else
//     {
//         blinkled1 = false;
//     }
// }

// BLYNK_WRITE(V1) 
// {
//     if (param.asInt() == 1)
//     {
//         blinkled2 = true;
//     }
//     else
//     {
//         blinkled2 = false;
//     }
// }

// BLYNK_WRITE(V2) 
// {
//     if (param.asInt() == 1)
//     {
//         useraccess = true;
//     }
//     else
//     {
//         useraccess = false;
//         Serial.println(String("No. of visitors : ") + count);
//     }
// }

// BLYNK_CONNECTED()
// {
//     Blynk.syncVirtual(V0);
//     Blynk.syncVirtual(V1); 
// }

void loop()
{
    currStateA = digitalRead(Sensora);

    if (vis == 0)
    {
        Serial.print("No Visitors = 0");
        Serial.println(" ");
        vis++;
    }

    if (currStateA != prevStateA)
    {
        if (currStateA == LOW)
        {
            sensorATriggered = true;

            sensorATime = millis();
        }
    }
    currStateB = digitalRead(sensorB);
    if (currStateB != prevStateB)
    {
        if (currStateB == LOW)
        {
            sensorBTriggered = true;
            sensorBTime = millis();
        }
    }
    if (sensorBTriggered && sensorATriggered)

    {

        if (sensorATime > sensorBTime)

        {
            count++;
            Serial.println(String("No. of visitors : ") + count);
  //          Blynk.virtualWrite(V3, count);
            if (client.connect(server,80)){
              ThingSpeak.setField(1,int(count));
              ThingSpeak.writeFields(channelNumber, apiKey);
            }
            client.stop();
            Serial.println("Waiting...");
            delay(500);
        }

        if (sensorBTime > sensorATime)

        {
            if (count > 0)
            {
                count--;
                Serial.println(String("No. of visitors : ") + count);
               // Blynk.virtualWrite(V3, count);
               if (client.connect(server,80)){
              ThingSpeak.setField(1,int(count));
              ThingSpeak.writeFields(channelNumber, apiKey);
            }
            client.stop();
            Serial.println("Waiting...");
                delay(500);
            }
        }

        sensorATriggered = false;

        sensorBTriggered = false;
    }

    if (useraccess == true)
    {
        if (blinkled1 == true)
        {
            digitalWrite(led, HIGH);  
            delay(50);    
        }
        else
        {
            digitalWrite(led, LOW);
            delay(200);
        }

        if (blinkled2 == true)
        {
            digitalWrite(led2, HIGH);
            delay(50);
        }
        else
        {
            digitalWrite(led2, LOW);
            delay(200);
        }
    }
    if (useraccess == false)
    {
        if (count <= 0)
        {
            digitalWrite(led, LOW);
            digitalWrite(led2, LOW);
            //Blynk.virtualWrite(V0, 0);
            //Blynk.virtualWrite(V1, 0);
            blinkled1 = false;
            blinkled2 = false;
        }
        else
        {
            int data = analogRead(ldrPin);
            if (data <= threshold)
            {
                digitalWrite(led, HIGH);
                delay(50);
                //Blynk.virtualWrite(V0, 1);
                // blinkled1 = true;
                if (count >= 3)
                {
                    digitalWrite(led2, HIGH);
                    delay(50);
                    //Blynk.virtualWrite(V1, 1);
                    // blinkled2 = true;
                }
                else
                {
                    digitalWrite(led2, LOW);
                    //Blynk.virtualWrite(V1, 0);
                    // blinkled2 = false;
                    delay(200);
                }
            }
            else
            {
                digitalWrite(led, LOW);
                digitalWrite(led2,LOW);
                //Blynk.virtualWrite(V0, 0);
                blinkled1 = false;
                delay(200);
            }
        }
    }
    // Blynk.run();
    // timer.run(); // Initiates BlynkTimer
}
