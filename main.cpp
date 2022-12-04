#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include "motordriver.h"
#include "DHT22.h"

DigitalOut greenLed(PA_13);
DigitalOut yellowLed(PB_10);
DigitalOut redLed(PA_4);
Motor motorA(D11, PC_8);
I2C myI2C(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c myGUI(myI2C, D13, 0x78, 64, 128);
Serial pc(USBTX,USBRX);
DHT22 dht22(PB_2);


//passward set
int ppw[4] = {-1, -1, -1, -1};
int pw[4] = {0,0,0,0};
int xpos[4] = {0, 31, 64, 96};
int password[4]={1,2,3,4};

// LED
Ticker ledRedTicker;
void taskRedLED() {
	redLed = 1;
	wait(0.1);
	redLed = 0;
}
Ticker ledgreenTicker;
void taskgreenLED() {
	greenLed = 1;
	wait(0.1);
	greenLed = 0;
}

#define YPOS 20

void displayPW() {
	bool oRefresh = false;
	myGUI.setTextSize(3);
	for (int i=0; i<4; i++) {
		if (pw[i] != ppw[i]) {
			myGUI.setTextCursor(xpos[i], YPOS);
			myGUI.writeChar(0x30 + pw[i]);
			ppw[i] = pw[i];
			
			oRefresh = true;
		}
	}
	if (oRefresh) myGUI.display();
}


// buzzer
DigitalOut buzzer(PC_9);
#define NOTE_C_DELAY   1915
Timeout buzzerTicker;
int buzzercnt = 0;
void taskBuzzerMove() {
	buzzercnt = 0;
	for (int i = 0; i < 200; i++) {
		if (buzzercnt == 0) {
		buzzer = 1;
		wait_us(NOTE_C_DELAY);
		buzzer = 0;
		wait_us(NOTE_C_DELAY);
		}
	}
	buzzercnt = 1;
}


//button
DigitalIn btn1(PA_14);
DigitalIn btn2(PB_7);
DigitalIn btn3(PC_4);
typedef enum{
	NO_EDGE=0, RISING_EDGE, FALLING_EDGE
} edge_t;


edge_t detectBtnEdge1(){
			static int prevState = 1;
			
	edge_t value = NO_EDGE;
	int currState = btn1;
	if(currState != prevState){
		if(currState ==0)	
		value = FALLING_EDGE;
			else
				value = RISING_EDGE;
	}
}
edge_t detectBtnEdge2(){
			static int prevState = 1;
			
	edge_t value = NO_EDGE;
	int currState = btn2;
	if(currState != prevState){
		if(currState ==0)	
		value = FALLING_EDGE;
			else
				value = RISING_EDGE;
	}
}
edge_t detectBtnEdge3(){
			static int prevState = 1;
			
	edge_t value = NO_EDGE;
	int currState = btn3;
	if(currState != prevState){
		if(currState ==0)	
		value = FALLING_EDGE;
			else
				value = RISING_EDGE;
	}
}


//joy stick
AnalogIn xasis(PC_2);
AnalogIn yasis(PC_3);
Ticker jsTicker;
int x,y;
#define JS_NEUTRAL_VALUE 55
void task1JoystickInput(){
	x =(int)(xasis * 100) - JS_NEUTRAL_VALUE;
	if(abs(x)<=2) x=0;
	y =(int)(yasis * 100) - JS_NEUTRAL_VALUE;
	if(abs(y)<=2) y=0;	
}


int cursorPos =0;
int prevCursorPos =-1;

#define CURSOR_YPOS 61
#define CURSOR_WIDTH 24
#define CURSOR_HEIGHT 2
void drawCursor() {
	if (cursorPos != prevCursorPos) {
		myGUI.fillRect(xpos[prevCursorPos]+1, CURSOR_YPOS,
						CURSOR_WIDTH, CURSOR_HEIGHT, 0);
		myGUI.fillRect(xpos[cursorPos]+1, CURSOR_YPOS,
						CURSOR_WIDTH, CURSOR_HEIGHT, 1);
		myGUI.display();
		
		prevCursorPos = cursorPos;
		
	}
}

Ticker cmTicker;
//change passward number and position
void task2CursorMove(){
	
		if(x<-10){
			yellowLed = 1;
			if(--cursorPos <0) cursorPos =3;
			wait(0.2);
		}
		else if (x>10){
			yellowLed = 1;
			if(++cursorPos >3) cursorPos = 0;
			wait(0.2);
		}
		int num = pw[cursorPos];
		if(y<-10 && x==0){
			yellowLed = 1;
			if(++num > 9)num=0;
			wait(0.2);
		}
		else if(y>10 && x==0){
			yellowLed = 1;
			if(--num<0) num =9;
			wait(0.2);
		}
		yellowLed = 0;
		pw[cursorPos] = num;
	}

// print
void drawerror() {
	myGUI.clearDisplay();
	for(int i = 0; i < 4; i++) {
		pw[i] = 0;
		ppw[i] = -1;
		myGUI.setTextCursor(10,20);
    myGUI.printf("error");
		myGUI.display();
	}
	wait(2.0);
	myGUI.clearDisplay();
	myGUI.display();
}
void drawsuccess() {
	myGUI.clearDisplay();
	for(int i = 0; i < 4; i++) {
		pw[i] = 0;
		ppw[i] = -1;
		myGUI.setTextCursor(10,20);
    myGUI.printf("succe");
		myGUI.display();
	}
	wait(2.0);
	myGUI.clearDisplay();
	myGUI.display();
}
void drawmanual(){
	myGUI.clearDisplay();
	for(int i = 0; i < 4; i++) {
		pw[i] = 0;
		ppw[i] = -1;
		myGUI.setTextCursor(10,20);
    myGUI.printf("manual");
		myGUI.display();
	}
	wait(2.0);
	myGUI.clearDisplay();
	myGUI.display();
}
void drawauto(){
	myGUI.clearDisplay();
	for(int i = 0; i < 4; i++) {
	pw[i] = 0;
	ppw[i] = -1;
	myGUI.setTextCursor(10,20);
  myGUI.printf("auto");
	myGUI.display();
	}
	wait(2.0);
	myGUI.clearDisplay();
	myGUI.display();
}


// moter
int passivity_count = 0;
int bt1_count = 0;
int bt2_count = 0;
Ticker moterTicker;
void taskMoterMove() {
	if (passivity_count == 1) {
		buzzerTicker.attach(&taskBuzzerMove, 1);
		ledgreenTicker.attach(&taskgreenLED, 0.2);
		motorA.backward(0.5);
		wait(1.0);
		motorA.stop();
		buzzerTicker.detach();
		ledgreenTicker.detach();
		passivity_count = 0;
		bt1_count = 0;
		bt2_count = 0;
	}
}
void dht(){
			if(dht22.sample()){
			float temp = dht22.getTemperature()/10.0f;
			float humidity = dht22.getHumidity()/10.0f;
			wait(1);
				myGUI.setTextSize(1);
				myGUI.setTextCursor(10,1);
        myGUI.printf("%.1f C\t %1.f%\r\n",temp,humidity);
				myGUI.display();
			if(passivity_count == 0){
				myGUI.setTextCursor(10,10);
        myGUI.printf("door:rock");
			}
			else{
				myGUI.setTextCursor(10,10);
        myGUI.printf("door:open");
			}
			}
}

int main() {
	myGUI.clearDisplay();
	myGUI.display();
	jsTicker.attach(&task1JoystickInput,0.4);
	cmTicker.attach(&task2CursorMove,0.4);
	while(1) {
					dht();
					displayPW();
					drawCursor();
		if (detectBtnEdge1() == FALLING_EDGE || detectBtnEdge2() == FALLING_EDGE || detectBtnEdge3() == FALLING_EDGE) {
			if (!btn1 && bt1_count == 0 && bt2_count == 0) {
				if (pw[0] == password[0] && pw[1] == password[1] && pw[2] == password[2] && pw[3] == password[3]) {
					bt1_count = 1;
					buzzerTicker.attach(&taskBuzzerMove, 1);
					ledgreenTicker.attach(&taskgreenLED, 0.2);
					motorA.forward(0.5);
					wait(1.0);
					motorA.stop();
					buzzerTicker.detach();
					ledgreenTicker.detach();
					passivity_count = 1;
					moterTicker.attach(&taskMoterMove, 30);
					drawsuccess();
				} else {
					ledRedTicker.attach(&taskRedLED, 0.2);
					drawerror();
					ledRedTicker.detach();
				}
			}
			if (!btn2) {
				if (passivity_count == 0 && bt1_count == 0 && bt2_count == 0) {
					ledgreenTicker.attach(&taskgreenLED, 0.2);
					buzzerTicker.attach(&taskBuzzerMove, 0.1);
					motorA.forward(0.5);
					wait(1.0);
					motorA.stop();
					ledgreenTicker.detach();
					buzzerTicker.detach();
					passivity_count = 1;
					bt2_count = 1;
					moterTicker.attach(&taskMoterMove, 30);

					drawmanual();
				} else if (passivity_count == 1 && bt1_count == 0 && bt2_count == 1) {
					buzzerTicker.attach(&taskBuzzerMove, 0.1);
					ledgreenTicker.attach(&taskgreenLED, 0.2);
					motorA.backward(0.5);
					wait(1.0);
					motorA.stop();
					ledgreenTicker.detach();
					buzzerTicker.detach();
					moterTicker.detach();
					passivity_count = 0;
					bt1_count = 0;
					bt2_count = 0;
					drawauto();
				} else {
					buzzerTicker.attach(&taskBuzzerMove, 0.1);
					ledgreenTicker.attach(&taskgreenLED, 0.2);
					motorA.backward(0.5);
					wait(1.0);
					motorA.stop();
					ledgreenTicker.detach();
					buzzerTicker.detach();
					moterTicker.detach();
					passivity_count = 0;
					bt1_count = 0;
					bt2_count = 0;
					drawauto();
				}
			}
				if(!btn3&&passivity_count == 1){
					ledRedTicker.attach(&taskRedLED, 0.2);
					wait(1.0);
						for(int i=0; i<4; i++){
							password[i] = pw[i];
						}
						ledRedTicker.detach();
						drawsuccess();
						passivity_count = 0;
						bt1_count = 0;
						bt2_count = 0;
					}
	}
}
	wait(0.2);
}