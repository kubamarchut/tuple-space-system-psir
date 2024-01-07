#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_protocol.h>

#define UDP_TUPLE_SPACE_PORT 2001
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

byte MAC[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01}; 

unsigned int localPort = UDP_TUPLE_SPACE_PORT;
ZsutIPAddress client_ip = ZsutIPAddress(192, 168, 89, 11);

ZsutEthernetUDP Udp;
FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

void setup()
{
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE);
  stdout = &f_out;
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
    field_t my_tuple[2];    /* an array of fields (name not included) */

    /* make a tuple */
    my_tuple[0].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = 128;
    my_tuple[1].is_actual = TS_YES;
    my_tuple[1].type = TS_FLOAT;
    my_tuple[1].data.float_field = 3.14;

    printf("size of int: %d\n", sizeof(float));

    unsigned char packet[1024];
    //int packet_len;
   // int packet_len = serializePacket(packet, TS_CMD_OUT, "nice_constants", my_tuple, 2);
    
    //displayProtocolBytes(packet, packet_len, 14);

    /* add a tuple to the tuple space */
    ts_out("nice_constants", my_tuple, 2);

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
