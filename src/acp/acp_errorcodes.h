#ifndef ACP_ERRORCODES_H_
#define ACP_ERRORCODES_H_


#define ERR_UN_OPCODE			  1 //unspecified opcode
#define ERR_ANA_OOR				  2 //analog port out of range
#define ERR_DIG_OOR				  3 //dig port out of range
#define ERR_MOTOR_OOR			  4 //motor port out of range
#define ERR_SERVO_OOR			  5 //servo port out of range
#define ERR_SERVO_OFF			  6 //servo is off
#define ERR_OPP_SUPP			 11 //operation not supported
#define ERR_INVALID_PAYLOAD		 12 //invalid payload length
#define ERR_TIMEOUT				 13 //time error
#define ERR_EXTPORT				 15 //external port error
#define ERR_UNSPECIFIED			255 //unspecified error


#endif /* ACP_ERRORCODES_H_ */
