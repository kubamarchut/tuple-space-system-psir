#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_space.h>
#include <udp_setup.h>

#define MIN 2
#define MAX 32
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
const long interval_1 = 750; // Interval in milliseconds (750 ms)
const long interval_2 = 250; // Interval in milliseconds (250 ms)

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

uint32_t i = MIN;
uint32_t checked_ints = 0;
bool finished = false;

void loop()
{
  unsigned long currentMillis = ZsutMillis();
  if (currentMillis - previousMillis_2 >= interval_2 && checked_ints < MAX-1) {
    field_t tuple_result[1];
    tuple_result[0].is_actual = TS_NO;
    tuple_result[0].type = TS_INT;
    unsigned char* tuple_name = "is_prime";
    int in_result = ts_inp(tuple_name, tuple_result, 1);
    if (in_result == TS_FAILURE){
        printf("an error encourted\n");
    }
    else if (in_result == TS_NO_TUPLE){
        //printf("no tuple matched your tuple template\n");
    }
    else if (in_result == TS_SUCCESS){
      checked_ints++;
      uint32_t test =  tuple_result[0].data.int_field;

      printf("received result for int %ld is prime\n", test);
    }
    tuple_name = "is_not_prime";
    in_result = ts_inp(tuple_name, tuple_result, 1);
    if (in_result == TS_FAILURE){
        printf("an error encourted\n");
    }
    else if (in_result == TS_NO_TUPLE){
        //printf("no tuple matched your tuple template\n");
    }
    else if (in_result == TS_SUCCESS){
      checked_ints++;
      uint32_t test =  tuple_result[0].data.int_field;

      printf("received result for int %ld is not a prime\n", test);
    }
    previousMillis_2 = currentMillis;
  }

  if (currentMillis - previousMillis_1 >= interval_1 && !finished) {
    //Serial.println("some time has passed sending new tuple");
    field_t my_tuple[1];    /* an array of fields (name not included) */

    /* make a tuple */
    my_tuple[0].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = i++;

    //Serial.println("sending new tuple: ");
        /* add a tuple to the tuple space */
    int out_result = ts_out("check_if_prime", my_tuple, 1);
    if (out_result == TS_FAILURE){
      printf("an error encourted\n");
    }
    else if (out_result == TS_SUCCESS){
      //printf("new task sent\n");
    }
    

    if (i > MAX){finished = true;}
    previousMillis_1 = currentMillis;
  }

}


  