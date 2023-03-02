/* =========================================================================
*                       FEH Robot Project Group 910F
*
*Group Members: Searita Chen, Bennett Godinho-Nelson, Samantha Smith, John Ulm
*
*                             Robot Code
===========================================================================*/

//IMPORTS
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHIO.h>
#include <FEHUtility.h>

//CONSTANTS
#define RIGHT_MOTOR_PORT FEHMotor::Motor1 //
#define LEFT_MOTOR_PORT FEHMotor::Motor0 //
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P1_0
#define BUMP_SWITCH_PORT FEHIO::P0_1
#define CIRCUM (2*3.1415*3)
#define RIGHT_ENCODER_PORT FEHIO::P0_1 //
#define LEFT_ENCODER_PORT FEHIO::P0_0 //
#define RED_VALUE 0.1 //Arbitrary Red Value
#define BLUE_VALUE 1 //Arbitrary Blue Value
#define ROBOT_WIDTH 8.6
#define PI 3.14159

//COMPONENTS
DigitalEncoder right_encoder(RIGHT_ENCODER_PORT);
DigitalEncoder left_encoder(LEFT_ENCODER_PORT);
FEHMotor right_motor(RIGHT_MOTOR_PORT, 9.0);
FEHMotor left_motor(LEFT_MOTOR_PORT, 9.0);
AnalogInputPin cdsCell(CDS_SENSOR_PORT);

//GLOBAL VARIABLES
int x, y;

//FUNCTION HEADERS
void testCDS();
void checkPoint1Code();
void moveForward(int, double);
void moveBackward(int, double);
void stopDriving();
void rotateLeft(int, double);
void driveTest();
void rotateRight(int, double);


int main() {
    checkPoint1Code();
}

/*
DRIVING TEST CODE
*/
void driveTest(){
    int percent;
    LCD.SetBackgroundColor(BLACK);
    LCD.SetFontColor(RED);
    LCD.DrawRectangle(10, 10, 50, 50);
    LCD.WriteAt("25%", 11, 11);
    LCD.DrawRectangle(70, 10, 50, 50);
    LCD.WriteAt("50%", 71, 11);
    LCD.DrawRectangle(10, 70, 50, 50);
    LCD.WriteAt("75%", 11, 71);

    LCD.DrawRectangle(100, 100, 50, 50);
    LCD.WriteAt("CDS", 101, 101);

    while (!LCD.Touch(&x, &y)) {
        if (x >= 10 && x <= 60 && y >= 10 && y <= 60){
            percent = 25;
        }
        else if (x >= 70 && x <= 120 && y >= 10 && y <= 60){
            percent = 50;
        }
        else if (x >= 10 && x <= 60 && y >= 70 && y <= 120){
            percent = 75;
        }
        else if (x >= 100 && x <= 150 && y >= 100 && y <= 150){
            testCDS();
        }
    }
    //forward
    LCD.Write("Going forward 6in");
    moveForward(percent, 6);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //backward
    LCD.Write("Going backward 6in");
    moveBackward(percent, 6);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //turn right
    LCD.Write("Turning right 90 degrees");
    rotateRight(percent, 90);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //turn right
    LCD.Write("Turning right 90 degrees");
    rotateRight(percent, 90);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //turn left
    LCD.Write("Turning right 90 degrees");
    rotateLeft(percent, 90);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);
}


/*
Used for testing the CDS cell values and prints the value to the screen
USED FOR TESTING PURPOSES ONLY, NOT FOR CHECKING COLORS WHILE ROBOT IS RUNNING
*/
void testCDS(){
    while (1 == 1){
       LCD.Clear();
       LCD.Write(cdsCell.Value());
       Sleep(0.1);
   }
}

/*
The main drive code for checkpoint 1
Drives up the ramp, touches the kiosk, and drives back down the ramp
*/
void checkPoint1Code(){
    //ANY PRE-DRIVE SETUP STUFF
    int motor_percent = 40;
    double first_movement = 3;
    double first_turn = 33;
    double second_movement = 30;
    double second_turn = 45;
    double third_movement = 26;
    
    //WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE){}
    //GO
    //
    moveForward(motor_percent,first_movement);
    //turn slightly towards ramp
    rotateLeft(motor_percent, first_turn);
    //drive up ramp
    moveForward(motor_percent, second_movement);
    //turn towards boarding pass buttons
    rotateLeft(motor_percent, second_turn);
    //drive towards boarding pass buttons
    moveForward(motor_percent, third_movement);

    Sleep(0.5);
    //GO BACK
    //back up to ramp
    rotateRight(motor_percent, 35);
    moveBackward(motor_percent, 10);
    rotateLeft(motor_percent, 6);
    moveBackward(motor_percent, 30);
}
    

/*
Drive function
@param percent
    the speed that the robot should move forward
@param inches
    the distance that the robot will move
*/
void moveForward(int percent, double inches) //using encoders
{
    int counts = (int) ((inches/CIRCUM)*318);
    int fullSpeedCounts = (counts * 0.90);
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than 90% of the distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2);

    //drop speed to 75% of max speed
    right_motor.SetPercent(percent * 0.9);
    left_motor.SetPercent(percent * 0.9);

    //While the average of the left and right encoder is less than the full distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2);


    //Turn off motors
    stopDriving();
}

/*
Drive function
@param percent
    the speed that the robot should move backward
@param inches
    the distance that the robot will move
*/
void moveBackward(int percent, double inches) //using encoders
{
    int counts = (int) ((inches/CIRCUM)*318);
    int fullSpeedCounts = (counts * 0.90);
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(-1*percent);

    //While the average of the left and right encoder is less than 90% of the distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2);

    //drop speed to 75% of max speed
    right_motor.SetPercent(-1*percent * 0.9);
    left_motor.SetPercent(-1*percent * 0.9);

    //While the average of the left and right encoder is less than the full distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2){}


    //Turn off motors
    stopDriving();
}

void stopDriving(){
    right_motor.Stop();
    left_motor.Stop();
}

/*
Turn Right function
@param percent
    the speed that the robot should turn
@param degrees
    the angle that the robot will move
*/
void rotateLeft(int percent, double degrees){
    int counts = (ROBOT_WIDTH/2 * degrees * (PI/180) * 318)/ CIRCUM;
    double speedMultiplier = 0.9;
    int fullSpeedCounts = (counts * 0.90);


    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1*percent);


    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts()<fullSpeedCounts * 2));
    right_motor.SetPercent(percent * speedMultiplier);
    left_motor.SetPercent(percent * speedMultiplier * -1);
    while((right_encoder.Counts()<counts * 2));


    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
Turn Left function
@param percent
    the speed that the robot should turn
@param degrees
    the angle that the robot will move
*/
void rotateRight(int percent, double degrees){
    int counts = (ROBOT_WIDTH/2 * degrees * (PI/180) * 318)/ CIRCUM;
    double speedMultiplier = 0.9;
    int fullSpeedCounts = (counts * 0.90);


    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    //Set both motors to desired percent
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(percent);


    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts()<fullSpeedCounts * 2));
    right_motor.SetPercent(percent * speedMultiplier * -1);
    left_motor.SetPercent(percent * speedMultiplier);
    while((right_encoder.Counts()<counts * 2));


    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

