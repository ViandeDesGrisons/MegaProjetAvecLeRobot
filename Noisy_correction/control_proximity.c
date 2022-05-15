#include <control_proximity.h>
#include <sensors/proximity.h>
#include <avoid_obstacle.h>
#include "leds.h"
#include <chprintf.h> //A ENLEVER


//#define STACK_CHK_GUARD 0xe2dee396
//
//messagebus_t bus;
//MUTEX_DECL(bus_lock);
//CONDVAR_DECL(bus_condvar);

//=============================GLOBAL VARIABLES=============================
uint8_t activ_detection=OFF;
//=============================END GLOBAL VARIABLES=============================

//============================THREADS=============================
static THD_WORKING_AREA(waThdDetection, 128);
static THD_FUNCTION(ThdDetection, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
//    messagebus_init(&bus, &bus_lock, &bus_condvar);
//    messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");
//    proximity_msg_t proximity_values;
//    proximity_start();

    while(1){

    	  if (find_obstacle() && !activ_detection)
    	  {
    		activ_detection=ON;
    	  }
//    	  messagebus_topic_wait();
//    	  chprintf((BaseSequentialStream *)&SD3, "dans le thread ");
//    	  messagebus_topic_wait(proximity_topic, &proximity_values, sizeof(proximity_values));
    	  chThdSleepMilliseconds(50);
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
//	chprintf((BaseSequentialStream *)&SD3, "OUI ");
//	if (find_obstacle()) { //A ENLEVER
//		set_led(LED1, ON);
		set_led(LED3, ON);
		set_led(LED5, ON);
		set_led(LED7, ON);

		avoid_obstacle();
//		chprintf((BaseSequentialStream *)&SD3, "dans controle led motor ");
//	}
	clear_leds();
	set_speed_motor(STOP);
	activ_detection=OFF;
}
//=============================END EXTERNAL FUNCTIONS=============================
