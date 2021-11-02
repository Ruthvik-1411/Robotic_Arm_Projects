#include<Servo.h>

int pot1=A0;                                //Defining analog pins from the potentiometers
int pot2=A1;
int pot3=A2;
int pot4=A3;
int v1=0,v2=0,v3=0,v4=0,prev_r=0,prev_b=0;    //Variable to store the values from pots

int grip=11;      //Pins of 4 servo motors at different joints
int left=9;
int right=10;
int base=8;

Servo gripper;      //Instances for each servo at each joint
Servo leftarm;
Servo rightarm;
Servo sbase;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pot1,INPUT);      //Declaring status of pots as inputs and servos as outputs
  pinMode(pot2,INPUT);
  pinMode(pot3,INPUT);
  pinMode(pot4,INPUT);
  pinMode(grip,OUTPUT);
  pinMode(left,OUTPUT);
  pinMode(right,OUTPUT);
  pinMode(base,OUTPUT);
  gripper.attach(grip);     //Telling that this servo is attached at this pin
  leftarm.attach(left);
  rightarm.attach(right);
  sbase.attach(base);
  gripper.write(50);      //Initial positions of all servos reslting in a default position of robotic arm
  leftarm.write(90);
  rightarm.write(45);
  sbase.write(90);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  readpots();             //Read the value from the pots
  grip_fun(v1);           //Then send these values to functions that are made to handle specific servo motors as each motor has a limited angle to move. 
  left_fun(v2);
  right_fun(v3);
  base_fun(v4);
  delay(2000);
  v1=0;v2=0;v3=0;v4=0;    //Re initialize the values from pots to zero
}

//Function to read analog voltage values in the range 0-1024 representing 0-5v/3.3v 
void readpots(){
  int a1=0,a2=0,a3=0,a4=0,n=3;        //variables to store the calculations
  for(int i=0;i<n;i++){               //As the pots have fluctuations capacitors are added, but to be more stable the readings are taken 3 times and added
    a1=((analogRead(pot1))*(5/3.5));
    a2=((analogRead(pot2))*(5/3.5));
    a3=((analogRead(pot3))*(5/3.5));
    a4=((analogRead(pot4))*(5/3.5));
    v1+=a1;
    v2+=a2;
    v3+=a3;
    v4+=a4;
    delay(1000);
  }
  v1=round(v1/n);       //The average of those values is rounded and printed for reference. Printing can be omitted 
  v2=round(v2/n);
  v3=round(v3/n);
  v4=round(v4/n);
  Serial.print(v1);
  Serial.print(",");
  Serial.print(v2);
  Serial.print(",");
  Serial.print(v3);
  Serial.print(",");
  Serial.print(v4);
  delay(1000);      //As the values of v1,v2.. are global they are updated when this function is called
}

//Function to handle actuation of gripper joint
void grip_fun(int pos){
  pos=map(pos,0,1023,50,130);     //In my setup the movement is restricted to 50-130 degrees. So the value obtained from pots is mapped to this range.
  Serial.print("::");       //Print this value
  Serial.print(pos);
  delay(200);
  gripper.write(pos);   //Write this mapped value to the gripper servo motor
}

//Function to handle actuation of left servo i.e up/down motion
void left_fun(int pos){
  pos=map(pos,0,1023,75,180);   //In my setup the movement is restricted to 75-180 degrees. So the value obtained from pots is mapped to this range.
  Serial.print(",");
  Serial.print(pos);
  delay(200);
  leftarm.write(pos);       //Write this mapped value to the left servo motor
}

//Function to handle actuation of right servo motor i.e forward/backward motion
void right_fun(int posr){
  posr=map(posr,0,1023,0,135);  //In my setup the movement is restricted to 0-130 degrees. So the value obtained from pots is mapped to this range.
  Serial.print(",");
  Serial.print(posr);
  delay(200);                 //The range of movement is high for this joint. So sudden increase in value for any reason might cause damage to the stabilty of the arm. So to add a delay in movement the following is done
  if(posr>prev_r){            //If the value obtained(to go) now is higher than previous position in before step then go to the said position by increasing the previous angle until it reaches the required angle
    while((posr-prev_r)){
      rightarm.write(prev_r); //keep increasing this value at servo also
      prev_r++;
      delay(10);
    }
  }
  else if(posr<prev_r){       //If the value obtained(to go) now is lesser than previous position in before step then go to the said postion by decreasing the previous angle until it reaches the required angle
    while((prev_r-posr)!=0){
      rightarm.write(prev_r);     //keep decreasing this value at servo also
      prev_r--;
      delay(10);
    }
  }
  prev_r=posr;        //Make the present position as previous position to use in next step
}

//Function to handle actuation of base servo motor i.e rotary base joint
void base_fun(int pos){
  pos=map(pos,0,1023,0,180);    //Map the values to range of 0-180, a 360 deg servo can be used to have bigger workspace 
  Serial.print(",");
  Serial.println(pos);
  delay(200);       //Just like previous function the movement to reach the new position is delayed with 10 milliseconds adding stabiltiy
  if(pos>prev_b){
    while((pos-prev_b)){
      sbase.write(prev_b);
      prev_b++;
      delay(10);
    }
  }
  else if(pos<prev_b){
    while((prev_b-pos)!=0){
      sbase.write(prev_b);
      prev_b--;
      delay(10);
    }
  }
  prev_b=pos;
}
