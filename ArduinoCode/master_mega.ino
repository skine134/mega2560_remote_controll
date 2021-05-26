#include <MegaRemoteControl.h>
#include <SoftwareSerial.h>

#define TX_PIN 2
#define RX_PIN 3

MegaRemoteControl megaRemoteControl;
SoftwareSerial btSerial(TX_PIN, RX_PIN);
String inputString="";
void setup()
{
//    Serial3.begin(9600);
    megaRemoteControl.begin();
    btSerial.begin(9600);
}

void loop()
{
    //읽을 수 있는 데이터가 없음.
    //    if(!Serial3.available())
    if (!btSerial.available())
        return;
    //    char inputChar = Serial3.read();
    char inputChar = btSerial.read();
    //    Serial.println("input : "+inputChar);
    inputString += inputChar;
    // 데이터를 다 못읽었으면 리턴, 다 읽었으면 다음 진행
    int finishLoc = inputString.indexOf("<EOF>");
    if (finishLoc == -1)
        return;
    megaRemoteControl.startRemoteAction(inputString.substring(0,finishLoc));
    inputString = "";
}
