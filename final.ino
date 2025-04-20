#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>
#include <PingPongGame.h>
#include <pitches.h>

#define REST 0

//Number screen
#define TM_STROBE 11
#define TM_CLOCK 12
#define TM_DIO 13

// screen
LiquidCrystal_I2C lcd(0x27, 16, 2);
// number screen
TM1638plus tm(TM_STROBE, TM_CLOCK, TM_DIO);
//game
SimpleTimer timer;
LedControl lc = LedControl(9, 8, 7, 1);
PingPongGame pingPong(lc, timer);

int greenLed = D6;
int redLed = D10;
int buzzer = D5;
int challenge = 0;
bool firstRun = true;
int time;
int currentNumber;
int currentRound;
//morse code game
const int totalRounds = 8;
bool challenge2Active = false;
bool waitingForInput = false;
//time
unsigned long countDownStart = 0;
const unsigned long countDownDuration = 5 * 60 * 1000UL;
bool countdownActive = false;
//simon game
const int maxSequence = 5;
int sequence[maxSequence];
int userIndex = 0;
int simonCurrentRound = 0;
bool waitingForInputSimon = false;
bool simonPlayingSequence = false;
bool simonGameActive = false;
const int simonTones[8] = {
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
    NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

//final game
int finalSequence[] = {2, 5, 8, 6};
int finalUserInput[4];
int finalInputIndex = 0;
bool finalChallengeActive = false;

int melody[] = {
    NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
    NOTE_G5, REST, NOTE_G4, REST
};
int noteDurations[] = {
    8, 8, 8, 8, 8, 8, 8,
    4, 4, 4, 4
};


void updateCountdown() {
    if (!countdownActive) return;

    unsigned long elapsed = millis() - countDownStart;
    
    if (elapsed >= countDownDuration) {
        tm.displayText("TIMEUP");
        countdownActive = false;
    } else {
        unsigned long remaining = (countDownDuration - elapsed) / 1000;  // Seconds left
        int minutes = remaining / 60;
        int seconds = remaining % 60;

        char buffer[9];
        sprintf(buffer, "%02d:%02d", minutes, seconds);
        tm.displayText(buffer);
    }
}

void challengeFailed(){
    lcd.clear();
    lcd.print("FAILED");
    lcd.setCursor(0, 1);
    lcd.print("Time expired");

    tm.displayText("TIME UP ");

    for (int i = 0; i < 3; i++) {
        tone(buzzer, NOTE_G2, 500);
        delay(600);
        noTone(buzzer);
        delay(400);
    }

    finalChallengeActive = false;
    challenge2Active = false;
    simonGameActive = false;
}


void buzzerSound(){
    tone(buzzer, 1000);
    delay(500);
    noTone(buzzer);
}

void errorSound(){
    tone(buzzer, NOTE_G3, 300);
    delay(400);
    tone(buzzer, NOTE_C3, 500);
    delay(600);
    noTone(buzzer);
}

void successSound() {
    tone(buzzer, NOTE_C5, 100);
    delay(120);
    tone(buzzer, NOTE_E5, 100);
    delay(120);
    tone(buzzer, NOTE_G5, 200);
    delay(250);
    noTone(buzzer);
}

void scrollText(String text, int lineNum, int delayTime){
    for(int i = 0; i < text.length() - 16; i++){
        lcd.setCursor(0, lineNum);
        lcd.print(text.substring(i, i + 16));
        delay(200);
    }
    delay(delayTime);
    lcd.clear();
}

void challengeInfo(){
lcd.print("Welcome to my");
    lcd.setCursor(0, 1);
    lcd.print("challenge!");
    delay(2000);
    lcd.clear();

    scrollText("To escape the room, certain challenges are presented!", 0, 2000);
    lcd.clear();

    lcd.print("After challenge,");
    lcd.setCursor(0, 1);
    lcd.print("a number appears");
    delay(4000);
    lcd.clear();

    scrollText("in the end, press button = to the number", 0, 2000);
    lcd.clear();
}

void challenge2Info(){
    lcd.print("Challenge 2:");
    lcd.setCursor(5, 1);
    lcd.print("Morse code");
    delay(2000);
    lcd.clear();

    lcd.print("green led='.'");
    lcd.setCursor(5, 1);
    lcd.print("red led='-'");
}

void glowGreen(){
    digitalWrite(greenLed, HIGH);
    delay(500);
    digitalWrite(greenLed, LOW);
    delay(500);
}

void glowRed(){
        digitalWrite(redLed, HIGH);
        delay(500);
        digitalWrite(redLed, LOW);
        delay(500);
}

void pressBtn(){
    if(!waitingForInput) return;
    uint8_t buttons = tm.readButtons();

    for(int btn = 0; btn < 8; btn++){
        if(buttons & (1 << btn)){
            if(btn + 1 == currentNumber){
                tm.setLED(currentRound, 1);
                successSound();

                currentRound++;
                waitingForInput = false;

                if(currentRound >= totalRounds){
                    challenge2Complete();
                } else{
                    delay(1000);
                    nextRound();
                }
            } else{
                errorSound();
                morseNumber(currentNumber);
                waitingForInput = true;
            }
            while(tm.readButtons());
            break;
        }
    }
}

void nextRound(){
    currentNumber = random(1, 9);
    morseNumber(currentNumber);
    waitingForInput = true;
}

void challenge2Complete(){
    challenge2Active = false;

  for(int i = 0; i < 3; i++) {
    tm.setLEDs(0b11111111);
    successSound();
    delay(300);
    tm.setLEDs(0b00000000);
    delay(300);
  }
    tm.displayASCII(7, '5');
    challenge = 3;
}

void morseNumber(int number){
    if(number <=5){
        for(int i = 0; i < number; i++) glowGreen();
        for(int i = 0; i < 5 - number; i++) glowRed();
    } else{
        number -= 5;
        for(int i = 0; i < number; i++) glowRed();
        for(int i = 0; i < 5 - number; i++) glowGreen();
    }

}

void challenge2Action(){
    challenge2Active = true;
    currentRound = 0;
    tm.setLEDs(0b00000000);
    nextRound();
}

void challenge3Info(){
    lcd.print("Challenge 3:");
    lcd.setCursor(5, 1);
    lcd.print("Simon game");
    delay(2000);
}

void startSimonGame(){
    simonCurrentRound = 1;
    userIndex = 0;
    generateSequence();
    simonPlayingSequence = true;
    simonGameActive = true;
}

void generateSequence(){
    for(int i = 0; i < maxSequence; i++){
        sequence[i] = random(0, 8);
    }
}

void playSequence(){
    tm.setLEDs(0);
    for(int i = 0; i < simonCurrentRound; i++){
        int led = sequence[i];
        tm.setLED(led, 1);
        tone(buzzer, simonTones[led], 300);
        delay(500);
        tm.setLED(led, 0);
        noTone(buzzer);
        delay(250);
    }
    simonPlayingSequence = false;
}

void checkUserInput() {
    uint8_t buttons = tm.readButtons();
    if(buttons){
        for (int btn = 0; btn < 8; btn++) {
            if (buttons & (1 << btn)) {
                    tm.setLED(btn, 1);
                    tone(buzzer, simonTones[btn], 200);
                    delay(200);
                    tm.setLED(btn, 0);
                    noTone(buzzer);
                if (btn == sequence[userIndex]) {
                    userIndex++;
                    if (userIndex >= simonCurrentRound) {
                        if (simonCurrentRound > maxSequence) {
                            simonWin();
                            lcd.clear();
                            lcd.print("8");
                            delay(5000);
                            challenge = 4;
                        } else{
                            simonCurrentRound++;
                            userIndex = 0;
                            simonPlayingSequence = true;
                            delay(1000);
                        }
                    }
                } else {
                    simonLose();
                    startSimonGame();
                }
                while (tm.readButtons());
                break;
            }
        }
    }
}

void simonLose() {
    for (int i = 0; i < 3; i++) {
        tm.setLEDs(0b11111111);
        tone(buzzer, NOTE_G3, 200);
        delay(200);
        tm.setLEDs(0);
        tone(buzzer, NOTE_C3, 400);
        delay(400);
    }
    noTone(buzzer);
    delay(2000);
}

void simonWin() {
    for (int i = 0; i < 3; i++) {
        tm.setLEDs(0b11111111);
        tone(buzzer, NOTE_C5, 100);
        delay(120);
        tone(buzzer, NOTE_E5, 100);
        delay(120);
        tone(buzzer, NOTE_G5, 200);
        delay(250);
        tm.setLEDs(0);
        delay(200);
    }
    noTone(buzzer);
    delay(2000);
}

void challenge4Info(){
    finalChallengeActive = true;
    finalInputIndex = 0;
    lcd.clear();
    lcd.print("(a^2+b)c/x =12");

    tm.displayText("00000000");
    for(int i = 0; i < 4; i++){
        tm.displayASCII(i*2, '_');
    }
}

void checkFinalInput() {
  uint8_t buttons = tm.readButtons();
  
  for (int btn = 0; btn < 8; btn++) {
        if (buttons & (1 << btn)) {
            int num = btn + 1;
        
            finalUserInput[finalInputIndex] = num;
            tm.displayASCII(finalInputIndex*2, '0' + num);
            finalInputIndex++;

            tone(buzzer, NOTE_C5, 100);
            delay(100);
            noTone(buzzer);
        
            if (finalInputIndex >= 4) {
                verifyFinalSequence();
            }
        
            delay(250);
            while (tm.readButtons());
            break;
        }
  }
}

void verifyFinalSequence() {
    bool correct = true;
    for (int i = 0; i < 4; i++) {
        if (finalUserInput[i] != finalSequence[i]) {
        correct = false;
        break;
        }
    }

    if (correct) {
            tm.setLEDs(0b11111111);
            successSound();
            lcd.clear();
            lcd.print("Correct! 2,5,8,6");
            delay(3000);
            challenge = 5;
            everythingComplete();
    } else {
        for (int i = 0; i < 3; i++) {
            errorSound();
            tm.setLEDs(0b10101010);
            delay(300);
            tm.setLEDs(0b01010101);
            delay(300);
        }
        tm.setLEDs(0);
        challenge4Info();
    }
}

void everythingComplete(){
    finalChallengeActive = false;

    tm.setLEDs(0b11111111);
    lcd.clear();
    lcd.print("CONGRATULATIONS");
    lcd.setCursor(0, 1);
    lcd.print("puzzle solved");

    unsigned long startTime = millis();
    int melodyLength = sizeof(melody)/sizeof(melody[0]);
    int currentNote = 0;
  
    while (millis() - startTime < 10000) {
        int noteDuration = 1000 / noteDurations[currentNote];
        tone(buzzer, melody[currentNote], noteDuration);

        delay(noteDuration * 1.3);
        noTone(buzzer);
        
        currentNote = (currentNote + 1) % melodyLength;
  }

  noTone(buzzer);
  tm.setLEDs(0);
}

void setup() {
    lcd.init();
    lcd.clear();
    lcd.backlight();
    tm.displayBegin();

    pinMode(greenLed, OUTPUT);
    pinMode(redLed, OUTPUT);
    pinMode(buzzer, OUTPUT);

    countDownStart = millis();
    countdownActive = true;

    //start the 8x8 matrix
    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0); 
}

void loop() {
    switch(challenge){
        case 0:
            challengeInfo();
            lcd.print("first challenge:");
            lcd.setCursor(5, 1);
            lcd.print("PING PONG!");
            challenge = 1;
            
            break;
        case 1:
            updateCountdown();
            if(firstRun){
                lc.clearDisplay(0);
                pingPong.begin();
                firstRun = false;
            }
            if(!pingPong.isGameOver()) pingPong.update();
            else challenge = 2;
            break;
        case 2:
            updateCountdown();
            if(!challenge2Active){
                challenge2Info();
                challenge2Action();
            } else{
                pressBtn();
            }
            break;
        case 3:
            updateCountdown();
            if(!simonGameActive){
            challenge3Info();
                delay(2000);
                startSimonGame();
            } else{
                if(simonPlayingSequence) {
                    playSequence();
                } else{
                    checkUserInput();
                }
            }
            break;
        case 4:
            if(finalChallengeActive){
                checkFinalInput();
            } else{
                challenge4Info();
            }
            break;
    }


}
