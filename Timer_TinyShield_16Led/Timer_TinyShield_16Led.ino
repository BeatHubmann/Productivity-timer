/*
 * Productivity timer using 
 * TinyDuino Edge 16 LED
 * 
 * 2017 by Beat Hubmann
 * 
 */

unsigned long session=          25 * 60 * 1e3;
unsigned long short_break=       5 * 60 * 1e3;
unsigned long long_break=       20 * 60 * 1e3;
unsigned long long_long_break=  60 * 60 * 1e3;
unsigned int session_count= 0;
unsigned int all= 16;

void setup()
{
  led_on(0); // Set all LEDs to off
  // Serial.begin(9600); // Debug output  
}

void loop()
{
  boot();
  block(3);
  relax(long_break);
  block(3);
  relax(long_long_break);
  block(3);
  relax(long_break);
  block(3);
  relax(long_break);
  block(2);
  fireworks();
  sleep();
}

void work(unsigned long ms)
{
  unsigned long segment= ms / all;
  for (int i= 1; i < all+1; ++i)
  {
    full(i, segment); 
  }
  ++session_count;
  flash(session_count, 10);  
}

void relax(unsigned long ms)
{
  unsigned long segment= ms / all;
  for (int i= all; i > 0; --i)
  {
    full(i, segment); 
  }
  flash(all, 10);
}

void block(unsigned int n)
{
  for (int i=0; i < n-1; ++i)
  {
    work(session);
    relax(short_break);
  }
  work(session);
}

void fireworks()
{
  int decay;
  for (int i= 0; i < 20; ++i)
  {
    decay= random(20, 200);
    for (int j=8; j > 0; --j)
    {
      int k= all + 1 - j;
      for (int l= 0; l < decay; ++l)
      {
        led_on(j);
        delay(1);
        led_on(k);
        delay(1);
      }
      led_on(0);
      decay/= 2;
    }
    delay(random(50, 300));
  }
}

void boot()
{
  for (int i=1; i < 17; ++i)
  {
    led_on(i);
    delay(100);
  }
  for (int i=16; i > 0; --i)
  {
    led_on(i);
    delay(100);
  }
}

void flash(int number, int sec)
{
  for (int i= 0; i < sec*2; ++i)
  {
    full(number, 250);
    led_on(0);
    delay(250);
  }
}

void full(int number, unsigned long ms)
{
  unsigned long target_time= millis() + ms;
  while (millis() < target_time)
  { 
    for (int i= 1; i < number+1; i++)
    {
      led_on(i);
      delay(1);
    }
  }
}

void led_on(int ledNum)
{
  // Set pins 5 to 9 to INPUT, LOW to switch off all LEDs:
  for(int i=5;i<10;i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
  // Check if valid ledNum in argument, return if not
  if(ledNum<1 || ledNum>16)
    return;
  // Charlieplexing pin assignment for LEDs:  
  char highpin[16]={5,6,5,7,6,7,6,8,5,8,8,7,9,7,9,8};
  char lowpin[16]= {6,5,7,5,7,6,8,6,8,5,7,8,7,9,8,9};
  // Correct for 1-numbering of LEDs:
  ledNum--;
  //Set pins corresponding to ledNum to HIGH and LOW respectively:
  digitalWrite(highpin[ledNum],HIGH);
  digitalWrite(lowpin[ledNum],LOW);
  //Set mode of pins corresponding to ledNum to OUTPUT:
  pinMode(highpin[ledNum],OUTPUT);
  pinMode(lowpin[ledNum],OUTPUT);
}

void sleep()
{
  led_on(0);
  // Serial.end();
  exit(0);
}

