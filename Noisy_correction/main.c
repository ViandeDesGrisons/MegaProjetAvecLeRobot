#include <stdlib.h>
#include <string.h>

#include <audio/microphone.h>
#include <audio_processing.h>
#include <control_sound_obstacle.h>
#include "hal.h"
#include <main.h>
#include "memory_protection.h"
#include <motors.h>
#include <sensors/proximity.h>

//#include "ch.h"
//#include <usbcfg.h>
//#include <chprintf.h>
//#include <i2c_bus.h>
//#include <msgbus/messagebus.h>
//#include <fft.h>
//#include <communications.h>
//#include <arm_math.h>
//#include <avoid_obstacle.h>
//#include "leds.h"

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


int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

    // Inits the Inter Process Communication bus.
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    //starts the serial communication
    serial_start();
    //inits the motors
    motors_init();
    //begin audio processing
    mic_start(&processAudioData);
    //inits the captor of proximity
    proximity_start();
    //calibration of ir sensors
    calibrate_ir();

    //time to inits and calibrate all
    chThdSleepMilliseconds(2000);

    //inits thread
    control_start();

    while (1) {
    }
}

