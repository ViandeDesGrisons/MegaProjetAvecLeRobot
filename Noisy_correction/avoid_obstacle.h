#ifndef AVOID_OBSTACLE_H_
#define AVOID_OBSTACLE_H_

#define DETECTION_DISTANCE 500 //distance minimum before the contact (find with testing)
#define SPEED_MOTOR        250

#define SENSOR_IR1         0
#define SENSOR_IR2         1 //utile?
#define SENSOR_IR3         2
#define SENSOR_IR4         3
#define SENSOR_IR5         4
#define SENSOR_IR6         5
#define SENSOR_IR7         6 //utile?
#define SENSOR_IR8         7
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
#define TRUE               1
#define FALSE              0

#define LEFT              -1 //turn in counterclockwise
#define RIGHT			   1 //turn in clockwise

#define TIME_TO_TURN       2000

uint8_t find_obstacle(void);
void avoid_obstacle(void);
void turn_and_move(int8_t side);
//void quarter_turn(int8_t side);

#endif /* AVOID_OBSTACLE_H_ */
