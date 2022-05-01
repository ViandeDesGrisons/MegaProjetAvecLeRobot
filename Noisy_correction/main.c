#include <stdio.h>
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
#include <i2c_bus.h>
#include <msgbus/messagebus.h>
//#include <pi_regulator.h>
//#include <process_image.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

void delay(unsigned int n)
{
    while (n--) {
        __asm__ volatile ("nop");
    }
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
    dcmi_start();
	po8030_start();
	//inits the motors
	motors_init();

	//stars the threads for the pi regulator and the processing of the image
//	pi_regulator_start();
//	process_image_start();

	messagebus_init(&bus, &bus_lock, &bus_condvar);

	//mettre genre sleep de 1000ms pour être sûr

	proximity_start();
	calibrate_ir();

//    // Enable GPIOB and GPIOD peripheral clock for the LEDs
//    RCC->AHB1ENR    |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIODEN;
//
//    // FRONT_LED defined in main.h
//    gpio_config_output_opendrain(FRONT_LED);

    /* Infinite loop. */
    while (1) {
//    	delay(SystemCoreClock/16);
//    	gpio_toggle(FRONT_LED);
//    		chprintf((BaseSequentialStream *)&SD3, "calibration = %d \n", get_calibrated_prox(0));
    	controle_LED_MOTOR();

    }

//        chThdSleepMilliseconds(1000);
}

