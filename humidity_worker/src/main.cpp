#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_protocol.h>
#include <udp_setup.h>
#include <HumidityCheck.h>

#define FAN ZSUT_PIN_D2
#define MAX 4096
#define MAX_BUFFER 32
#define PACKET_BUFFER_LENGTH 32

char buffer[MAX_BUFFER];
unsigned char packetBuffer[PACKET_BUFFER_LENGTH];

FILE f_out;
int sput(char c, __attribute__((unused)) FILE *f) { return !Serial.write(c); }

unsigned long previousMillis = 0;
const long interval = 1000; // Interval in milliseconds (1 second)
const long interval_alarm = 10000;
unsigned long previous_alarm = 0;
bool alarm_on = false;

void setup()
{
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE);
  ZsutPinMode(FAN, OUTPUT);
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
}

void loop()
{
  unsigned long currentMillis = ZsutMillis();

  if (currentMillis - previous_alarm >= interval_alarm && alarm_on)
  {
    field_t tuple_result[2];
    tuple_result[0].is_actual = TS_NO;
    tuple_result[0].type = TS_FLOAT;
    tuple_result[1].is_actual = TS_NO;
    tuple_result[1].type = TS_INT;

    printf("UCIEKAJCIE\n");

    alarm_on = false;

    ts_inp("alarm_status", tuple_result, 2);
  }

  if (currentMillis - previousMillis >= interval)
  {
    field_t my_tuple[1]; /* an array of fields (name not included) */
    field_t alarm_check[2];

    /* make a tuple */
    my_tuple[0].is_actual = TS_NO;
    my_tuple[0].type = TS_FLOAT;
    my_tuple[0].data.int_field = NULL;

    alarm_check[0].is_actual = TS_NO;
    alarm_check[0].type = TS_FLOAT;
    alarm_check[0].data.float_field = NULL;
    alarm_check[1].is_actual = TS_NO;
    alarm_check[1].type = TS_INT;
    alarm_check[1].data.int_field = NULL;

    /* add a tuple to the tuple space */
    ts_rdp("check_alarm", alarm_check, 2);

    printf(" %d\n ", alarm_check[1].data.int_field);

    if (alarm_check[1].data.int_field == 1)
    {
      printf("Turning on the fan\n");
      ZsutDigitalWrite(FAN, HIGH);
    }
    else
    {
      printf("Turning off the fan\n");
      ZsutDigitalWrite(FAN, LOW);
    }

    uint32_t test = ZsutAnalog5Read();
    if (test != NULL)
    {
      printf("humidity is %d %%\n", test / 10);
      field_t tuple_result[2];
      tuple_result[0].is_actual = TS_YES;
      tuple_result[0].type = TS_FLOAT;
      tuple_result[0].data.float_field = test;
      tuple_result[1].is_actual = TS_YES;
      tuple_result[1].type = TS_INT;
      tuple_result[1].data.int_field = humidity(test);

      if (tuple_result[1].data.int_field == 0)
        printf(" All good\n");
      else if (tuple_result[1].data.int_field == 1 && !alarm_on)
      {
        ts_out("alarm_status", tuple_result, 2);
        printf("ALARM\n");
        alarm_on = true;
        previous_alarm = ZsutMillis();
      }

      // Serial.println("");

      previousMillis = currentMillis;
    }
  }
}