#ifndef IMU_H_
#define IMU_H_


#include <stdint.h>
#include <stdbool.h>


void imu_init();

int16_t imu_getRateX();
int16_t imu_getRateY();
int16_t imu_getRateZ();

int16_t imu_getAccelX();
int16_t imu_getAccelY();
int16_t imu_getAccelZ();

int16_t imu_getPoseX();
int16_t imu_getPoseY();
int16_t imu_getPoseZ();


#endif /* IMU_H_ */
