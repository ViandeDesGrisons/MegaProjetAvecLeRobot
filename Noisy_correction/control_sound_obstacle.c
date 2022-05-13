#include <audio_processing.h>
#include <arm_math.h>
#include <avoid_obstacle.h>
#include <control_sound_obstacle.h>
#include "leds.h"
#include <motors.h>
#include <sensors/proximity.h>

/***************************INTERNAL FUNCTIONS************************************/
void control_motor_sound(void)
{
	//move the robot if the max frequency detected is close to FREQ (406 Hz)
	if(max_norm_index >= FREQ_L && max_norm_index <= FREQ_H)
	{
		//if the sound is coming from the left, turn right
		if(Left_Phase < Right_Phase - PHASE_THRESHOLD){
			left_motor_set_speed(-SPEED_TURN);
			right_motor_set_speed(SPEED_TURN);
			set_body_led(ON);			//to turn on the body led when the robot is moving
		}

		//if the sound is coming from the right, turn left
		if(Right_Phase < Left_Phase - PHASE_THRESHOLD){
			left_motor_set_speed(SPEED_TURN);
			right_motor_set_speed(-SPEED_TURN);
			set_body_led(ON);			//to turn on the body led when the robot is moving
		}

		//if the robot is centered, go frontward or backward depending from where the sound is coming
		if(fabs(Right_Phase - Left_Phase) <= PHASE_THRESHOLD){

			if(Front_Phase < Back_Phase - PHASE_THRESHOLD){
				left_motor_set_speed(SPEED_FORWARD);
				right_motor_set_speed(SPEED_FORWARD);
				set_body_led(1);		//to turn on the body led when the robot is moving
			}

			if(Back_Phase < Front_Phase - PHASE_THRESHOLD){
				left_motor_set_speed(-SPEED_FORWARD);
				right_motor_set_speed(-SPEED_FORWARD);
				set_body_led(1);		//to turn on the body led when the robot is moving
			}
		}
		//When the average is done, reset the phases.
		Left_Phase = CLEAR;
		Right_Phase = CLEAR;
		Front_Phase = CLEAR;
		Back_Phase = CLEAR;

	//If the max frequency detected is not close to FREQ, the robot is not moving
	}else{
		set_speed_motor(STOP);
		set_body_led(OFF);
	}
}
void control_motor_obstacle(void)
{
	set_led(LED1, ON);
	set_led(LED3, ON);
	set_led(LED5, ON);
	set_led(LED7, ON);
	avoid_obstacle();
	clear_leds();
	set_speed_motor(STOP);
}
/*************************END INTERNAL FUNCTIONS**********************************/

/**********************************THREADS****************************************/
static THD_WORKING_AREA(waThdControl, 128);
static THD_FUNCTION(ThdControl, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t mode = SOUND;
    while(1){
    	switch(mode){

    	case(SOUND):
			control_motor_sound();
			if (find_obstacle())
			{
				mode=OBSTACLE;
			}
    		break;

    	case(OBSTACLE):
				set_body_led(OFF);
    			control_motor_obstacle();
				mode=SOUND;
    			break;
    	}
    	chThdSleepMilliseconds(50);
    }
}
/********************************END THREADS**************************************/

/****************************PUBLIC FUNCTIONS*************************************/
void control_start(void)
{
	chThdCreateStatic(waThdControl, sizeof(waThdControl), NORMALPRIO, ThdControl, NULL);
}
/**************************END PUBLIC FUNCTIONS***********************************/
