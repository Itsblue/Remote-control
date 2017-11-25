
#include <Servo.h>

Servo ForeAft;    // Define a Servo for Forward and Reverse Motion
Servo LeftRight; // Define a Servo for Left and Right Motion

int ForeAft_Pin = 0;
int LeftRight_Pin = 1;

float ForeAft_Input ;       // Range
float LeftRight_Input ;    // Range
float ForeAft_Output;       // Range 0 - 180 degrees
float LeftRight_Output;    // Range 0 - 180 degrees
int Fore_Limit = 800;       // High ADC Range of Joystick ForeAft
int Aft_Limit = 220;       // Low ADC Range of Joystick ForeAft
int Right_Limit = 800;     // High ADC Range of Joystick LeftRight
int Left_Limit = 226;      // Low ADC Range of Joystick LeftRight

void setup()               // Main Program Initialization
{
  Serial.begin(9600);

  ForeAft.attach(12);       // Servo Output 
  LeftRight.attach(13);    //  Servo Output

}

void loop()
{
  ForeAft_Input = analogRead(ForeAft_Pin) ;
  LeftRight_Input = analogRead(LeftRight_Pin) ;
  ForeAft_Output = convertToServo(ForeAft_Input) ;  
  LeftRight_Output = convertToServo(LeftRight_Input) ;

    ForeAft.write(ForeAft_Output);
    LeftRight.write(LeftRight_Output);


  //  Serial.print(LeftRight_Input);
  //  Serial.print(" ");
  Serial.print(ForeAft_Output);
  Serial.print(" ");
  Serial.print(LeftRight_Output);
  Serial.print(" ");

  Serial.println();

}


int convertToServo(float x){    // Function to convert between ADC and Servo Value
  float result;
  result = ((x-220)*0.31);
  return round(result);
}

int convertForeAftToServo(float y){
  int result;
 
  result = ((y-Aft_Limit)/(Fore_Limit-Aft_Limit)*180);
  return result;
}

int convertLeftRightToServo(float x){
  int result;
  
  result = ((x-Left_Limit)/(Right_Limit-Left_Limit)*180);
  return result;
}
