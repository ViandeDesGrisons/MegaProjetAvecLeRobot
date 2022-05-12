#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <sensors/proximity.h>
#include <chprintf.h>
#include <control_proximity.h>
#include <i2c_bus.h>
#include <msgbus/messagebus.h>
#include <audio/microphone.h>
#include <audio_processing.h>
#include <fft.h>
#include <communications.h>
#include <arm_math.h>
#include <avoid_obstacle.h>
#include "leds.h"

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

//    messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");
//    proximity_msg_t proximity_values;

    //starts the serial communication
    serial_start();
    //inits the motors
    motors_init();
    //begin audio processing
    mic_start(&processAudioData);

    //inits the captor of proximity
    proximity_start();

    calibrate_ir();

    chThdSleepMilliseconds(2000);

    uint8_t mode = SOUND;
    //inits thread
//    detection_proximity_start();

    while (1) {

    	switch(mode){

    	case(SOUND):
			//move the robot if the max frequency detected is close to FREQ (406 Hz)
			if(max_norm_index >= FREQ_L && max_norm_index <= FREQ_H){

				//if the sound is coming from the left, turn right
				if(Left_Phase < Right_Phase - PHASE_THRESHOLD){
					left_motor_set_speed(-SPEED_TURN);
					right_motor_set_speed(SPEED_TURN);
					set_body_led(1);			//to turn on the body led when the robot is moving
				}

				//if the sound is coming from the right, turn left
				if(Right_Phase < Left_Phase - PHASE_THRESHOLD){
					left_motor_set_speed(SPEED_TURN);
					right_motor_set_speed(-SPEED_TURN);
					set_body_led(1);			//to turn on the body led when the robot is moving
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
				Left_Phase = 0;
				Right_Phase = 0;
				Front_Phase = 0;
				Back_Phase = 0;


			//If the max frequency detected is not close to FREQ, the robot is not moving
			}else{
				left_motor_set_speed(0);
				right_motor_set_speed(0);
				set_body_led(0);
			}

			if (find_obstacle())
			{
				mode=OBSTACLE;
			}
    		break;

    	case(OBSTACLE):
				set_body_led(0);
				control_led_motor();
				mode=SOUND;
    			break;
    	}
    	chThdSleepMilliseconds(50);
    }
}

