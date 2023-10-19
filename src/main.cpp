#include <Arduino.h>

#define MAXIMUM_NUM_NEOPIXELS 5
#include <NeoPixelConnect.h>

#define LED_PIN 22

#define S1_pin 2
#define S2_pin 3
#define S3_pin 4

NeoPixelConnect strip(LED_PIN, MAXIMUM_NUM_NEOPIXELS, pio0, 0);

typedef struct
{
  int state, new_state;
  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;

// Input variables
uint8_t S1, prevS1;
uint8_t S2, prevS2;
uint8_t S3, prevS3;

// Output variables
uint8_t LED_1, LED_2;

// Our finite state machines
fsm_t fsm1, fsm2, fsm3, fsm4, fsm5, fsm6, fsm7, fsm8, fsm9;

int i;

int LEDS[5];
int violet[] = {148, 0, 211};
int blue[] = {0, 0, 255};
int cyan[] = {0, 183, 235};
int green[] = {0, 255, 0};
int yellow[] = {255, 255, 0};
int orange[] = {255, 94, 5};
int white[] = {255, 255, 255};
int color[] = {0, 255, 0};
int current_state = 0;
int current_ligth[5] = {1, 1, 1, 1, 1};

unsigned long interval, last_cycle;
unsigned long loop_micros;
uint16_t blink_period;
uint16_t blink_period2; // added blink period for LED2 control

uint16_t total_time;
uint16_t blink_time;
uint16_t blink_1seconds;
uint16_t blink_2seconds;
uint16_t blink_5seconds;
uint16_t blink_10seconds;
uint16_t blink_500ms;

// Set new state
void set_state(fsm_t &fsm, int new_state)
{
  if (fsm.state != new_state)
  { // if the state chnanged tis is reset
    fsm.state = new_state;
    fsm.tes = millis();
    fsm.tis = 0;
  }
}

void setup()
{
  pinMode(S1_pin, INPUT_PULLUP);
  pinMode(S2_pin, INPUT_PULLUP);
  pinMode(S3_pin, INPUT_PULLUP);

  // Start the serial port with 115200 baudrate
  Serial.begin(115200);

  blink_period = 1000 * 1.0 / 0.33; // In ms

  blink_period2 = 120; // Set period to 1s
  blink_500ms = 500;
  blink_1seconds = 1000;
  blink_2seconds = 2000;
  blink_5seconds = 5000;
  blink_10seconds = 10000;
  blink_time = 2000;
  interval = 40;
  set_state(fsm1, 0);
  set_state(fsm2, 0);
  set_state(fsm3, 0);
  set_state(fsm4, 0);
  set_state(fsm5, 0);
  set_state(fsm6, 0);
  set_state(fsm7, 0);
  set_state(fsm8, 0);
  set_state(fsm9, 0);
  total_time = 0;
  i = 0;
}

void loop()
{
  uint8_t b;
  if (Serial.available())
  { // Only do this if there is serial data to be read
    b = Serial.read();
    if (b == '-')
      blink_period = 100 * blink_period / 80; // Press '-' to decrease the frequency
    if (b == '+')
      blink_period = 80 * blink_period / 100; // Press '+' to increase the frequency
  }
  // To measure the time between loop() calls
  // unsigned long last_loop_micros = loop_micros;

  // Do this only every "interval" miliseconds
  // It helps to clear the switches bounce effect
  unsigned long now = millis();
  if (now - last_cycle > interval)
  {
    loop_micros = micros();
    last_cycle = now;

    // Read the inputs
    prevS1 = S1;
    prevS2 = S2;

    S1 = !digitalRead(S1_pin);
    S2 = !digitalRead(S2_pin);
    S3 = !digitalRead(S3_pin);

    // FSM processing

    // Update tis for all state machines
    unsigned long cur_time = millis(); // Just one call to millis()
    fsm1.tis = cur_time - fsm1.tes;
    fsm2.tis = cur_time - fsm2.tes;
    fsm3.tis = cur_time - fsm3.tes;
    fsm4.tis = cur_time - fsm4.tes;
    fsm5.tis = cur_time - fsm5.tes;
    fsm6.tis = cur_time - fsm6.tes;
    fsm7.tis = cur_time - fsm7.tes;
    fsm8.tis = cur_time - fsm8.tes;
    fsm9.tis = cur_time - fsm9.tes;

    // Calculate next state for the first state machine
    if (fsm1.state == 0 && (S1 || S2 || S3) && fsm5.state == 0)
    {
      fsm1.new_state = 1;
    }
    else if (fsm1.state == 1 && fsm1.tis > blink_time && fsm3.state == 0)
    {
      fsm1.new_state = 2;
    }
    else if (fsm1.state == 2 && fsm1.tis > blink_time && fsm3.state == 0)
    {
      fsm1.new_state = 3;
    }
    else if (fsm1.state == 3 && fsm1.tis > blink_time && fsm3.state == 0)
    {
      fsm1.new_state = 4;
    }
    else if (fsm1.state == 4 && fsm1.tis > blink_time && fsm3.state == 0)
    {
      fsm1.new_state = 5;
    }
    else if (fsm1.state == 5 && fsm1.tis > blink_time && fsm3.state == 0)
    {
      fsm1.new_state = 6;
    }
    else if (fsm1.state == 6 && fsm1.tis > blink_time && fsm3.state == 0)
    {
      fsm1.new_state = 7;
    }
    else if (fsm1.state == 7 && fsm1.tis > blink_500ms && fsm3.state == 0)
    {
      fsm1.new_state = 8;
    }
    else if (fsm1.state == 8 && fsm1.tis > blink_500ms && fsm3.state == 0)
    {
      fsm1.new_state = 7;
    }

    if (fsm2.state == 0 && S1 && fsm3.state == 0 && fsm5.state != 2 && fsm5.state != 3 && fsm5.state == 0)
    {
      fsm1.new_state = 1;
      fsm2.new_state = 1;
    }
    else if (fsm2.state == 1 && !S1)
    {
      fsm2.new_state = 0;
    }

    if (fsm3.state == 0 && fsm1.state >> 0 && S3 && fsm5.state != 2 && fsm5.state != 3 && fsm5.state == 0)
    {
      int j = 0;
      current_state = fsm1.state;
      for (int i = 0; i < 5; i++)
      {

        current_ligth[i] = LEDS[i];
        if (j != 0)
        {
          current_ligth[i] = 1;
        }
        if (LEDS[i] == 1 && j == 0)
        {
          j++;
          current_ligth[i] = 0;
        }
      }
      fsm3.new_state = 1;
    }
    else if (fsm3.state == 1 && !S3 && fsm1.state != 0)
    {
      fsm3.new_state = 2;
    }
    else if (fsm3.state == 2 && fsm3.tis > blink_500ms && !S3)
    {
      fsm3.new_state = 3;
    }
    else if (fsm3.state == 3 && fsm3.tis > blink_500ms && !S3)
    {
      fsm3.new_state = 2;
    }
    else if (S3 && (fsm3.state == 2 || fsm3.state == 3))
    {
      fsm3.new_state = 4;
    }
    else if (fsm3.state == 4 && !S3)
    {
      fsm3.new_state = 0;
    }

    if (fsm4.state == 0 && S2 && LEDS[0] != 1 && fsm1.state >> 1 && fsm3.state == 0 && fsm5.state != 2 && fsm5.state != 3 && fsm5.state == 0)
    {
      fsm4.new_state = 1;
      fsm1.new_state = fsm1.state - 1;
    }
    if (fsm4.state == 1 && !S2)
    {
      fsm4.new_state = 0;
    }
    if (fsm5.state == 0 && S2 && fsm1.state > 2)
    {
      fsm5.new_state = 1;
    }
    else if (fsm5.state == 1 && fsm5.tis > 3000)
    {
      fsm5.new_state = 2;
      fsm1.state = 0;
      fsm2.state = 0;
      fsm3.state = 0;
      fsm4.state = 0;
    }
    else if (fsm5.state == 1 && !S2)
    {
      fsm5.new_state = 0;
    }

    else if (fsm5.state == 2 && fsm5.tis > 500)
    {
      fsm5.new_state = 3;
    }
    else if (fsm5.state == 3 && fsm5.tis > 500)
    {
      fsm5.new_state = 2;
    }
    else if (S2 && (fsm5.state == 2 || fsm5.state == 3))
    {
      fsm5.new_state = 4;
    }
    else if (fsm5.state == 4 && fsm5.tis > 3000)
    {
      fsm5.new_state = 5;
      fsm1.state = 0;
    }
    else if (fsm5.state == 5 && !S2)
    {
      fsm5.new_state = 0;
    }
    else if (fsm5.state == 4 && !S2)
    {
      fsm5.new_state = 6; // configuração 1
    }
    else if (fsm5.state == 6 && S2)
    {
      fsm5.new_state = 7;
    }
    else if (fsm5.state == 7 && S2 && fsm5.tis > 3000)
    {
      fsm5.new_state = 5;
      fsm6.new_state = 0; // guarda conf
    }
    else if (fsm5.state == 7 && !S2)
    {
      fsm5.new_state = 8;
      fsm6.new_state = 0; // configuração 2
    }
    else if (fsm5.state == 8 && S2)
    {
      fsm5.new_state = 9;
    }
    else if (fsm5.state == 9 && S2 && fsm5.tis > 3000)
    {
      fsm5.new_state = 5; // guarda conf
    }
    else if (fsm5.state == 9 && !S2)
    {
      fsm5.new_state = 10; // configuração 3
    }
    else if (fsm5.state == 10 && S2)
    {
      fsm5.new_state = 11;
    }
    else if (fsm5.state == 11 && S2 && fsm5.tis > 3000)
    {
      fsm5.new_state = 5; // guarda conf
    }
    else if (fsm5.state == 11 && !S2)
    {
      fsm5.new_state = 12;
    }
    else if (fsm5.state == 12 && S2)
    {
      fsm5.new_state = 4;
    }

    if (fsm6.state == 0 && (fsm5.state == 6))
    {
      fsm6.new_state = 1;
    }
    else if (fsm6.state == 1 && !S3 && fsm6.tis > blink_1seconds)
    {
      fsm6.new_state = 2;
      blink_time = blink_1seconds;
    }
    else if (fsm6.state == 2 && !S3 && fsm6.tis > blink_1seconds)
    {
      fsm6.new_state = 1;
      blink_time = blink_1seconds;
    }
    else if (S3 && (fsm6.state == 2 || fsm6.state == 1))
    {
      fsm6.new_state = 3;
    }
    else if (fsm6.state == 3 && !S3)
    {
      fsm6.new_state = 4;
    }
    else if (fsm6.state == 4 && !S3 && fsm6.tis > blink_2seconds)
    {
      fsm6.new_state = 5;
      blink_time = blink_2seconds;
    }
    else if (fsm6.state == 5 && !S3 && fsm6.tis > blink_2seconds)
    {
      fsm6.new_state = 4;
      blink_time = blink_2seconds;
    }
    else if (S3 && (fsm6.state == 4 || fsm6.state == 5))
    {
      fsm6.new_state = 6;
    }
    else if (!S3 && fsm6.state == 6)
    {
      fsm6.new_state = 7;
    }
    else if (fsm6.state == 7 && !S3 && fsm6.tis > blink_5seconds)
    {
      fsm6.new_state = 8;
      blink_time = blink_5seconds;
    }
    else if (fsm6.state == 8 && !S3 && fsm6.tis > blink_5seconds)
    {
      fsm6.new_state = 7;
      blink_time = blink_5seconds;
    }
    else if (S3 && (fsm6.state == 7 || fsm6.state == 8))
    {
      fsm6.new_state = 9;
    }
    else if (!S3 && fsm6.state == 9)
    {
      fsm6.new_state = 10;
    }
    else if (fsm6.state == 10 && !S3 && fsm6.tis > blink_10seconds)
    {
      fsm6.new_state = 11;
      blink_time = blink_10seconds;
    }
    else if (fsm6.state == 11 && !S3 && fsm6.tis > blink_10seconds)
    {
      fsm6.new_state = 10;
      blink_time = blink_10seconds;
    }
    else if (S3 && (fsm6.state == 10 || fsm6.state == 11))
    {
      fsm6.new_state = 12;
    }
    else if (fsm6.state == 12 && !S3)
    {
      fsm6.new_state = 1;
    }
    else if (fsm6.state > 0 && (fsm5.state == 5 || fsm5.state == 7) && fsm8.state > 0)
    {
      fsm6.new_state = 0;
    }
    if (fsm8.state == 0 && fsm5.state == 10)
    {
      fsm8.new_state = 1;
    }
    else if (fsm8.state == 1 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 2;
    }
    else if (fsm8.state == 2 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 1;
    }
    else if (S3 && (fsm8.state == 1 || fsm8.state == 2))
    {
      fsm8.new_state = 3;
    }
    else if (fsm8.state == 3 && !S3)
    {
      fsm8.new_state = 4;
    }
    else if (fsm8.state == 4 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 5;
    }
    else if (fsm8.state == 5 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 4;
    }
    else if (S3 && (fsm8.state == 4 || fsm8.state == 5))
    {
      fsm8.new_state = 6;
    }
    else if (fsm8.state == 6 && !S3)
    {
      fsm8.new_state = 7;
    }
    else if (fsm8.state == 7 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 8;
    }
    else if (fsm8.state == 8 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 7;
    }
    else if (S3 && (fsm8.state == 7 || fsm8.state == 8))
    {
      fsm8.new_state = 9;
    }
    else if (fsm8.state == 9 && !S3)
    {
      fsm8.new_state = 10;
    }
    else if (fsm8.state == 10 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 11;
    }
    else if (fsm8.state == 11 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 10;
    }
    else if (S3 && (fsm8.state == 10 || fsm8.state == 11))
    {
      fsm8.new_state = 12;
    }
    else if (fsm8.state == 12 && !S3)
    {
      fsm8.new_state = 13;
    }
    else if (fsm8.state == 13 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 14;
    }
    else if (fsm8.state == 14 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 13;
    }
    else if (S3 && (fsm8.state == 13 || fsm8.state == 14))
    {
      fsm8.new_state = 15;
    }
    else if (fsm8.state == 15 && !S3)
    {
      fsm8.new_state = 16;
    }
    else if (fsm8.state == 16 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 17;
    }
    else if (fsm8.state == 17 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 16;
    }
    else if (S3 && (fsm8.state == 16 || fsm8.state == 17))
    {
      fsm8.new_state = 18;
    }
    else if (fsm8.state == 18 && !S3)
    {
      fsm8.new_state = 19;
    }
    else if (fsm8.state == 19 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 20;
    }
    else if (fsm8.state == 20 && fsm8.tis > blink_1seconds && !S3)
    {
      fsm8.new_state = 19;
    }
    else if (S3 && (fsm8.state == 19 || fsm8.state == 20))
    {
      fsm8.new_state = 3;
    }
    else if (fsm6.state > 0 && fsm5.state == 5)
    {
      fsm8.new_state = 0;
    }
    else if (fsm8.state > 0 && (fsm5.state == 5 || fsm5.state == 9) && fsm8.state > 0)
    {
      fsm8.new_state = 0;
    }
    /// idle effect
    if (fsm1.state == 7 || fsm1.state == 8)
    {
      i++;
    }
    else
    {
      i = 0;
    }

    if (fsm9.state == 0 && ((i * interval) > 30000))
    {
      fsm9.new_state = 1;
      fsm1.new_state = 0;
    }
    else if (fsm9.state == 1 && fsm1.tis > 200)
    {
      fsm9.new_state = 2;
    }
    else if (fsm9.state == 2 && fsm1.tis > 200)
    {
      fsm9.new_state = 3;
    }
    else if (fsm9.state == 3 && fsm1.tis > 200)
    {
      fsm9.new_state = 4;
    }
    else if (fsm9.state == 4 && fsm1.tis > 200)
    {
      fsm9.new_state = 5;
    }
    else if (fsm9.state == 5 && fsm1.tis > 200)
    {
      fsm9.new_state = 6;
    }
    else if (fsm9.state == 6 && fsm1.tis > 200)
    {
      fsm9.new_state = 7;
    }
    else if (fsm9.state == 7 && fsm1.tis > 200)
    {
      fsm9.new_state = 8;
    }
    else if (fsm9.state == 8 && fsm1.tis > 200)
    {
      fsm9.new_state = 1;
    }
    else if (fsm9.state > 0 && (S1 || S2 || S3))
    {
      fsm9.new_state = 0;
    }

    // Update the states
    set_state(fsm1, fsm1.new_state);
    set_state(fsm2, fsm2.new_state);
    set_state(fsm3, fsm3.new_state);
    set_state(fsm4, fsm4.new_state);
    set_state(fsm5, fsm5.new_state);
    set_state(fsm6, fsm6.new_state);
    set_state(fsm7, fsm7.new_state);
    set_state(fsm8, fsm8.new_state);
    set_state(fsm9, fsm9.new_state);

    // Actions set by the current state of the first state machine
    if (fsm1.state == 0)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
    }
    else if (fsm1.state == 1)
    {
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
      LEDS[0] = 1, LEDS[1] = 1, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 2)
    {
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 3)
    {
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 4)
    {
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 5)
    {
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
    }
    else if (fsm1.state == 6)
    {
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
    }
    else if (fsm1.state == 7)
    {
      strip.neoPixelSetValue(4, 255, 0, 0);
      strip.neoPixelSetValue(3, 255, 0, 0);
      strip.neoPixelSetValue(2, 255, 0, 0);
      strip.neoPixelSetValue(1, 255, 0, 0);
      strip.neoPixelSetValue(0, 255, 0, 0);
      LEDS[0] = 1, LEDS[1] = 1, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 8)
    {
      strip.neoPixelSetValue(4, 0, 0, 0);
      strip.neoPixelSetValue(3, 0, 0, 0);
      strip.neoPixelSetValue(2, 0, 0, 0);
      strip.neoPixelSetValue(1, 0, 0, 0);
      strip.neoPixelSetValue(0, 0, 0, 0);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
    }

    if (fsm3.state == 2)
    {
      strip.neoPixelSetValue(4, 0 * current_ligth[0], 255 * current_ligth[0], 0 * current_ligth[0]);
      strip.neoPixelSetValue(3, 0 * current_ligth[1], 255 * current_ligth[1], 0 * current_ligth[1]);
      strip.neoPixelSetValue(2, 0 * current_ligth[2], 255 * current_ligth[2], 0 * current_ligth[2]);
      strip.neoPixelSetValue(1, 0 * current_ligth[3], 255 * current_ligth[3], 0 * current_ligth[3]);
      strip.neoPixelSetValue(0, 0 * current_ligth[4], 255 * current_ligth[4], 0 * current_ligth[4]);
      LEDS[0] = current_ligth[0], LEDS[1] = current_ligth[1], LEDS[2] = current_ligth[2], LEDS[3] = current_ligth[3], LEDS[4] = current_ligth[4];
    }
    else if (fsm3.state == 3)
    {
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 0 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 0 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 0 * LEDS[4]);
    }
    if (fsm5.state == 0)
    {
      while (S2)
      {
      }
    }

    else if (fsm5.state == 2)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 3)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 3)
    {
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 255 * LEDS[1], 0 * LEDS[1], 0 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 4)
    {
      while (S2)
      {
      }
    }
    else if (fsm5.state == 5)
    {
      LEDS[0] = 1, LEDS[1] = 1, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 6)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 7)
    {
      while (S2)
      {
      }
    }
    else if (fsm5.state == 8)
    {
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 9)
    {
      while (S2)
      {
      }
    }
    else if (fsm5.state == 10)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm5.state == 11)
    {
      while (S2)
      {
      }
    }
    else if (fsm5.state == 12)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 1, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 0 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    if (fsm6.state == 1)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 2)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 3)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 4)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 5)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 6)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 7)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 8)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 9)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 10)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 11)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 12)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 0 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 0 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 0 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 0 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 0 * LEDS[4], 255 * LEDS[4]);
    }
    if (fsm8.state == 1)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = violet[0], color[1] = violet[1], color[2] = violet[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 2)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = violet[0], color[1] = violet[1], color[2] = violet[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 3)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = blue[0], color[1] = blue[1], color[2] = blue[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 4)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = blue[0], color[1] = blue[1], color[2] = blue[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 5)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = blue[0], color[1] = blue[1], color[2] = blue[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 6)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = cyan[0], color[1] = cyan[1], color[2] = cyan[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 7)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = cyan[0], color[1] = cyan[1], color[2] = cyan[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 8)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = cyan[0], color[1] = cyan[1], color[2] = cyan[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 9)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = green[0], color[1] = green[1], color[2] = green[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 10)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = green[0], color[1] = green[1], color[2] = green[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 11)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = green[0], color[1] = green[1], color[2] = green[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 12)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = yellow[0], color[1] = yellow[1], color[2] = yellow[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 13)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = yellow[0], color[1] = yellow[1], color[2] = yellow[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 14)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = yellow[0], color[1] = yellow[1], color[2] = yellow[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 15)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = orange[0], color[1] = orange[1], color[2] = orange[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 16)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = orange[0], color[1] = orange[1], color[2] = orange[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 17)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = orange[0], color[1] = orange[1], color[2] = orange[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 18)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = white[0], color[1] = white[1], color[2] = white[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 19)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = white[0], color[1] = white[1], color[2] = white[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    else if (fsm8.state == 20)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      color[0] = white[0], color[1] = white[1], color[2] = white[2];
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 0 * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 1)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 2)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 1, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 3)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 4)
    {
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 5)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 6)
    {
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 7)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 8)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 1, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }
    if (fsm9.new_state == 6)
    {
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
      strip.neoPixelSetValue(4, color[0] * LEDS[0], color[1] * LEDS[0], color[2] * LEDS[0]);
      strip.neoPixelSetValue(3, color[0] * LEDS[1], color[1] * LEDS[1], color[2] * LEDS[1]);
      strip.neoPixelSetValue(2, color[0] * LEDS[2], color[1] * LEDS[2], color[2] * LEDS[2]);
      strip.neoPixelSetValue(1, color[0] * LEDS[3], color[1] * LEDS[3], color[2] * LEDS[3]);
      strip.neoPixelSetValue(0, color[0] * LEDS[4], color[1] * LEDS[4], color[2] * LEDS[4]);
    }

    // Set the outputs
    strip.neoPixelShow();

    // Debug using the serial port
    Serial.print("S1: ");
    Serial.print(S1);

    Serial.print(" S2: ");
    Serial.print(S2);

    Serial.print(" S3: ");
    Serial.print(S3);

    Serial.print(" LED 1: ");
    Serial.print(LEDS[0]);

    Serial.print(" LED 2: ");
    Serial.print(LEDS[1]);

    Serial.print(" LED 3: ");
    Serial.print(LEDS[2]);

    Serial.print(" LED 4: ");
    Serial.print(LEDS[3]);

    Serial.print(" LED 5: ");
    Serial.print(LEDS[4]);

    Serial.print(" fsm1: ");
    Serial.print(fsm1.state);

    Serial.print(" fsm2: ");
    Serial.print(fsm2.state);

    Serial.print(" fsm3: ");
    Serial.print(fsm3.state);

    Serial.print(" fsm4: ");
    Serial.print(fsm4.state);

    Serial.print(" fsm5: ");
    Serial.print(fsm5.state);

    Serial.print(" fsm6: ");
    Serial.print(fsm6.state);

    Serial.print(" fsm8: ");
    Serial.print(fsm8.state);

    Serial.print(" fsm9: ");
    Serial.print(fsm9.state);

    Serial.print(" i ");
    Serial.print(i);

    Serial.print(" blink: ");
    Serial.print(blink_time);

    Serial.print(" loop: ");
    Serial.print(micros() - loop_micros);
    Serial.println();
  }
}
