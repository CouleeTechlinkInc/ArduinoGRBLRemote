#include <EEPROM.h>

#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 27;
int OkMode = 0;
int ledPin = 11;
IRrecv irrecv(RECV_PIN);

decode_results results;
decode_results lastCommand;
void setup()
{
  pinMode( ledPin , OUTPUT );
  Serial.begin(115200);
  Serial1.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Mouse.begin();
  Keyboard.begin();
}

int runCommand( decode_results command ){
  int runLast = 0;
switch( command.value & 0xFFFFFFF ){
      case 0xFF02FD:
        if( OkMode  == 0 ){
          Serial.print("SPACE");
          Keyboard.set_modifier(0);
          Keyboard.set_key1(KEY_SPACE);
          Keyboard.send_now();
          Keyboard.set_modifier(0);
          Keyboard.set_key1(0);
          Keyboard.send_now();
        }
        if( OkMode == 1 ){
          Serial.print("MOUSE CLICK");
          Mouse.click();
        }
      break;
      case 0xFFA857:
        Serial.print("Down");
        Mouse.move(0, 40);
      break;
      case 0xFF629D:
        Serial.print("UP");
        Mouse.move(0, -40);
       break;
       case 0xFFC23D:
        Serial.print("Right");
        Mouse.move(40, 0);
       break;
       case 0xFF22DD:
         Serial.print("Left");
         Mouse.move(-40, 0);
       break;
       case 0xFF6897:
         Serial.print("1");
       break;
       case 0xFF9867:
         Serial.print("2");
       break;
       case 0xFFB04F:
         Serial.print("3");
       break;
       case 0xFF30CF:
         Serial.print("4");
       break;
       case 0xFF18E7:
         Serial.print("5");
       break;
       case 0xFF7A85:
         Serial.print("6");
       break;
       case 0xFF10EF:
         Serial.print("7");
       break;
       case 0xFF38C7:
         Serial.print("8");
       break;
       case 0xFF5AA5:
         Serial.print("9");
       break;
       case 0xFF42BD:
         Serial.print("*");
         OkMode = 0;
         digitalWrite( ledPin , HIGH );
       break;
      case 0xFF4AB5:
         Serial.print("0");

       break;
       case 0xFF52AD:
         Serial.print("#");
         OkMode = 1;
         digitalWrite( ledPin , HIGH );
       break;
       case 0xFFFFFFF:
         runLast = 1;
         Serial.print("+");
       break;
       default:
         runLast = 2;
         Serial.println(command.value , HEX);
                  
       break;
    }
    return runLast;
}

void loop() {
  int incomingByte;
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial1.write(incomingByte);
  }
  if (Serial1.available() > 0) {
    incomingByte = Serial1.read();
    Serial.write(incomingByte);
  }
  
  if (irrecv.decode(&results)) {
    
    int runLast = runCommand(results);
    if( runLast == 1 ){
      runCommand(lastCommand);
    }
    if( runLast == 0 ){
     lastCommand = results;
    } 
    irrecv.resume(); // Receive the next value
  }
}
