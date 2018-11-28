#ifndef SERVO_H_
#define SERVO_H_

// HW_VERSION, set in Makefile
#ifndef HW_VERSION
	#error No HW_VERSION defined!
#elif ((HW_VERSION != 2) && (HW_VERSION != 3) && (HW_VERSION != 4))
	#error Defined HW_VERSION is not supported by this firmware!
#endif

//SW_VERSION
#define SW_VERSION 0

#endif /* SERVO_H_ */
