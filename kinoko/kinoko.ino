#include <WS2812.h>

#define MAX_HUE 767
#define MIN_HUE 0
#define STEPS_DELAY 80

#define LEDS_PIN 0
#define BTN_PIN 1

WS2812 LED(5); // 5 LEDs
uint16_t init_hue[5]; 
uint16_t end_hue[5];
uint8_t rgb_converted[3];
uint16_t btn_timer = 0;
uint8_t saturation = 255;
uint8_t brightness = 200;
int8_t adding = 1;
uint8_t buttonState = 0; 
cRGB value;
cRGB black;

void setup() {
  pinMode(BTN_PIN, INPUT);
  
	LED.setOutput(LEDS_PIN);
  for(int i=0;i<5;i++){
    init_hue[i] = random(MIN_HUE,MAX_HUE);
    end_hue[i] = random(MIN_HUE,MAX_HUE);
  }
}

void loop() {
  digitalWrite(2, HIGH);
  buttonState = digitalRead(BTN_PIN);
  if (buttonState == HIGH) {
    btn_timer++;
    if(btn_timer > 200){
      if(saturation == 255){
        adding = -1;
      }else if(saturation == 0){
        adding = 1;
      }
      saturation = saturation + adding;
    }
  }else{
    btn_timer = 0;
  }
  
  for(int i=0;i<5;i++){
    if(init_hue[i] < end_hue[i]){
      init_hue[i] = init_hue[i] + 1;
      if(init_hue[i] > MAX_HUE) init_hue[i] = MAX_HUE;
    }else if(init_hue[i] > end_hue[i]){
      init_hue[i] = init_hue[i] - 1;
      if(init_hue[i] < MIN_HUE) init_hue[i] = MIN_HUE;
    }else if(init_hue[i] == end_hue[i]){
      end_hue[i] = random(MIN_HUE,MAX_HUE);  
    }
    
    hsb2rgb(init_hue[i], saturation, brightness, rgb_converted);
      
    value.r = rgb_converted[0];
    value.g = rgb_converted[1];
    value.b = rgb_converted[2];
    
    LED.set_crgb_at(i, value);    
  }
  LED.sync(); // Sends the value to the LED
  delay(STEPS_DELAY); 
  
  
}

/******************************************************************************
 * from: http://www.johngineer.com/blog/?p=1022
 * 
 * accepts hue, saturation and brightness values and outputs three 8-bit color
 * values in an array (color[])
 *
 * saturation (sat) and brightness (bright) are 8-bit values.
 *
 * hue (index) is a value between 0 and 767. hue values out of range are
 * rendered as 0.
 *
 *****************************************************************************/
void hsb2rgb(uint16_t index, uint8_t sat, uint8_t bright, uint8_t color[3])
{
  uint16_t r_temp, g_temp, b_temp;
  uint8_t index_mod;
  uint8_t inverse_sat = (sat ^ 255);

  index = index % 768;
  index_mod = index % 256;

  if (index < 256)
  {
    r_temp = index_mod ^ 255;
    g_temp = index_mod;
    b_temp = 0;
  }

  else if (index < 512)
  {
    r_temp = 0;
    g_temp = index_mod ^ 255;
    b_temp = index_mod;
  }

  else if ( index < 768)
  {
    r_temp = index_mod;
    g_temp = 0;
    b_temp = index_mod ^ 255;
  }

  else
  {
    r_temp = 0;
    g_temp = 0;
    b_temp = 0;
  }

  r_temp = ((r_temp * sat) / 255) + inverse_sat;
  g_temp = ((g_temp * sat) / 255) + inverse_sat;
  b_temp = ((b_temp * sat) / 255) + inverse_sat;

  r_temp = (r_temp * bright) / 255;
  g_temp = (g_temp * bright) / 255;
  b_temp = (b_temp * bright) / 255;

  color[0]  = (uint8_t)r_temp;
  color[1]  = (uint8_t)g_temp;
  color[2] = (uint8_t)b_temp;
}

