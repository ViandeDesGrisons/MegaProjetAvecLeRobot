#ifndef AVOID_OBSTACLE_H_
#define AVOID_OBSTACLE_H_

#define DETECTION_DISTANCE 			300
#define DETECTION_DISTANCE_DIAGONAL 100
#define SPEED_MOTOR       			250


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

#define STEP_ONE_TURN				1000
#define POSITION_FOR_HALF_EPUCK		300
#define TURN_ADAPTATION_CORRECTION	180
#define TURN_MOTOR_CORRECTION		1.3

#define QUARTER_TURN_DEG			90
#define HALF_TURN_DEG				180
#define FULL_ANGLE_DEG				360


uint8_t find_obstacle(void);
void set_speed_motor(uint8_t speed);
void avoid_obstacle(void);

#endif /* AVOID_OBSTACLE_H_ */
