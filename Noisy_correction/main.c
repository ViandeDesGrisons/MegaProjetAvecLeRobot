#include <stdio.h> //attention deja defini dans avoid obstacle
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include <control_proximity.h>
#include <i2c_bus.h>
#include <msgbus/messagebus.h>
#include <avoid_obstacle.h> //a enlever

#define STACK_CHK_GUARD 0xe2dee396

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

//static THD_FUNCTION(ThdProximity, arg) {
//
//    chRegSetThreadName(__FUNCTION__);
//    (void)arg;
//
////    systime_t time;
//
//    while(1){
////        time = chVTGetSystemTime();
//        palTogglePad(GPIOD, GPIOD_LED_FRONT);
//        chThdSleepUntilWindowed(time, time + MS2ST(10));
//    }
//}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
    serial_start();
    //start the USB communication
    usb_start();
    //starts the camera
//    dcmi_start();
	//initialises clock generation
//	po8030_start();
	//inits the motors
	motors_init();

	messagebus_init(&bus, &bus_lock, &bus_condvar);

	proximity_start();
//	calibrate_ir();

//	detection_start();
    /* Infinite loop. */
//	change_direction_sensor(1,2,3,4);
//	quarter_turn(-1);
//	turn_and_move(1);

//	quarter_turn(1);
//	motor_advance_half_epuck();
//	motor_turn(90,side);
    while (1) {
//    	chprintf((BaseSequentialStream *)&SD3, "set pos left = %d \n", left_motor_get_pos());
//    	chprintf((BaseSequentialStream *)&SD3, "set pos right = %d \n", right_motor_get_pos());
    	avoid_obstacle();
//    	chprintf((BaseSequentialStream *)&SD3, "get prox diag = %d \n", get_prox(SENSOR_IR2));
//    	chprintf((BaseSequentialStream *)&SD3, "verify left = %d \n", verify_left());
//    	chprintf((BaseSequentialStream *)&SD3, "verify right = %d \n", verify_right());
//    	chprintf((BaseSequentialStream *)&SD3, "verify back = %d \n", verify_back());
//    	chprintf((BaseSequentialStream *)&SD3, "verify front = %d \n", verify_front());
//    	chprintf((BaseSequentialStream *)&SD3, "get_prox = %d \n", get_prox(SENSOR_IR3));
    }

//        chThdSleepMilliseconds(1000);
}



