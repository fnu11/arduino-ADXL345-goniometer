#include <Wire.h> 
#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_ADAFRUIT_128X64
//U8GLIB_SSD1306_128X64
 
// lol draw :) pr-2
void draw(int pr) {  
  char pr1_buffer[4];
   
  if (pr>45 && pr<135){ 
    u8g.setRot270();
    u8g.setFont(u8g_font_osb29n);
    sprintf(pr1_buffer, "%d", pr-2); 
    u8g.drawStr( 0, 75, pr1_buffer);
  }
  
  if (pr>0 && pr<45) { 
    u8g.undoRotation();
    u8g.setFont(u8g_font_osb35n);
    sprintf(pr1_buffer, "%d", pr-4); //correction :D
    u8g.drawStr( 25, 50, pr1_buffer);
  }
  
  if (pr>135 && pr<180){
    u8g.setRot180();
    u8g.setFont(u8g_font_osb35n);
    sprintf(pr1_buffer, "%d", pr+1); 
    u8g.drawStr( 25, 50, pr1_buffer);
  }
  
  if (pr<0) {
    u8g.undoRotation();
    u8g.setFont(u8g_font_osb35n);
    sprintf(pr1_buffer, "%d", pr-2); 
    u8g.drawStr( 25, 50, pr1_buffer);
  } 
}
 

int ADXL345 = 0x53;  
float X_out, Y_out, Z_out;  
int pr1; 

void setup() {   
  delay(50); 
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();      

  u8g.begin();  
  u8g.setHardwareBackup(u8g_backup_avr_spi);  
  u8g.setFont(u8g_font_osb35n);
  u8g.setColorIndex(0); 
  u8g.setHiColorByRGB(255,255,255);
  delay(50);  
}

void loop() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
  X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
  Y_out = Y_out/256;
  Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
  Z_out = Z_out/256;

  /*
  Serial.print("Xa= ");
  Serial.print(X_out);
  Serial.print("   Ya= ");
  Serial.print(Y_out);
  Serial.print("   Za= ");
  Serial.println(Z_out);*/ 
 
  pr1 = atan2(Y_out, X_out) * 180 / PI + 90;
    
  u8g.firstPage();  
  do {
    draw(pr1);
  } while( u8g.nextPage() );
  delay(500); 
}
