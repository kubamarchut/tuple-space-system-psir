#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_space.h>
#include <tuple_space.c>

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
}

void loop()
{
  int temp = 1; // REMOVE BEFORE FLIGHT - just testing compilation
  int MAX = 5000;
  for (int i = 2; i <= MAX; i++)
  {
    field_t my_tuple[2]; // an array of fields (name not included)
    my_tuple[0].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = i; // Replace with your desired data from tuple space
    my_tuple[1].is_actual = TS_YES;
    my_tuple[1].type = TS_FLOAT;
    my_tuple[1].data.float_field = 3.14; // Replace with your desired data from tuple space

    // Convert the tuple data to bytes
    packetBuffer[0] = my_tuple[0].data.int_field >> 8;
    packetBuffer[1] = my_tuple[0].data.int_field & 0xFF;

    uint32_t floatInt;
    memcpy(&floatInt, &my_tuple[1].data.float_field, sizeof(float));
    packetBuffer[2] = (floatInt >> 24) & 0xFF;
    packetBuffer[3] = (floatInt >> 16) & 0xFF;
    packetBuffer[4] = (floatInt >> 8) & 0xFF;
    packetBuffer[5] = floatInt & 0xFF;

    // Send data via UDP
    Udp.beginPacket(client_ip, localPort);
    Udp.write(packetBuffer, 6); // Sending 6 bytes, adjust if needed
    Udp.endPacket();

    delay(1000);
  }

  // Below is the tuple space code integrated with Arduino loop
  int nice_power;
  double temp, pi;
  field_t my_tuple[2]; /* an array of fields (name not included) */
  field_t my_template[1];

  /* make a tuple */
  my_tuple[0].is_actual = TS_YES;
  my_tuple[0].type = TS_INT;
  my_tuple[0].data.int_field = 128;
  my_tuple[1].is_actual = TS_YES;
  my_tuple[1].type = TS_FLOAT;
  my_tuple[1].data.float_field = 3.14;

  /* add a tuple to the tuple space */
  ts_out("nice_constants", my_tuple, 2); /* ("nice_constants",128,3.14) */
}
