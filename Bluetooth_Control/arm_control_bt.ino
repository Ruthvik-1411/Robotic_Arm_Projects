#include<Servo.h>           //Including library to handle servo functions

int pos_g=0,pos_l=0,pos_r=0,pos_b=0;        //Variables to store values from bluetooth corresponding to 4 joints respectively
int prev_g=50,prev_l=90,prev_r=45,prev_b=90;    //Default positions of servo motors 
String rx_data="";                            //String to store data coming from bluetooth module

int save_grip[47],save_left[47],save_right[47],save_base[47]; //Arrays to store the steps from mobile app, 47 steps can be stored in one go. (47 -> Hitman >< :) )
int step_no=0;        //Variable to help remeber the index of above array

int grip=11;          //Declaration of pins to which servos at four joints are attached
int left=10;
int right=9;
int base=8;

Servo gripper;        //Instances of servo for each joint 
Servo leftarm;
Servo rightarm;
Servo base_joint;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);         //begin serial communication at 9600
  pinMode(grip,OUTPUT);       //Declaring servo motors at joints as outputs
  pinMode(left,OUTPUT);
  pinMode(right,OUTPUT);
  pinMode(base,OUTPUT);
  gripper.attach(grip);       //Telling each servo instance that the respective servo motor is attached at this pin 
  leftarm.attach(left);
  rightarm.attach(right);
  base_joint.attach(base);
  gripper.write(prev_g);      //Making the robotic arm to be at its default position by giving the value of initial angles to servos
  leftarm.write(prev_l);
  rightarm.write(prev_r);
  base_joint.write(prev_b);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){                 //Read the serial buffer and if anything is available in the serial buffer then do this
    rx_data=Serial.readString();          //Read the entire data in string format and store in this variable
    Serial.println(rx_data);              //For debugging, can be omitted
    if(rx_data.startsWith("g")){                                                      //If the data starts with a g, then 
      grip_fun((rx_data.substring((rx_data.length())-2,rx_data.length())).toInt());   //read the last two elements of the string and convert this to integer and send it to the function that handles actuation of gripper joint
    }
     if(rx_data.startsWith("l")){                                                     //If the data starts with a l, then
      left_fun((rx_data.substring((rx_data.length())-2,rx_data.length())).toInt());   //read the last two elements of the string and convert this to integer and send it to the function that handles actuation of left i.e up/down joint
    }
     if(rx_data.startsWith("r")){                                                       //If the data starts with a r, then
      right_fun((rx_data.substring((rx_data.length())-2,rx_data.length())).toInt());    //read the last two elements of the string and convert this to integer and send it to the function that handles actuation of right i.e forward.backward joint
    }
     if(rx_data.startsWith("b")){                                                       //If the data starts with a b, then
      base_fun((rx_data.substring((rx_data.length())-2,rx_data.length())).toInt());     //read the last two elements of the string and convert this to integer and send it to the function that handles actuation of base i.e rotary joint
    }
    //Save, Run and Reset Actions
    if(rx_data.startsWith("Save")){         //If the data starts with 'Save', when you press send saved steps button in app, then 
      save_grip[step_no]=prev_g;            //store the value of present position which is obtained as the actuator functions are called
      save_left[step_no]=prev_l;
      save_right[step_no]=prev_r;
      save_base[step_no]=prev_b;
      step_no++;                  //Increase the step no so that next time you receive the save it can store in the array with next index
      delay(500);
    }
    if(rx_data.startsWith("Run")){        //If the data starts with 'Run', when you press Run saved steps button in app, then
      run_seq();                          //Call this function run_seq which takes care of running all the stored steps and pausing in between
    }
    if(rx_data.startsWith("Reset")){        //If the data starts with 'Reset', when you press reset button in app, then 
      step_no=0;                              //Re-initialize the value of steps to 0 so that next time save is pressed it startes storing from the first index
      memset(save_grip,0,sizeof(save_grip));      //Also make all the elements of the arrays to zero so they will be clean 
      memset(save_left,0,sizeof(save_left));
      memset(save_right,0,sizeof(save_right));
      memset(save_base,0,sizeof(save_base));
    }
  }
  else{                         //If nothing is there in serial buffer then print this. This condition can be omitted when operating without usb cable
    Serial.println("...");
    delay(2000);
  }
}

//Function to handle to actuation of gripper servo motor/joint
void grip_fun(int posg){
  posg=round(map(posg,0,99,50,130));      //The values recieved from the servo are in the range of 0-100 so map them to the range in which the servo is restricted to move
  if(posg==prev_g){                     //If the position to go in the previous step and present step are the same the do nothing
    delay(10);
  }
  if(posg>prev_g){                    //If the position to go in the present step is more than previous step eg: 50deg to 100degs now, then increase the previous angle until it becomes the present req angle
    for(int i=prev_g;i<=posg;i++){
      gripper.write(i);               //Also reflect this steady increase in angle at the servo so that a stable and slow motion can be achieved at the joint
      delay(10);
    }
  }
  if(posg<prev_g){                  //If the position to go in the previous step is more than present step eg: 100deg to 500degs now, then decrease the previous angle until it becomes the present req angle
    for(int i=prev_g;i>=posg;i--){
      gripper.write(i);               //Also reflect this steady decrease in angle at the servo so that a stable and slow motion can be achieved at the joint
      delay(10);
    }
  }
  prev_g=posg;                //make the present position as previous position so that it can be used to save the steps and anything else
  Serial.println(prev_g);
}

//Function to handle actuation of left servo motor/join that handles up/down movement
void left_fun(int posl){
  posl=round(map(posl,0,99,75,180));
  if(posl==prev_l){
    delay(10);
  }
  if(posl>prev_l){
    for(int i=prev_l;i<=posl;i++){
      leftarm.write(i);
      delay(10);
    }
  }
  if(posl<prev_l){
    for(int i=prev_l;i>=posl;i--){
      leftarm.write(i);
      delay(10);
    }
  }
  prev_l=posl;
  Serial.println(prev_l);
}

//Function to handle actuation of right servo motor/joint that handles forward/backward movement
void right_fun(int posr){
  posr=round(map(posr,0,99,0,135));
  if(posr==prev_r){
    delay(10);
  }
  if(posr>prev_r){
    for(int i=prev_r;i<=posr;i++){
      rightarm.write(i);
      delay(10);
    }
  }
  if(posr<prev_r){
    for(int i=prev_r;i>=posr;i--){
      rightarm.write(i);
      delay(10);
    }
  }
  prev_r=posr;
  Serial.println(prev_r);
}

//Function to handle actuation of base servo motor/joint that handle base rotary movement
void base_fun(int posb){
  posb=round(map(posb,0,99,0,180));
  if(posb==prev_b){
    delay(10);
  }
  if(posb>prev_b){
    for(int i=prev_b;i<=posb;i++){
      base_joint.write(i);;
      delay(10);
    }
  }
  if(posb<prev_b){
    for(int i=prev_b;i>=posb;i--){
      base_joint.write(i);
      delay(10);
    }
  }
  prev_b=posb;
  Serial.println(prev_b);
}

//Function that handles everything after Run saved steps button is pressed in the app
void run_seq(){
  while(rx_data!="Reset"){              //As long as the recieved data is not 'Reset' continue doing this
    for(int i=0;i<=step_no-1;i++){      //For each step that is stored in the array recieved from the app,
      if(Serial.available()>0){             //If new data is recieved then do this
        rx_data=Serial.readString();        //Read the new data and store it in this variable
        if(rx_data=="Pause"){               //If this data is 'Pause' then
          while(rx_data!="Run"){            //As long as the recieved data is not 'Run' continue doing this
            if(Serial.available()>0){       //Read again
              rx_data=Serial.readString();  //Store again
              if(rx_data=="Reset"){           //Check if its Reset, if it is then break out of the entire loop and go to above lines to execute code when reset is pressed
                break;        
              }           
            }
          }                               //If not then keep looking until its not Run
        }
      }
      //gripper
      if(save_grip[i]==save_grip[i+1]){         //Change position of gripper servo using the value of angle stored in arrays obtained from the app
        delay(10);
      }
      if(save_grip[i+1]>save_grip[i]){
        for(int j=save_grip[i];j<=save_grip[i+1];j++){
          gripper.write(j);
          delay(20);
        }
      }
      if(save_grip[i+1]<save_grip[i]){
        for(int j=save_grip[i];j>=save_grip[i+1];j--){
          gripper.write(j);
          delay(20);
        }
      }
      //Change position of left servo using the value of angle stored in arrays obtained from the app
      //left
      if(save_left[i]==save_left[i+1]){
        delay(10);
      }
      if(save_left[i+1]>save_left[i]){
        for(int j=save_left[i];j<=save_left[i+1];j++){
          leftarm.write(j);
          delay(20);
        }
      }
      if(save_left[i+1]<save_left[i]){
        for(int j=save_left[i];j>=save_left[i+1];j--){
          leftarm.write(j);;
          delay(20);
        }
      }
      //Change position of right servo using the value of angle stored in arrays obtained from the app
      //right
      if(save_right[i]==save_right[i+1]){
        delay(10);
      }
      if(save_right[i+1]>save_right[i]){
        for(int j=save_right[i];j<=save_right[i+1];j++){
          rightarm.write(j);
          delay(20);
        }
      }
      if(save_right[i+1]<save_right[i]){
        for(int j=save_right[i];j>=save_right[i+1];j--){
          rightarm.write(j);
          delay(20);
        }
      }
      //base
      //Change position of base servo using the value of angle stored in arrays obtained from the app
      if(save_base[i]==save_base[i+1]){
        delay(10);
      }
      if(save_base[i+1]>save_base[i]){
        for(int j=save_base[i];j<=save_base[i+1];j++){
          base_joint.write(j);
          delay(20);
        }
      }
      if(save_base[i+1]<save_base[i]){
        for(int j=save_base[i];j>=save_base[i+1];j--){
          base_joint.write(j);
          delay(20);
        }
      }
      delay(2000);
      //One step is executed here, i.e the position of 4 servos in this step are reached and now moving to the next step. This process acts like a sequence of playing steps
    }
  }
  //If reset is pressed then you know what to do!!
}
