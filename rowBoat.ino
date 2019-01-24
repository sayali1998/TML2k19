#include<WiFiUdp.h>
#include<ESP8266WiFi.h>

#define m1 D1
#define m2 D2
#define m3 D3
#define m4 D4

const char* ssid="Markiv";
const char* password="87654321";

const int SERVER_PORT=1111;
const int BAUD_RATE=115200;

byte incomingByte=0;

const int FORWARD=51;
const int LEFT=53;
const int RIGHT=49;

const int RESET=57;

byte packetBuffer[512];

WiFiUDP Udp;
IPAddress ip;

void moveForward()
{
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
  }

void moveLeft()
{
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
  }

void moveRight()
{
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
  }

void resetAll()
{
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
  }

void connectWifi()
{
  Serial.println("Connecting to Wifi Network");
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    }
    Serial.println("WiFi Connected");
    Udp.begin(SERVER_PORT);
    ip=WiFi.localIP();
    Serial.println(ip);
  }


void detectInput()
{
  if(incomingByte==FORWARD)
    moveForward();
  else if(incomingByte==LEFT)
    moveLeft();
  else if(incomingByte==RIGHT)
    moveRight();
  else if(incomingByte==RESET)
    resetAll();
  }

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  connectWifi();
}



void loop() {
  int noBytes=Udp.parsePacket();
  String received_command="";
  if(noBytes)
  {
    Serial.print(millis()/1000);
    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.print(Udp.remotePort());

    Udp.read(packetBuffer,noBytes);
    Serial.println();

    if(noBytes>1)
    {
      incomingByte=0;
      Serial.println("Greater than 1");
      for (int i =0; i<noBytes; i++)
      {
        int flag;
        if(i==0)
          flag=10;
        else
          flag=1;
        incomingByte=incomingByte+ ((packetBuffer[i]-48)*flag);
        }
        incomingByte=incomingByte+48;
        Serial.println(incomingByte);
        Serial.println();
      }
      else
      {
        Serial.println(packetBuffer[0]);
        incomingByte=packetBuffer[0];
        Serial.println();
        }
        detectInput();
    }  

}
