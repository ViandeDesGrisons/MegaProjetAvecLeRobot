#include <control_proximity.h>
#include <sensors/proximity.h>
#include <avoid_obstacle.h>
#include "leds.h"

//=============================GLOBAL VARIABLES=============================
uint8_t activ_detection=OFF;
//=============================END GLOBAL VARIABLES=============================

//============================THREADS=============================
static THD_WORKING_AREA(waThdDetection, 128);
static THD_FUNCTION(ThdDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

//     time;

    while(1){
    	  if (find_obstacle())
    	  {
    		  activ_detection=ON;
    	  }
//        messagebus_topic_wait();
    	  chThdSleepMilliseconds(1000);
    }
}
//============================END THREADS=============================

//=============================EXTERNAL FUNCTIONS=============================
void detection_proximity_start(void)
{
	chThdCreateStatic(waThdDetection, sizeof(waThdDetection), NORMALPRIO+1, ThdDetection, NULL);
}

void control_led_motor(void) //mettre dans partie dans partie de Romain
{
	if (find_obstacle()) {
		set_led(LED1, ON);
		set_led(LED3, ON);
		set_led(LED5, ON);
		set_led(LED7, ON);
		avoid_obstacle();
	}
	clear_leds();
	set_speed_motor(STOP);
}
//=============================END EXTERNAL FUNCTIONS=============================
