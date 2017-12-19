#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"

void hcp_handler_imu(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	int16_t x;
	int16_t y;
	int16_t z;

	if(opcode == HCP_IMU_RATE_REQ)
	{
		x = imu_getRateX();
		y = imu_getRateY();
		z = imu_getRateZ();
		ringbuffer_push(conn.txBuffer, HCP_IMU_RATE_REP);
	}
	else if(opcode == HCP_IMU_ACCEL_REQ)
	{
		x = imu_getAccelX();
		y = imu_getAccelY();
		z = imu_getAccelZ();
		ringbuffer_push(conn.txBuffer, HCP_IMU_ACCEL_REP);
	}
	else if(opcode == HCP_IMU_POSE_REQ)
	{
		x = imu_getPoseX();
		y = imu_getPoseY();
		z = imu_getPoseZ();
		ringbuffer_push(conn.txBuffer, HCP_IMU_POSE_REP);
	}

	ringbuffer_push(conn.txBuffer, (uint8_t)(x >> 8)); //high byte
	ringbuffer_push(conn.txBuffer, (uint8_t)x); //low byte
	ringbuffer_push(conn.txBuffer, (uint8_t)(y >> 8)); //high byte
	ringbuffer_push(conn.txBuffer, (uint8_t)y); //low byte
	ringbuffer_push(conn.txBuffer, (uint8_t)(z >> 8)); //high byte
	ringbuffer_push(conn.txBuffer, (uint8_t)z); //low byte
}
