#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_space.h>
#include <udp_setup.h>
#include <PrimeCheck.h>

#define MAX 4096
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

unsigned long previousMillis = 0;
const long interval = 1000; // Interval in milliseconds (1 second)

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

  //ZsutEthernet.begin(MAC);
  
  randomSeed(ZsutMillis());
  setupUDP();

  Serial.print(F("My IP address: "));
  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    Serial.print(ZsutEthernet.localIP()[thisByte], DEC);
    Serial.print(F("."));
  }
  Serial.println();

  //Udp.begin(5000);

  //ZsutIPAddress client_ip = ZsutIPAddress(192, 168, 89, 11);
  /*Udp.beginPacket(client_ip, localPort);
  packetBuffer[0] = HELLO_MSG;
  Udp.write(packetBuffer, 1);
  Udp.endPacket();*/
}

uint32_t i = 2;

void loop()
{
  unsigned long currentMillis = ZsutMillis();
  if (currentMillis - previousMillis >= interval) {
    field_t my_tuple[1];    /* an array of fields (name not included) */

    /* make a tuple */
    my_tuple[0].is_actual = TS_NO;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = NULL;

    /* add a tuple to the tuple space */
    ts_inp("check_prime", my_tuple, 1);

    uint32_t test =  my_tuple[0].data.int_field;
    if (test != NULL){
      printf("checking int %d", test);
      field_t tuple_result[2];
      tuple_result[0].is_actual = TS_YES;
      tuple_result[0].type = TS_INT;
      tuple_result[0].data.int_field = test;
      tuple_result[1].is_actual = TS_YES;
      tuple_result[1].type = TS_INT;
      tuple_result[1].data.int_field = isPrime(test);
      ts_out("check_prime_result", tuple_result, 2);

      if (isPrime(test) == 1)
        printf(" it is a prime\n");
      else
        printf(" it is not a prime\n");
    }
    else {
      printf("nothing to check\n");
    }


    if (i >= MAX){i = 0;}
    previousMillis = currentMillis;
  }

}