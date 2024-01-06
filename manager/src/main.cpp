#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>

#define UDP_TUPLE_SPACE_PORT 2001
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

byte MAC[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01}; 

unsigned int localPort = UDP_TUPLE_SPACE_PORT;
ZsutIPAddress client_ip = ZsutIPAddress(192, 168, 89, 11);

ZsutEthernetUDP Udp;

void setup()
{
  Serial.begin(115200);
  Serial.print(F("Manager init... ["));
  Serial.print(F(__FILE__));
  Serial.print(F(", "));
  Serial.print(F(__DATE__));
  Serial.print(F(", "));
  Serial.print(F(__TIME__));
  Serial.println(F("]"));

  ZsutEthernet.begin(MAC);

  Serial.print(F("My IP address: "));
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    Serial.print(ZsutEthernet.localIP()[thisByte], DEC);
    Serial.print(F("."));
  }
  Serial.println();

  Udp.begin(localPort);

  ZsutIPAddress client_ip = ZsutIPAddress(192, 168, 89, 11);
  /*Udp.beginPacket(client_ip, localPort);
  packetBuffer[0] = HELLO_MSG;
  Udp.write(packetBuffer, 1);
  Udp.endPacket();*/
}

void loop()
{
  int temp = 1; //REMOVE BEFOR FLIGHT - just testing compilation
  int MAX = 5000;
  for (int i = 2; i <= MAX; i++){
    Udp.beginPacket(client_ip, localPort);
    packetBuffer[0] = i;
    Udp.write(packetBuffer, 1);
    Udp.endPacket();
    delay(1000);
  }
}
