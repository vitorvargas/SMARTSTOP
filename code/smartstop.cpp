#define b1 13
#define b2 12
#define led 11 //quando estiver aceso, o botao deverá ser apertado
#define PIN_GPRS_RST 6
#define LINHA_1 1
#define LINHA_2 2
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
SoftwareSerial ser(2, 3); // pinos 2 e 3 - tx rx respectivamente
int connectGPRS(void);
String response;
//
###########################################################################
// SETUP
//
###########################################################################
void setup(){
//delay(10000);
Serial.begin(9600);
mp3_set_serial (Serial);
//set Serial for DFPlayer-mini mp3 module
delay(1);
// delay 1ms to set volume
mp3_set_volume (26);
// value 0~30
ser.begin(9600);
//Serial.println("----------------------------------");
pinMode(led, OUTPUT);
pinMode(b1, INPUT_PULLUP);
pinMode(b2, INPUT_PULLUP);33
// return;
pinMode(PIN_GPRS_RST, OUTPUT);
digitalWrite(PIN_GPRS_RST, HIGH);
delay(3000);
digitalWrite(PIN_GPRS_RST, LOW);
delay(12000);
while(connectGPRS() != 0);
}// fim do void setup
//
###########################################################################
// LOOP
//
###########################################################################
void loop()
{
processalinhas();
delay(2000);
}
int processa_resposta(int linha)
{
response.trim();
Serial.print("response65=");
Serial.println(response);
if( response == "t_0" ) {
Serial.println("Falta menos de um minuto");
mp3_play (1);
delay(6000);
} else if( response == "t_1" ) {
Serial.println("Falta um minuto");
mp3_play (2);
delay(6000);
}else if( response == "t_2" ) {34
Serial.println("Faltam dois minutos");
mp3_play (3);
delay(6000);
}else if( response == "t_3" ) {
Serial.println("Faltam tres minutos");
mp3_play (4);
delay(6000);
}else if( response == "t_4" ) {
Serial.println("Faltam quatro minutos");
mp3_play (5);
delay(6000);
}else if( response == "t_5" ){
Serial.println("Falta um minuto");
mp3_play (6);
delay(6000);
}else if( response == "t_6" ){
Serial.println("Faltam seis minutos");
mp3_play (7);
delay(6000);
}else if( response == "t_7" ){
Serial.println("Faltam sete minutos");
mp3_play (8);
delay(6000);
}else if( response == "t_8" ){
Serial.println("Faltam oito minutos");
mp3_play (9);
delay(6000);
}else if( response == "t_9" ){
Serial.println("Faltam nove minutos");
mp3_play (10);
delay(6000);
}else if(response == "+"){
Serial.println("Faltam nove minutos");
mp3_play (10);
delay(6000);
}
return 0;
}
//
###########################################################################
// CONNECT GPRS35
//
###########################################################################
int connectGPRS(void)
{
boolean tryAgain = true;
while(tryAgain) {
if ( sendATcommand("ATZ", "OK", "ERROR", 7000) !=1 )
continue;
if (sendATcommand("AT+CPIN?", "OK", "ERROR", 3000) !=1 )
continue;
if (sendATcommand("AT+CREG?", "OK", "ERROR", 3000) !=1 )
continue;
if (sendATcommand("AT+CGATT?", "+CGATT: 1", "ERROR", 5000) !=1 ) {
sendATcommand("AT+CGATT=1", "OK", "ERROR", 4000);
continue;
}
if (sendATcommand("AT+CIPMUX=0", "OK", "ERROR", 3000) !=1 )
continue;
if (sendATcommand("AT+CSTT=\"tim.br\",\"tim\",\"tim\"", "OK", "ERROR", 3000)
!=1 )
continue;
if (sendATcommand("AT+CIPSTATUS", "OK", "ERROR", 6000) !=1 )
continue;
if (sendATcommand("AT+CIICR", "OK", "ERROR", 12000) !=1 )
continue;
sendATcommand("AT+CIFSR", "lalalalal", "ERROR", 2000);
if
(sendATcommand("AT+CIPSTART=\"TCP\",\"ec2-52-67-56-149.sa-east-1.compute.amazon
aws.com\",\"54155\"", "CONNECT OK", "ERROR", 10000) !=1 )
//
if
(sendATcommand("AT+CIPSTART=\"TCP\",\"ec2-52-67-56-149.sa-east-1.compute.amazon
aws.com\",\"8081\"", "CONNECT OK", "ERROR", 10000) !=1 )
continue;
tryAgain = false;
}
return 0;
}
int processalinhas()
{36
int linha;
int bn,bv, bn2, bv2;
digitalWrite (led, HIGH); //escolher a linha do ônibus
Serial.println("aperte um botao");
bv=bn;
bv2=bn2;
bn=digitalRead(b1);
bn2=digitalRead(b2);
Serial.println(bn);
Serial.println(bn2);
if (bv== HIGH || bn == LOW)
{
Serial.println("botao1 pressionado");
sendDataSockets("lA15_abc", 10000);
linha=LINHA_1;
} else if(bv2 == HIGH || bn2 == LOW) {
Serial.println("botao2 pressionado");
sendDataSockets("lA19_def", 10000);
linha=LINHA_2;
} else
return 0;
delay(100);
int answer = readSerialWithTimeout("SEND OK", "ERROR", 1000);
//ou //CONNECT OK
if( answer == 1 ) {
Serial.print("response=");
Serial.println(response);
answer = readSerialWithTimeout("lalalaja", "?", 500);
if( answer == 2)
return -1;
Serial.print("response162=");
Serial.println(response);
processa_resposta(linha);
} else if(answer == 2){37
Serial.println("401");
//TODO
} else if(answer == 0){
Serial.println("[ERROR] CONNECTION");
delay(1000);
}
}
//
###########################################################################
// SEND AT COMMAND
//
###########################################################################
int sendATcommand(String ATcommand, String expectedAnswer, String expectedError,
unsigned int timeout){
delay(100);
ser.flush();
while(ser.available() > 0){ ser.read();}
// Clean the input buffer
ser.println(ATcommand);
Serial.print("AT enviado: ");
Serial.println(ATcommand);
// Send the AT command
return readSerialWithTimeout(expectedAnswer, expectedError, timeout);
} // fim do send at command
//
###########################################################################
// READ SERIAL WITH TIMEOUT (int)
//
###########################################################################
int readSerialWithTimeout(String expectedAnswer, String expectedError, unsigned int
timeout){
response = "";
char bytes;
int answer = 0; //0 = no response, 1 = okay, 2 = error
unsigned long previous;
//
//
//
delay(5);
ser.flush();
while(ser.available() > 0){ ser.read();}
previous = millis();
// Clean the input buffer38
do{
if(ser.available() > 0){
bytes = ser.read();
response += bytes;
if(searchString(response, expectedAnswer)){
answer = 1;
break;
} else if(searchString(response, expectedError)){
answer = 2;
break;
}
}
}
// Waits for the asnwer with time out
while((answer == 0) && ((millis() - previous) < timeout));
Serial.print("answer: ");
Serial.println(answer);
Serial.print("resposta:");
Serial.print(response);
Serial.println("");
return answer;
} // fim do Read Serial with time out
//
###########################################################################
// SEARCH STRING
//
###########################################################################
boolean searchString(String original, String ocorrence){
int found = 0;
int index_occ=0;
if(original.length() > 0){
for(int i=0; i< original.length() && found == 0; i++){
if(original[i] == ocorrence[index_occ]){
index_occ++;
}else{39
index_occ=0;
}
if(index_occ == ocorrence.length()){
found = 1;
return true;
}
}
}
if(found == 0){
return false;
}
return false;
} //fim do Boolean Search String
//
###########################################################################
// SEND DATA SOCKETS
//
###########################################################################
void sendDataSockets(String data, unsigned timeout){
String c="AT+CIPSEND="+String(data.length());
if(sendATcommand(c, ">", "ERROR", timeout) == 1){
delay(10);
ser.println(data);
ser.write(0x1a);
Serial.print("enviando: ");
Serial.println(data);
delay(10);
}else{
Serial.print("[ERROR] AT+CIPSEND");
Serial.print("datasockets:");
Serial.print(data);
Serial.println("]]]");
}
} //fim do void SendDataSockets
