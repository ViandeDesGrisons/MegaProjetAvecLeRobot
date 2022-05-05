#ifndef AVOID_OBSTACLE_H_
#define AVOID_OBSTACLE_H_

#define DETECTION_DISTANCE 500 //distance minimum before the contact (find with testing)
#define SPEED_MOTOR        300

#define SENSOR_IR1         0
#define SENSOR_IR2         1
#define SENSOR_IR3         2
#define SENSOR_IR4         3
#define SENSOR_IR5         4
#define SENSOR_IR6         5
#define SENSOR_IR7         6
#define SENSOR_IR8         7
/**
 * Sensor position:
 *
 *      FRONT
 *       ###
 *    # 8   1 #
 *  # 7       2 #
 * #     TOP     #
 * #     VIEW    #
 *  # 6        3 #
 *    # 5   4 #
 *       ###
 *
 */
#define TRUE               1
#define FALSE              0

#define LEFT              -1
#define RIGHT			   1

#define QUARTER_TURN       90
#define FULL_TURN          360
#define STEPS_WHEEL_TURN   1000
#define CORRECTION_FACTOR  1

uint8_t find_obstacle(void);
void avoid_obstacle(void);

#endif /* AVOID_OBSTACLE_H_ */
