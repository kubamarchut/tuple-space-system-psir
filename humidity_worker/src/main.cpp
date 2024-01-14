#include <ZsutDhcp.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <tuple_space.h>
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
bool fan_on = false;

void setup()
{
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE);
  ZsutPinMode(FAN, OUTPUT);
  ZsutDigitalWrite(FAN, HIGH);
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
    long test = ZsutAnalog5Read();
    if (test != NULL)
    {
      float humidity_read = mapfloat(test, 0, 1023, 0, 100);
      if (humidity(humidity_read, alarm_on) == 0){
        field_t tuple_result[2];
        tuple_result[0].is_actual = TS_NO;
        tuple_result[0].type = TS_FLOAT;
        tuple_result[1].is_actual = TS_NO;
        tuple_result[1].type = TS_INT;

        printf("Alarm off\n");

        alarm_on = false;

        int in_result = ts_inp("alarm_status", tuple_result, 2);
        if (in_result == TS_FAILURE){
            printf("an error encourted\n");
        }
        else if (in_result == TS_NO_TUPLE){
            printf("somebody already removed alarm\n");
        }
        else if (in_result == TS_SUCCESS){
          //no idea
        }
      }
      else{
        previous_alarm = ZsutMillis();
      }
    }
  }

  if (currentMillis - previousMillis >= interval)
  {
    field_t alarm_check[2];

    alarm_check[0].is_actual = TS_NO;
    alarm_check[0].type = TS_FLOAT;
    alarm_check[1].is_actual = TS_NO;
    alarm_check[1].type = TS_INT;

    /* add a tuple to the tuple space */
    int rd_result = ts_rdp("alarm_status", alarm_check, 2);
    if (rd_result == TS_FAILURE){
      printf("an error encourted\n");
    }
    else if (rd_result == TS_NO_TUPLE){
      if (fan_on){
        printf("Turning off the fan\n");
        ZsutDigitalWrite(FAN, HIGH);
        fan_on = false;
      }
    }
    else if (rd_result == TS_SUCCESS){
      if(!fan_on){
        printf("Turning on the fan\n");
        ZsutDigitalWrite(FAN, LOW);
        fan_on = true;
      }
    }

    long test = ZsutAnalog5Read();
    if (test != NULL)
    {
      float humidity_read = mapfloat(test, 0, 1023, 0, 100);
      printf("humidity is ");
      Serial.print(humidity_read);
      printf("%%\n");
      field_t tuple_result[2];
      tuple_result[0].is_actual = TS_YES;
      tuple_result[0].type = TS_FLOAT;
      tuple_result[0].data.float_field = humidity_read;
      tuple_result[1].is_actual = TS_YES;
      tuple_result[1].type = TS_INT;
      tuple_result[1].data.int_field = humidity(humidity_read, alarm_on);

      //if (tuple_result[1].data.int_field == 0)
        //printf(" All good\n");
      if (humidity(humidity_read, alarm_on) == 1 && !alarm_on)
      {
        int out_result = ts_out("alarm_status", tuple_result, 2);
        if (out_result == TS_FAILURE){
          printf("an error encourted\n");
        }
        else if (out_result == TS_SUCCESS){
          printf("ALARM sent\n");
        }
    
        alarm_on = true;
        previous_alarm = ZsutMillis();
      }

      previousMillis = currentMillis;
    }
  }
}