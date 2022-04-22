#include <Arduino.h>
#include <U8g2lib.h>
#include <unordered_map>
#include <map>
#include <list>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include "core.h"
#include "controller.h"
#include "controllers/grade_controller.h"
#include "controllers/updown_controller.h"
#include "controllers/rpm_controller.h"
// U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/12, /* cs=*/26, /* dc=*/27, /* reset=*/14);
   U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/5, /* data=*/4 , /* reset=*/U8X8_PIN_NONE); // ESP32 Thing, pure SW emulated I2C

class TestController : public Controller
{
public:
  void Poll(unsigned long time) override
  {
    Serial.print("from test controller,time = ");
    Serial.println(time);
  }
  void Init()
  {
  }

  int Interval() override
  {
    return 1000;
  }
};
Core core;
TestController testController;
GradeController gradeCtl(34, 22, 23);
UpDownController upDownCtl(14, 27);
RpmController bmpCtl(32, 33);
int tick = 0;
void setup()
{
  // write your initialization code here
  Serial.begin(115200);
  u8g2.begin();
  u8g2.enableUTF8Print(); // 使print支持UTF8字集
  core.AddController(&bmpCtl);
  core.AddController(&gradeCtl);
  core.AddController(&upDownCtl);
  bmpCtl.Init();
  gradeCtl.Init();
  upDownCtl.Init();
  upDownCtl.On(UpDownEvent::UpOne, [&]()
               { gradeCtl.UpOneGrade(); });
  upDownCtl.On(UpDownEvent::DownOne, [&]()
               { gradeCtl.DownOneGrade(); });
  upDownCtl.On(UpDownEvent::KeepDownOne, [&]()
               { gradeCtl.DownOneGrade(); });
  upDownCtl.On(UpDownEvent::KeepUpOne, [&]()
               { gradeCtl.UpOneGrade(); });

}

void loop()
{
  auto c = millis();
  core.Poll(c);
  delay(10);
  static int i = 0;
  if (i++ % 10 == 0)
  {
    char gradeOutput[50] = {0};
    char rmpOutput[50] = {0};
    char xyOutput[50] = {0};
    char timeOutput[50] = {0};
    sprintf(gradeOutput, "L = %d", gradeCtl.TargetGrade());
    sprintf(rmpOutput, "R = %d", bmpCtl.Rpm());
    // sprintf(gradeOutput, "L = %d,V= %d", gradeCtl.TargetGrade(),gradeCtl.CurrentValue());
    // sprintf(rmpOutput, "R = %d,C = %d", bmpCtl.Bpm(),bmpCtl.Count());
    // sprintf(xyOutput,"X = %d",upDownCtl.X());
    auto time = millis()/1000;
    auto hour = time/3600;
    auto min = (time%3600)/60;
    auto second = time%60;
    sprintf(timeOutput,"T = %d:%d:%d",hour,min,second);
    // Serial.println(output);

    u8g2.clearBuffer();                 // 清除内部缓冲区
    u8g2.setFont(u8g2_font_ncenB12_tr); // choose a suitable font
    u8g2.drawStr(0, 20, gradeOutput);        // write something to the internal memory
    u8g2.drawStr(0, 40, rmpOutput);       // write something to the internal memory
 //   u8g2.drawStr(0, 45, xyOutput);       // write something to the internal memory
    u8g2.drawStr(0, 60, timeOutput);       // write something to the internal memory
  //     u8g2.setFont(u8g2_font_wqy12_t_chinese2);  // use wqy chinese2 for all the glyphs of "你好世界"
  // u8g2.setFontDirection(0);
  // u8g2.clearBuffer();
  // u8g2.setCursor(0, 15);
  // u8g2.print("Hello World!");
  // u8g2.setCursor(0, 40);
  // u8g2.print("你好世界");		// Chinese "Hello World" 
  // u8g2.sendBuffer();
    u8g2.sendBuffer();                  // transfer internal memory to the display
  };
  // write your code here
}