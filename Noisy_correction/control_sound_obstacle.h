#ifndef CONTROL_SOUND_OBSTACLE_H_
#define CONTROL_SOUND_OBSTACLE_H_

#define SOUND				0
#define OBSTACLE			1

#define OFF					0
#define ON					1

#define SPEED_1				300
#define SPEED_2				400
#define SPEED_3				500
#define SPEED_4				600
#define TURN_SPEED			500

#define TIME_SLEEP_CONTROL	10

void control_start(void);
/*
*	start the thread that switch between the sound mode and obstacle mode
*/

#endif /* CONTROL_SOUND_OBSTACLE_H_ */
