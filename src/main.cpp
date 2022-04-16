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
// U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/12, /* cs=*/26, /* dc=*/27, /* reset=*/14);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/16, /* data=*/17, /* reset=*/U8X8_PIN_NONE); // ESP32 Thing, pure SW emulated I2C

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
GradeController gradeCtl(15, 18, 19);
UpDownController upDownCtl(32, 33);
void setup()
{
  // write your initialization code here
  Serial.begin(115200);
  u8g2.begin();
  core.AddController(&gradeCtl);
  core.AddController(&upDownCtl);
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
  if (i++ % 50 == 0)
  {
    char output[50] = {0};
    char output1[50] = {0};
    sprintf(output, "T:%d", gradeCtl.TargetGrade());
    sprintf(output1, "C:%d", gradeCtl.CurrentGrade());

    // Serial.println(output);

    u8g2.clearBuffer();                 // 清除内部缓冲区
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.drawStr(0, 20, output);        // write something to the internal memory
    u8g2.drawStr(0, 40, output1);       // write something to the internal memory
    u8g2.sendBuffer(); // transfer internal memory to the display
  };
  // write your code here
}