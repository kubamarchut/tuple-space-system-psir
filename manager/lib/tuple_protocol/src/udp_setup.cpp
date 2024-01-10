#include "udp_setup.h"

#define UDP_TUPLE_SPACE_PORT 5000
#define LOCAL_PORT 5010

byte MAC[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01};

unsigned int localPort = LOCAL_PORT;
ZsutIPAddress client_ip = ZsutIPAddress(172, 30, 196, 67);

ZsutEthernetUDP Udp;

void setupUDP()
{
  ZsutEthernet.begin(MAC);
  Udp.begin(localPort);
}
int send_udp_packet(unsigned char *buffer, int length)
{
  Udp.beginPacket(client_ip, UDP_TUPLE_SPACE_PORT);
  Udp.write(buffer, length);
  Udp.endPacket();

  return 1;
}
int receive_udp_packet(char *buffer, int length)
{
  int packetSize = Udp.parsePacket();
  if (packetSize > 0)
  {
    Udp.read(buffer, length);
    return packetSize;
  }
  return 0;
}