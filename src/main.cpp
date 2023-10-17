#include <Arduino.h>

#define MAXIMUM_NUM_NEOPIXELS 5
#include <NeoPixelConnect.h>

#define LED_PIN 22

#define S1_pin 2
#define S2_pin 3
#define S3_pin 4

NeoPixelConnect strip(LED_PIN, MAXIMUM_NUM_NEOPIXELS, pio0, 0);

// testar a as modificações do git
int testegit;

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
fsm_t fsm1, fsm2, fsm3, fsm4, fsm5, fsm6;

int LEDS[5];
int current_state = 0;
int current_ligth[5] = {1, 1, 1, 1, 1};

unsigned long interval, last_cycle;
unsigned long loop_micros;
uint16_t blink_period;
uint16_t blink_period2; // added blink period for LED2 control

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
  interval = 40;
  set_state(fsm1, 0);
  set_state(fsm2, 0);
  set_state(fsm3, 0);
  set_state(fsm4, 0);
  set_state(fsm5, 0);
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

    // Calculate next state for the first state machine
    if (fsm1.state == 0 && (S1 || S2 || S3) && fsm5.state == 0)
    {
      fsm1.new_state = 1;
    }
    else if (fsm1.state == 1 && fsm1.tis > blink_2seconds && fsm3.state == 0)
    {
      fsm1.new_state = 2;
    }
    else if (fsm1.state == 2 && fsm1.tis > blink_2seconds && fsm3.state == 0)
    {
      fsm1.new_state = 3;
    }
    else if (fsm1.state == 3 && fsm1.tis > blink_2seconds && fsm3.state == 0)
    {
      fsm1.new_state = 4;
    }
    else if (fsm1.state == 4 && fsm1.tis > blink_2seconds && fsm3.state == 0)
    {
      fsm1.new_state = 5;
    }
    else if (fsm1.state == 5 && fsm1.tis > blink_2seconds && fsm3.state == 0)
    {
      fsm1.new_state = 6;
    }
    else if (fsm1.state == 6 && fsm1.tis > blink_period2 && fsm3.state == 0)
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
      /* code */
    }
    if (fsm6.state == 0 && (fsm5.state == 2 || fsm5.state == 3))
    {
      fsm6.new_state = 1;
    }
    else if (fsm6.state == 1 && !S3 & fsm6.tis > blink_1seconds)
    {
      fsm6.new_state = 0;
    }
    else if (S3 && fsm6.state == 0 || fsm6.state == 1)
    {
      fsm6.new_state = 2;
    }

    else if (fsm6.state == 2 && !S3 & fsm6.tis > blink_2seconds)
    {
      fsm6.new_state = 3;
    }
    else if (fsm6.state == 3 && !S3 & fsm6.tis > blink_2seconds)
    {
      fsm6.new_state = 2;
    }
    else if (S3 && (fsm6.state == 2 || fsm6.state == 3))
    {
      fsm6.new_state = 4;
    }
    else if (fsm6.state == 4 && !S3 & fsm6.tis > blink_5seconds)
    {
      fsm6.new_state = 5;
    }
    else if (fsm6.state == 5 && !S3 & fsm6.tis > blink_5seconds)
    {
      fsm6.new_state = 4;
    }
    else if (S3 && (fsm6.state == 4 || fsm6.state == 5))
    {
      fsm6.new_state = 6;
    }

    else if (fsm6.state == 6 && !S3 & fsm6.tis > blink_10seconds)
    {
      fsm6.new_state = 7;
    }
    else if (fsm6.state == 7 && !S3 & fsm6.tis > blink_10seconds)
    {
      fsm6.new_state = 6;
    }
    else if (S3 && (fsm6.state == 6 || fsm6.state == 7))
    {
      fsm6.new_state = 0;
    }
    else if ((fsm6.state == 0 || fsm6.state == 1 || fsm6.state == 2 || fsm6.state == 3 || fsm6.state == 4 || fsm6.state == 5 || fsm6.state == 6 || fsm6.state == 7 || fsm6.state == 8) && S2)
    {
      fsm6.new_state = 10;
    }
    else if (fsm6.state == 10 && fsm6.tis > 3000)
    {
      fsm5.state = 0;
      fsm6.state = 0;
    }
    // Update the states
    set_state(fsm1, fsm1.new_state);
    set_state(fsm2, fsm2.new_state);
    set_state(fsm3, fsm3.new_state);
    set_state(fsm4, fsm4.new_state);
    set_state(fsm5, fsm5.new_state);
    set_state(fsm6, fsm5.new_state);

    // Actions set by the current state of the first state machine
    if (fsm1.state == 0)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 0;
    }
    else if (fsm1.state == 1)
    {
      strip.neoPixelSetValue(4, 0, 255, 0);
      strip.neoPixelSetValue(3, 0, 255, 0);
      strip.neoPixelSetValue(2, 0, 255, 0);
      strip.neoPixelSetValue(1, 0, 255, 0);
      strip.neoPixelSetValue(0, 0, 255, 0);
      LEDS[0] = 1, LEDS[1] = 1, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 2)
    {
      strip.neoPixelSetValue(4, 0, 0, 0);
      strip.neoPixelSetValue(3, 0, 255, 0);
      strip.neoPixelSetValue(2, 0, 255, 0);
      strip.neoPixelSetValue(1, 0, 255, 0);
      strip.neoPixelSetValue(0, 0, 255, 0);
      LEDS[0] = 0, LEDS[1] = 1, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 3)
    {
      strip.neoPixelSetValue(4, 0, 0, 0);
      strip.neoPixelSetValue(3, 0, 0, 0);
      strip.neoPixelSetValue(2, 0, 255, 0);
      strip.neoPixelSetValue(1, 0, 255, 0);
      strip.neoPixelSetValue(0, 0, 255, 0);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 4)
    {
      strip.neoPixelSetValue(4, 0, 0, 0);
      strip.neoPixelSetValue(3, 0, 0, 0);
      strip.neoPixelSetValue(2, 0, 0, 0);
      strip.neoPixelSetValue(1, 0, 255, 0);
      strip.neoPixelSetValue(0, 0, 255, 0);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 1, LEDS[4] = 1;
    }
    else if (fsm1.state == 5)
    {
      strip.neoPixelSetValue(4, 0, 0, 0);
      strip.neoPixelSetValue(3, 0, 0, 0);
      strip.neoPixelSetValue(2, 0, 0, 0);
      strip.neoPixelSetValue(1, 0, 0, 0);
      strip.neoPixelSetValue(0, 0, 255, 0);
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
    }
    else if (fsm1.state == 6)
    {
      strip.neoPixelSetValue(4, 0, 0, 0);
      strip.neoPixelSetValue(3, 0, 0, 0);
      strip.neoPixelSetValue(2, 0, 0, 0);
      strip.neoPixelSetValue(1, 0, 0, 0);
      strip.neoPixelSetValue(0, 0, 0, 0);
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
    if (fsm6.state == 0)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 1)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 2)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 3)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 4)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 5)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 6)
    {
      LEDS[0] = 1, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 7)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 0, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
    }
    else if (fsm6.state == 8)
    {
      LEDS[0] = 0, LEDS[1] = 0, LEDS[2] = 1, LEDS[3] = 0, LEDS[4] = 1;
      strip.neoPixelSetValue(4, 0 * LEDS[0], 255 * LEDS[0], 255 * LEDS[0]);
      strip.neoPixelSetValue(3, 0 * LEDS[1], 255 * LEDS[1], 255 * LEDS[1]);
      strip.neoPixelSetValue(2, 0 * LEDS[2], 255 * LEDS[2], 255 * LEDS[2]);
      strip.neoPixelSetValue(1, 0 * LEDS[3], 255 * LEDS[3], 255 * LEDS[3]);
      strip.neoPixelSetValue(0, 0 * LEDS[4], 255 * LEDS[4], 255 * LEDS[4]);
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

    Serial.print(" blink: ");
    Serial.print(fsm5.tis);

    Serial.print(" loop: ");
    Serial.print(micros() - loop_micros);
    Serial.println();
  }
}
