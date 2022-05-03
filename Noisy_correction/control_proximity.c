#include <control_proximity.h>
#include <sensors/proximity.h>
#include "motors.h"
#include "leds.h"

//=============================INTERNAL FUNCTIONS=============================

uint8_t find_obstacle(void)
{
	if (get_prox(SENSOR_IR1) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR2) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR3) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR4) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR5) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR6) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR7) > DETECTION_DISTANCE ||
	   get_prox(SENSOR_IR8) > DETECTION_DISTANCE) {
		return TRUE;
	}
	return FALSE;
}

void stop_motor(void)
{
	left_motor_set_speed(0);
	right_motor_set_speed(0);
}

//=============================END INTERNAL FUNCTIONS=============================

//=============================EXTERNAL FUNCTIONS=============================

void control_led_motor(void)
{
	if (find_obstacle()) {
		stop_motor();
		set_led(LED1, ON);
		set_led(LED3, ON);
		set_led(LED5, ON);
		set_led(LED7, ON);
//		set_body_led(ON);
//		set_front_led(ON);
	}else{
		//set_redemarer motor??
		clear_leds();
//		set_body_led(OFF);
//		set_front_led(OFF);
	}
}

//=============================END EXTERNAL FUNCTIONS=============================
