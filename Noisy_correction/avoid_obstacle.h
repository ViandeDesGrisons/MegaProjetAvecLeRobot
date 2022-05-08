#ifndef AVOID_OBSTACLE_H_
#define AVOID_OBSTACLE_H_

#define DETECTION_DISTANCE 			300 //distance minimum before the contact (find with testing)
#define DETECTION_DISTANCE_DIAGONAL 50
#define SPEED_MOTOR       			250
#define TURN_CORRECTION             180

#define SENSOR_IR1         			0
#define SENSOR_IR2         			1
#define SENSOR_IR3         			2
#define SENSOR_IR4         			3
#define SENSOR_IR5         			4
#define SENSOR_IR6         			5
#define SENSOR_IR7         			6
#define SENSOR_IR8         			7
/**
 * Sensor position:
 *
 *      FRONT
 *       ###
 *    #IR8  IR1#
 *  #IR7     IR2#
 * #             #
 *# IR6 TOP   IR3 #
 * #    VIEW      #
 *  #            #
 *    #IR5   IR4#
 *       ###
 *
 */
#define TRUE               			1
#define FALSE              			0

#define RIGHT			   			1 //turn in clockwise
#define LEFT              		   -1 //turn in counterclockwise

#define TIME_TO_TURN       			1600
#define FULL_ANGLE_DEG				360
#define STEP_ONE_TURN				1000
#define CORRECTION_FACTOR			1.3
#define POSITION_FOR_HALF_EPUCK		300
#define QUARTER_TURN_DEG			90
#define HALF_TURN_DEG				180

void set_speed_motor(uint8_t speed);
uint8_t find_obstacle(void);
void avoid_obstacle(void);
void quarter_turn(int8_t side); //A ENLEVER
void turn_and_move(int8_t side); //A ENLEVER
void motor_advance_half_epuck(void);
void motor_turn(float angle, int8_t side);
//uint8_t verify_front(void);
//uint8_t verify_left(void);
//uint8_t verify_back(void);
//uint8_t verify_right(void);
//void change_direction_sensor(uint8_t old_front, uint8_t old_right, uint8_t old_back, uint8_t old_left);

#endif /* AVOID_OBSTACLE_H_ */
