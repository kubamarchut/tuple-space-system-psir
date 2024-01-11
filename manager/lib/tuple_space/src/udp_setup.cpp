#include "udp_setup.h"

#define UDP_TUPLE_SPACE_PORT 5000
#define LOCAL_PORT 5010

byte MAC[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01};

unsigned int localPort = LOCAL_PORT;
ZsutIPAddress client_ip = ZsutIPAddress(172, 21, 161, 164);

ZsutEthernetUDP Udp;

int setupUDP(){
  ZsutEthernet.begin(MAC);
  int socket = Udp.begin(localPort);

  return socket;
}
int send_udp_packet(unsigned char *buffer, int length){
  int connection = Udp.beginPacket(client_ip, UDP_TUPLE_SPACE_PORT);
  if (connection == 1)
  {
    Udp.write(buffer, length);
    int success = Udp.endPacket();

    return success;
  }
  // 0 means error 1 means success
  return connection;
}

int receive_udp_packet(char *buffer, int length){
  int packetSize = Udp.parsePacket();
  if (packetSize > 0)
  {
    int bytesRead = Udp.read(buffer, length);
    if (bytesRead > 0)
    {
      return packetSize;
    }
  }

  return -1;
}

int receive_udp_packet_timeout(char *buffer, int length, int timeout_seconds) {
  unsigned long startTime = ZsutMillis();

  while ((ZsutMillis() - startTime) < timeout_seconds * 1000) {
    int packetSize = Udp.parsePacket();
    if (packetSize > 0) {
      int bytesRead = Udp.read(buffer, length);
      if (bytesRead > 0) {
        return packetSize;
      }
    }
  }

  return -1;
}