/* MegaRemoteControl.h */
#ifndef __MEGA_REMOTE_CONTROL_H__
#define __MEGA_REMOTE_CONTROL_H__

#include <RgbLcd.h> //각각 예제 헤더
#include <FND.h>
#include <Buzzer.h>
#include <Temperature.h>
#include <LED.h>
#include <DCMotor.h>
#include <PIR.h>
#include <Sound.h>
#include <RgbLcd.h>

enum channel
{
    CHANNEL_LED,
    CHANNEL_DC_MOTOR,
    CHANNEL_FND,
    CHANNEL_BUZZER
};

class MegaRemoteControl
{
private:
    FND myfnd;
    RgbLcd lcd;
    Buzzer myBuzzer;
    LED myled;
    Temperature temper;
    DCMotor dcMotor;
    PIR pir;
    Sound sound;

    String inputString;
    String tag;
    int channel;
    int currentValue;
    int data;
    bool actionFlag;
    // bool isHundred;
    // bool isTwoHundred;

public:
    void begin();
    void startRemoteAction(String inputString);
    void readStringData(String inputString);
    void readType(String tag);
    void onStartModule(int channel,int curentValue);
    void readData(int data, bool channelFlag);
    void calCulCh(int data, bool sumFlag);
    void calCulValue(int data, bool sumFlag);

    void ledAction(int data);

    void dcMotorAction(int data);

    void fndAction(int data);

    void buzzerAction(int data);
};
extern MegaRemoteControl gMegaRemoteControl;
#endif
