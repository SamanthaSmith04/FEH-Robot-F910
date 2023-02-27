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
#define RIGHT_MOTOR_PORT FEHMotor::Motor0
#define LEFT_MOTOR_PORT FEHMotor::Motor1
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P0_0
#define CDS_START_THRESHOLD 0 //ARBITRARY CDS VALUE
#define BUMP_SWITCH_PORT FEHIO::P0_1
#define CIRCUM (2*3.1415*3)
#define RIGHT_ENCODER_PORT FEHIO::P0_0
#define LEFT_ENCODER_PORT FEHIO::P0_0
#define RED 1 //Arbitrary Red Value
#define BLUE 1 //Arbitrary Blue Value

//COMPONENTS
DigitalEncoder right_encoder(RIGHT_ENCODER_PORT);
DigitalEncoder left_encoder(LEFT_ENCODER_PORT);
FEHMotor right_motor(RIGHT_MOTOR_PORT, 9.0);
FEHMotor left_motor(LEFT_MOTOR_PORT, 9.0);




//GLOBAL VARIABLES
AnalogInputPin cdsCell(CDS_SENSOR_PORT);

//FUNCTION HEADERS
void testCDS();
void checkPoint1Code();


int main() {

}

/*
Used for testing the CDS cell values and prints the value to the screen
USED FOR TESTING PURPOSES ONLY, NOT FOR CHECKING COLORS WHILE ROBOT IS RUNNING
*/
void testCDS(){
    AnalogInputPin cdsCell(FEHIO::P0_0);
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
void checkpoint1Code(){
    //ANY PRE-DRIVE SETUP STUFF

    //WAIT FOR START LIGHT
    while (cdsCell.Value() > CDS_START_THRESHOLD){}
    //GO
    //
}

/*
Drive function
@param percent
    the speed that the robot should move forward
    Negative for backward, Positive for forward
@param inches
    the distance that the robot will move
*/
void move(int percent, double inches) //using encoders
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
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts);

    //drop speed to 75% of max speed
    right_motor.SetPercent(percent * 0.9);
    left_motor.SetPercent(percent * 0.9);

    //While the average of the left and right encoder is less than the full distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);


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
    int counts = 230;
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
    while((right_encoder.Counts()<fullSpeedCounts));
    right_motor.SetPercent(percent * speedMultiplier);
    right_motor.SetPercent(percent * speedMultiplier * -1);
    while((right_encoder.Counts()<counts));


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
    int counts = 250;
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
    while((right_encoder.Counts()<fullSpeedCounts));
    right_motor.SetPercent(percent * speedMultiplier * -1);
    right_motor.SetPercent(percent * speedMultiplier);
    while((right_encoder.Counts()<counts));


    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}


