#include <SoftwareSerial.h>
#include <IRremote.h>

#define TXPIN 2
#define RXPIN 3
#define IRPIN 5

SoftwareSerial btSerial(TXPIN, RXPIN); //블루투스 핀 번호
String data;
IRrecv irrecv(IRPIN); //적외선 핀 번호
decode_results Signal;

void setup()
{
    btSerial.begin(9600);
    Serial.begin(9600);
    irrecv.enableIRIn();
}

void loop()
{
    String getData = getRemoteDataDec();
    data +=getData;
    if(getData.length()!=1)
        sendData();

}
void sendData()
{
    Serial.println("send : "+data);
    btSerial.write(&data);
    data="";
}
String getRemoteDataDec()
{
    String inputData = "";
    if (irrecv.decode(&Signal) != true)
        return;
    Serial.println(Signal.value);
    // btSerial.write() 괄호 안에 각 버튼에 어울리는 알파벳을 지정해 전송합니다
    switch (Signal.value)
    {
    case 16738455: // 0 FF6897 16738455
        inputData = "0";
        break;
    case 16724175: // 1 FF30CF 16724175
        inputData = "1";
        break;
    case 16718055: // 2 FF18E7 16718055
        inputData = "2";
        break;
    case 16743045: // 3 FF7A85 16743045
        inputData = "3";
        break;
    case 16716015: // 4 FF10EF 16716015
        inputData = "4";
        break;
    case 16726215: // 5 FF38C7 16726215
        inputData = "5";
        break;
    case 16734885: // 6 FF5AA5 16734885
        inputData = "6";
        break;
    case 16728765: // 7 FF42BD 16728765
        inputData = "7";
        break;
    case 16730805: // 8 FF4AB5 16730805
        inputData = "8";
        break;
    case 16732845: // 9 FF52AD 16732845
        inputData = "9";
        break;
    case 16753245: // ch- 16753245
        inputData = "/-CH";
        break;
    case 16736925: // ch  16736925
        inputData = "/CH";
        break;
    case 16769565: // ch+ 16769565
        inputData = "/+CH";
        break;
    case 16720605: // <<  16720605
        inputData = "/BACK";
        break;
    case 16712445: // >>  16712445
        inputData = "/NEXT";
        break;
    case 16761405: // enter 16761405
        inputData = "/ENTER";
        break;
    case 16769055: // -   16769055
        inputData = "/SUB";
        break;
    case 16754775: // +   16754775
        inputData = "/SUM";
        break;
    case 16750695: // 100+  FF9867 16750695
        inputData = "/100";
        break;
    case 16756815: // 200+  FFB04F 16756815
        inputData = "/200";
        break;
    }
    irrecv.resume();
    return inputData;
}