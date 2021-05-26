#include <MegaRemoteControl.h>
#include <Arduino.h>
#include <RgbLcd.h>
#include <FND.h>
#include <Buzzer.h>
#include <Temperature.h>
#include <LED.h>
#include <DCMotor.h>
#include <PIR.h>
#include <Sound.h>
#define TEMP_ADDR 72 //각각의 arduino 예제 핀번호
#define BUZ_PIN 11
#define FND_ADDR_1 38
#define FND_ADDR_2 32
#define LED_ADDR_1 36
#define DCIN_A 7
#define DCIN_B 8
#define PIR_ADC 39
#define SOUND_ADC 56

void MegaRemoteControl::begin()
{
    myled.begin(LED_ADDR_1);

    myfnd.begin(FND_ADDR_1, FND_ADDR_2);

    temper.begin(TEMP_ADDR);

    myBuzzer.begin(BUZ_PIN);

    dcMotor.begin(DCIN_A, DCIN_B);

    pir.begin(PIR_ADC);
    Serial.begin(9600);

    sound.begin(SOUND_ADC);
    actionFlag = false;
    // LCD 핀번호 설정,  마지막 16,2는 LCD가  16 by 2라는 뜻이다.
    lcd.begin(62, 63, 64, 65, 66, 67, 45, 44, 43, 42, 16, 2);
    lcd.onBacklightGreen();
}
void MegaRemoteControl::startRemoteAction(String inputString)
{
    readStringData(inputString);
    readType(tag);
    onStartModule(channel, currentValue);
}
void MegaRemoteControl::readStringData(String inputString)
{
    // /를 구분으로 명령인자와 데이터 값 구분.
    Serial.print("input : ");
    Serial.println(inputString);
    int splitLoc = inputString.indexOf("/");
    data = inputString.substring(0, splitLoc).toInt();               // 데이터 저장
    tag = inputString.substring(splitLoc + 1, inputString.length()); // 명령인자 저장
    Serial.print("data :");
    Serial.println(data);
    Serial.print("tag : ");
    Serial.println(tag);
    inputString = "";
    actionFlag = true;
}
void MegaRemoteControl::readType(String tag)
{
    if (tag.indexOf("CH") == 0)
        readData(data, true);
    else if (tag.indexOf("ENTER") == 0)
        readData(data, false);
    else if (tag.indexOf("+CH") == 0)
        calCulCh(data, true);
    else if (tag.indexOf("-CH") == 0)
        calCulCh(data, false);
    else if (tag.indexOf("SUM") == 0)
        calCulValue(data, true);
    else if (tag.indexOf("SUB") == 0)
        calCulValue(data, false);
}
void MegaRemoteControl::readData(int data, bool channelFlag)
{
    if (channelFlag)
    {

        if (data < 0 || CHANNEL_BUZZER < data)
            return;
        channel = data;
        currentValue = 0;
    }
    else
    {
        currentValue = data;
    }
}
void MegaRemoteControl::calCulCh(int data, bool sumFlag)
{
    int operand = data;
    if (operand == 0)
        operand = 1;
    int temp = 0;
    if (sumFlag)
        temp = channel + operand;
    else
        temp = channel - operand;
    if (temp < 0 || CHANNEL_BUZZER < temp)
        return;
    channel = temp;
}
void MegaRemoteControl::calCulValue(int data, bool sumFlag)
{
    int operand = data;
    if (operand == 0)
        operand = 1;
    int temp = 0;
    if (sumFlag)
        temp = currentValue + operand;
    else
        temp = currentValue - operand;
    if (temp < 0)
        return;
    currentValue = temp;
}
void MegaRemoteControl::onStartModule(int channel, int currentValue)
{
    if (!actionFlag)
        return;
    actionFlag = false;
    switch (channel)
    {
    case CHANNEL_LED:
        ledAction(currentValue);
        break;
    case CHANNEL_DC_MOTOR:
        dcMotorAction(currentValue);
        break;
    case CHANNEL_FND:
        fndAction(currentValue);
        break;
    case CHANNEL_BUZZER:
        buzzerAction(currentValue);
        break;
    }
}

void MegaRemoteControl::ledAction(int currentValue)
{
    Serial.print("led : ");
    Serial.println(currentValue);
    //currentValue 값에 따라 led 불 밝혀짐
    if (currentValue == 0)
        myled.AllOff();
    else if (currentValue == 9)
        myled.AllOn();
    else
        myled.On(currentValue);
}

void MegaRemoteControl::dcMotorAction(int currentValue)
{
    Serial.print("dcmotor : ");
    Serial.println(currentValue);
    //currentValue 값이 0이면 정지 1이면 정회전 2면 역회전
    if (currentValue == 1)
    { //1번누르면 정회전
        dcMotor.Start(HIGH);
        return;
    }
    else if (currentValue == 0)
    { // 0번누르면 정지
        dcMotor.Stop();
        return;
    }
    else if (currentValue == 2)
    { // 2번 누르면 역회전
        dcMotor.Start(LOW);
        return;
    }
}
void MegaRemoteControl::fndAction(int currentValue)
{
    Serial.print("fnd : ");
    Serial.println(currentValue);
    //currentValue값 보여줌
    int south = (currentValue / 1000) % 10;
    int hundred = (currentValue / 100) % 10;
    int ten = (currentValue / 10) % 10;
    int one = (currentValue) % 10;
    myfnd.setAllNum(south, hundred, ten, one);
}
void MegaRemoteControl::buzzerAction(int currentValue)
{
    Serial.print("buzzer : ");
    Serial.println(currentValue);
    //currentValue값 만큼 소리 진폭 적용
    myBuzzer.setFreq(currentValue);
}