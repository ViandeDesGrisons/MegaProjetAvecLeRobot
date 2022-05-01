#include <controle_proximity.h>
#include <sensors/proximity.h>
#include "motors.h"
#include "leds.h"


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

void controle_LED_MOTOR(void)
{
	if (find_obstacle()) {
		stop_motor();
		set_body_led(ON);
		set_front_led(ON);
	}else{
		//set_redemarer motor??
		set_body_led(OFF);
		set_front_led(OFF);
	}
}

