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
#define LEFT_MOTOR_PORT FEHMotor::Motor0
#define LEFT_MOTOR_PORT FEHMotor::Motor1
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P0_0
#define BUMP_SWITCH_PORT FEHIO::P0_1
//Arbitrary Red Value
#define RED 1
#define CIRCUM (2*3.1415*3)


//GLOBAL VARIABLES


int main() {
AnalogInputPin cdsCell(CDS_SENSOR_PORT);

//Doesn't do anything until light Starts
while(cdsCell.Value() != RED){

}

}