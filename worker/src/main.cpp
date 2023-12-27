#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>

#define UDP_SERVER_PORT 1972
#define MAX_BUFFER 32

char buffer[MAX_BUFFER];

byte MAC[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01}; 

#define PACKET_BUFFER_LENGTH 64
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

unsigned int localPort = UDP_SERVER_PORT;

ZsutEthernetUDP Udp;

void setup()
{
  Serial.begin(115200);
  Serial.print(F("Server B init... ["));
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

  /*ZsutIPAddress client_ip = ZsutIPAddress(192, 168, 89, 11);
  Udp.beginPacket(client_ip, localPort);
  packetBuffer[0] = HELLO_MSG;
  Udp.write(packetBuffer, 1);
  Udp.endPacket();*/
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize > 0)
  {
    int len = Udp.read(packetBuffer, PACKET_BUFFER_LENGTH);

    sprintf(buffer, "%s", packetBuffer);

    String client_message = String(buffer);

    Serial.print(F("Recieved: "));

    if (packetBuffer[0] == )
    {
      

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      packetBuffer[0] = ;
      Udp.write(packetBuffer, PACKET_BUFFER_LENGTH);
      Udp.endPacket();
    }
  }
}
