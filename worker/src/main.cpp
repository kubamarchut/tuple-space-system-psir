#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <PrimeCheck.h>

#define UDP_TUPLE_SPACE_PORT 2001
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

byte MAC[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01}; 

unsigned int localPort = UDP_TUPLE_SPACE_PORT;

ZsutEthernetUDP Udp;

void setup()
{
  Serial.begin(115200);
  Serial.print(F("Worker init... ["));
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
  int temp = 1; //REMOVE BEFOR FLIGHT - just testing compilation
  int MAX = 100;
  for (int i = 2; i <= MAX; i++){
    int prime = isPrime(i);
    Serial.print(i, DEC);
    if (prime == 1){
      Serial.print(F(" is prime\n"));
    }
    else{
      Serial.print(F(" is not prime\n"));
    }
    delay(100);
  }
  int packetSize = Udp.parsePacket();
  if (packetSize > 0)
  {
    int len = Udp.read(packetBuffer, PACKET_BUFFER_LENGTH);

    sprintf(buffer, "%s", packetBuffer);

    String client_message = String(buffer);

    Serial.print(F("Recieved: "));

    if (packetBuffer[0] == temp)
    {
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      packetBuffer[0] = temp;
      Udp.write(packetBuffer, PACKET_BUFFER_LENGTH);
      Udp.endPacket();
    }
  }
}
