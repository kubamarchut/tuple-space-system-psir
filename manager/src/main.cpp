#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_space.h>
#include <udp_setup.h>

#define MAX 8042
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
const long interval_1 = 500; // Interval in milliseconds (1 second)
const long interval_2 = 100; // Interval in milliseconds (1 second)

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

uint32_t i = MAX;

void loop()
{
  unsigned long currentMillis = ZsutMillis();
  if (currentMillis - previousMillis_2 >= interval_2) {
    field_t tuple_result[2];
    tuple_result[0].is_actual = TS_NO;
    tuple_result[0].type = TS_INT;
    tuple_result[0].data.int_field = NULL;
    tuple_result[1].is_actual = TS_NO;
    tuple_result[1].type = TS_INT;
    tuple_result[1].data.int_field = NULL;
    const char* tuple_name = "check_prime_result";
    ts_inp(tuple_name, tuple_result, 2);

    uint32_t test =  tuple_result[0].data.int_field;
    if (test != NULL){
      printf("received result for int %ld", test);
      if (tuple_result[1].data.int_field == 1){
        printf(" is prime\n");
      }
      else{
        printf(" is not prime\n");
      }
    }
    previousMillis_2 = currentMillis;
  }
  if (currentMillis - previousMillis_1 >= interval_1) {
    //Serial.println("some time has passed sending new tuple");
    field_t my_tuple[1];    /* an array of fields (name not included) */

    /* make a tuple */
    my_tuple[0].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = i--;

    //Serial.println("sending new tuple: ");
        /* add a tuple to the tuple space */
    ts_out("check_prime", my_tuple, 1);

    /* make a tuple */
    my_tuple[0].is_actual = TS_NO;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = NULL;
    
    //delay(1000);
    /* make a tuple */
    //my_tuple[0].is_actual = TS_NO;
    //my_tuple[0].type = TS_INT;
    //my_tuple[0].data.int_field = i++;

    //Serial.print("sending tuple request: ");
    /* add a tuple to the tuple space */
    //ts_inp("check_prime", my_tuple, 1);
    //Serial.println("\n---------------------------------");
    //Serial.println("");
    

    if (i <= 2){i = MAX;}
    previousMillis_1 = currentMillis;
  }

}


  