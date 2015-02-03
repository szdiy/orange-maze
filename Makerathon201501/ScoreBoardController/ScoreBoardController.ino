#include "Makeblock.h"
#include <SoftwareSerial.h>
#include <Wire.h>

#define KEY_START 1
#define KEY_NULL 0

#define LED_START 1
#define LED_NULL 0

#define GAME_FAILED     2
#define GAME_WIN        1
#define GAME_NULL       0

#define DEFAULT_START_TIMER 30
#define DEFAULT_RUN_START_TIMER 10
long lastTime = 0;
long runlastTime = 0;
int keystart = KEY_NULL;
int win_flag = GAME_NULL;
int total_time = DEFAULT_START_TIMER;
int count_timer = total_time;
int bri = 0, st = 0;
int start_run_led = LED_NULL;
int start_run_timer = DEFAULT_RUN_START_TIMER;
int sta = KEY_NULL;
int suc_val; 

//red, green, blue,
int color_tab[30][3];

const int run_color_tab[30][3]=
{{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},
 {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
 {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
 
const int failed_color_tab[30][3]=
{{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
 {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
 {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

class SZDIY4Button: public Me4Button {
  public:
    uint8_t key;
    SZDIY4Button(uint8_t port):Me4Button(port) {
    }
    
    void initRead() {
      key = this->pressed();
    }
    
    void read4Button() {
      uint8_t newKey = this->pressed();
      if (newKey != key) { // on key changed
      
        switch(newKey)//Check which button pressed
        {
        case KEY1:
          keystart = KEY_START;
          Serial.println("KEY1 pressed");
          break;
        case KEY2:
          #if 0
          if(keystart != KEY_START)
          {
            total_time = total_time + 10;
          }
          if(total_time > 300)
          {
            total_time = 300;
          }
          #endif
          count_timer = total_time;
          Serial.println("KEY2 pressed");
          break;
        case KEY3:
          Serial.println("KEY3 pressed");
          reset_all();
          break;
        case KEY4:
          #if 0 
          if(keystart != KEY_START)
          {
            total_time = total_time - 10;
            if(total_time < 0)
            {
              total_time = 0;
            }
          }
          #endif
          count_timer = total_time;
          Serial.println("KEY4 pressed");
          break;
        }
        key = newKey;
      }
    }
};

Me7SegmentDisplay disp(PORT_6);  //display
SZDIY4Button btn(PORT_8);        //keyboard
MeRGBLed led(PORT_3);
MePort sucout(PORT_3);

void setup()
{
  btn.initRead();
  led.setNumber(30);
  disp.display(0000);
  Serial.begin(9600);
}
void TimingISR()
{
  if((keystart == KEY_START) && (win_flag == GAME_NULL))
  {
    count_timer = count_timer - 1;
  }
  if((start_run_led == LED_START) || (win_flag != GAME_NULL))
  {
    start_run_timer = start_run_timer - 1;
  }
  if(count_timer == 0)
  {
    keystart = KEY_NULL;
    start_run_led = LED_START;
    win_flag = GAME_FAILED;
  } 
  if(start_run_timer == 0)
  {
    reset_all();
  }
}

void reset_all()
{
  keystart = KEY_NULL;
  start_run_led = LED_NULL;
  count_timer = DEFAULT_START_TIMER;
  start_run_timer = DEFAULT_RUN_START_TIMER;
  win_flag = GAME_NULL;
}
void displaytime()
{
  if(millis()-lastTime>=1000)
  {
    TimingISR();
    Serial.println(keystart,DEC);
    Serial.println((int)count_timer,DEC);
    disp.display(count_timer);
    if(win_flag == GAME_NULL)
    {
      rgb_display_time(count_timer);
    }
    lastTime = millis();
  }
}

void breathlight()
{
  if(bri>=100) st=1;
  if(bri<=0) st=0;
  
  if(st)bri--;
  else bri++;
  for(int t=0;t<30;t++){
    led.setColorAt(t, bri, bri, bri); // parameter description: led number, red, green, blue, flash mode
  }
  led.show();
  delay(10);
}

int k;
void runlight(int win_flag)
{
  if(start_run_led != LED_START)
  {
    return;
  }
  if(millis() - runlastTime >= 150)
  {
    Serial.print("runlight show\n");
    int i,j,n;
    Serial.print("runlight show\n");
    for(i=0;i<30;i++)
    {
      for(j=0;j<3;j++)
      {
        n = i+k;
        if((i+k) >= 30)
        {
          n = (i+k)%30;
        }
        if(win_flag == GAME_WIN)
        {
          color_tab[i][j] = run_color_tab[n][j];
        }
        else
        {
          color_tab[i][j] = failed_color_tab[n][j]; 
        }
      }
    }
    for(i=0;i<30;i++)
    {
      led.setColorAt(i, color_tab[i][0], color_tab[i][1], color_tab[i][2]);   
    }
    led.show(); 
    k++;
    runlastTime = millis();
  }
}

void rgb_display_time(int count_timer)
{
  for(int i=0;i<30;i++)
  {
    if(i < count_timer)
    {
      led.setColorAt(i, 0, 0, 255); 
    }
    else
    {
      led.setColorAt(i, 0, 0, 0);
    }
  }
  led.show();  
}  

void loop()
{
  btn.read4Button();
  displaytime();
  if((keystart == KEY_NULL) && (start_run_led == LED_NULL))
  {
    breathlight();
  }
  if((start_run_led == LED_START) && (win_flag != GAME_NULL))
  {
    runlight(win_flag);
  }
  suc_val = sucout.dRead1();
  if((suc_val == 0) && (keystart == KEY_START))
  {
    start_run_led = LED_START;
    win_flag = GAME_WIN;
  }
  Serial.println(suc_val);
}

