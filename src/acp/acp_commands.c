#include "acp_commands.h"
#include "acp_opcodes.h"
#include "acp_handler.h"

acp_cmd_t acp_cmds[256] = {
	{NOP, 0, ACP_NO_PAYLOAD, acp_handler_nop},											//   0
	{ACP_UNUSED},																		//   1
	{ACP_UNUSED},																		//   2
	{ACP_UNUSED},																		//   3
	{ACP_UNUSED},																		//   4
	{ACP_UNUSED},																		//   5
	{ACP_UNUSED},																		//   6
	{ACP_UNUSED},																		//   7
	{ACP_UNUSED},																		//   8
	{ACP_UNUSED},																		//   9
	{ANALOG_SENSOR_REQUEST, 0, 1, acp_handler_analog_valueReq},							//  10
	{ACP_UNUSED},																		//  11
	{ANALOG_SENSOR_SUBSCRIPTION, ACP_VPL_FLAG, 0, acp_handler_analog_valueSub},			//  12
	{ACP_UNUSED},																		//  13
	{ANALOG_PULLUP_ACTION, ACP_VPL_FLAG, 0, acp_handler_analog_pullAct},				//  14
	{ACP_UNUSED},																		//  15
	{ACP_UNUSED},																		//  16
	{ACP_UNUSED},																		//  17
	{ACP_UNUSED},																		//  18
	{ACP_UNUSED},																		//  19
	{DIGITAL_SENSOR_REQUEST, 0, 1, acp_handler_digital_valueReq},						//  20
	{ACP_UNUSED},																		//  21
	{DIGITAL_SENSOR_SUBSCRIPTION, ACP_VPL_FLAG, 0, acp_handler_digital_valueSub},		//  22
	{ACP_UNUSED},																		//  23
	{DIGITAL_PULLUP_ACTION, ACP_VPL_FLAG, 0, acp_handler_digital_pullAct},				//  24
	{DIGITAL_OUTPUT_MODE_ACTION, ACP_VPL_FLAG, 0, not_implemented_handler},				//  25 TODO
	{DIGITAL_OUTPUT_LEVEL_ACTION, 0, 2, not_implemented_handler},						//  26 TODO
	{ACP_UNUSED},																		//  27
	{ACP_UNUSED},																		//  28
	{ACP_UNUSED},																		//  29
	{MOTOR_POWER_ACTION, 0, 3, acp_handler_motor_powerAct},								//  30
	{ACP_UNUSED},																		//  31
	{ACP_UNUSED},																		//  32
	{ACP_UNUSED},																		//  33
	{ACP_UNUSED},																		//  34
	{ACP_UNUSED},																		//  35
	{ACP_UNUSED},																		//  36
	{MOTOR_BRAKE_ACTION, 0, 2, acp_handler_motor_brakeAct},								//  37
	{MOTOR_OFF_ACTION, 0, 1, acp_handler_motor_offAct},									//  38
	{ACP_UNUSED},																		//  39
	{ACP_UNUSED},																		//  40
	{ACP_UNUSED},																		//  41
	{ACP_UNUSED},																		//  42
	{ACP_UNUSED},																		//  43
	{ACP_UNUSED},																		//  44
	{ACP_UNUSED},																		//  45
	{ACP_UNUSED},																		//  46
	{ACP_UNUSED},																		//  47
	{ACP_UNUSED},																		//  48
	{ACP_UNUSED},																		//  49
	{SERVO_ONOFF_ACTION, 0, 2, acp_handler_servo_onOffAct},								//  50
	{SERVO_POSITION_ACTION, 0, 2, acp_handler_servo_positionAct},						//  51
	{SERVO_POSITION_SYNC_ACTION, ACP_VPL_FLAG, 0, acp_handler_servo_positionSyncAct},	//  52
	{ACP_UNUSED},																		//  53
	{ACP_UNUSED},																		//  54
	{ACP_UNUSED},																		//  55
	{ACP_UNUSED},																		//  56
	{ACP_UNUSED},																		//  57
	{ACP_UNUSED},																		//  58
	{ACP_UNUSED},																		//  59
	{HWC_BATTERY_CHARGE_REQUEST, 0, ACP_NO_PAYLOAD, acp_handler_battery_chargeReq},	//  60
	{ACP_UNUSED},																		//  61
	{ACP_UNUSED},																		//  62
	{ACP_UNUSED},																		//  63
	{ACP_UNUSED},																		//  64
	{ACP_UNUSED},																		//  65
	{ACP_UNUSED},																		//  66
	{ACP_UNUSED},																		//  67
	{ACP_UNUSED},																		//  68
	{ACP_UNUSED},																		//  69
	{ACP_UNUSED},																		//  70
	{ACP_UNUSED},																		//  71
	{ACP_UNUSED},																		//  72
	{ACP_UNUSED},																		//  73
	{ACP_UNUSED},																		//  74
	{ACP_UNUSED},																		//  75
	{ACP_UNUSED},																		//  76
	{ACP_UNUSED},																		//  77
	{ACP_UNUSED},																		//  78
	{ACP_UNUSED},																		//  79
	{ACP_UNUSED},																		//  80
	{ACP_UNUSED},																		//  81
	{ACP_UNUSED},																		//  82
	{ACP_UNUSED},																		//  83
	{ACP_UNUSED},																		//  84
	{ACP_UNUSED},																		//  85
	{ACP_UNUSED},																		//  86
	{ACP_UNUSED},																		//  87
	{ACP_UNUSED},																		//  88
	{ACP_UNUSED},																		//  89
	{HWC_OFF_ACTION, 0, ACP_NO_PAYLOAD, acp_handler_HWC_offAct},						//  90
	{HWC_RESET_ACTION, 0, ACP_NO_PAYLOAD, acp_handler_HWC_resetAct},					//  91
	{ACP_UNUSED},																		//  92
	{ACP_UNUSED},																		//  93
	{ACP_UNUSED},																		//  94
	{ACP_UNUSED},																		//  95
	{ERROR_ACTION, 0, 2, not_implemented_handler},										//  96 TODO
	{ACP_UNUSED},																		//  97
	{ACP_UNUSED},																		//  98
	{ACP_UNUSED},																		//  99
	{ACP_UNUSED},																		// 100
	{ACP_UNUSED},																		// 101
	{ACP_UNUSED},																		// 102
	{ACP_UNUSED},																		// 103
	{ACP_UNUSED},																		// 104
	{ACP_UNUSED},																		// 105
	{ACP_UNUSED},																		// 106
	{ACP_UNUSED},																		// 107
	{ACP_UNUSED},																		// 108
	{ACP_UNUSED},																		// 109
	{HWC_TYPE_REQUEST, 0, ACP_NO_PAYLOAD, acp_handler_HWC_typeReq},						// 110
	{ACP_UNUSED},																		// 111
	{ACP_UNUSED},																		// 112
	{ACP_UNUSED},																		// 113
	{ACP_UNUSED},																		// 114
	{ACP_UNUSED},																		// 115
	{ACP_UNUSED},																		// 116
	{ACP_UNUSED},																		// 117
	{ACP_UNUSED},																		// 118
	{ACP_UNUSED},																		// 119
	{ACP_UNUSED},																		// 120
	{ACP_UNUSED},																		// 121
	{ACP_UNUSED},																		// 122
	{ACP_UNUSED},																		// 123
	{ACP_UNUSED},																		// 124
	{ACP_UNUSED},																		// 125
	{ACP_UNUSED},																		// 126
	{ACP_UNUSED},																		// 127
	{ACP_UNUSED},																		// 128
	{ACP_UNUSED},																		// 129
	{EXTERNAL_PORT_CONNECT_ACTION, 0, 2, not_implemented_handler},						// 130 TODO SPI?
	{EXTERNAL_PORT_SEND_ACTION, ACP_VPL_FLAG, 0, not_implemented_handler},				// 131 TODO SPI?
	{ACP_UNUSED},																		// 132
	{ACP_UNUSED},																		// 133
	{ACP_UNUSED},																		// 134
	{ACP_UNUSED},																		// 135
	{ACP_UNUSED},																		// 136
	{ACP_UNUSED},																		// 137
	{ACP_UNUSED},																		// 138
	{ACP_UNUSED},																		// 139
	{ACP_UNUSED},																		// 140
	{ACP_UNUSED},																		// 141
	{ACP_UNUSED},																		// 142
	{ACP_UNUSED},																		// 143
	{ACP_UNUSED},																		// 144
	{ACP_UNUSED},																		// 145
	{ACP_UNUSED},																		// 146
	{ACP_UNUSED},																		// 147
	{ACP_UNUSED},																		// 148
	{ACP_UNUSED},																		// 149
	{ACP_UNUSED},																		// 150
	{ACP_UNUSED},																		// 151
	{ACP_UNUSED},																		// 152
	{ACP_UNUSED},																		// 153
	{ACP_UNUSED},																		// 154
	{ACP_UNUSED},																		// 155
	{ACP_UNUSED},																		// 156
	{ACP_UNUSED},																		// 157
	{ACP_UNUSED},																		// 158
	{ACP_UNUSED},																		// 159
	{ACP_UNUSED},																		// 160
	{ACP_UNUSED},																		// 161
	{ACP_UNUSED},																		// 162
	{ACP_UNUSED},																		// 163
	{ACP_UNUSED},																		// 164
	{ACP_UNUSED},																		// 165
	{ACP_UNUSED},																		// 166
	{ACP_UNUSED},																		// 167
	{ACP_UNUSED},																		// 168
	{ACP_UNUSED},																		// 169
	{ACP_UNUSED},																		// 170
	{ACP_UNUSED},																		// 171
	{ACP_UNUSED},																		// 172
	{ACP_UNUSED},																		// 173
	{ACP_UNUSED},																		// 174
	{ACP_UNUSED},																		// 175
	{ACP_UNUSED},																		// 176
	{ACP_UNUSED},																		// 177
	{ACP_UNUSED},																		// 178
	{ACP_UNUSED},																		// 179
	{ACP_UNUSED},																		// 180
	{ACP_UNUSED},																		// 181
	{ACP_UNUSED},																		// 182
	{ACP_UNUSED},																		// 183
	{ACP_UNUSED},																		// 184
	{ACP_UNUSED},																		// 185
	{ACP_UNUSED},																		// 186
	{ACP_UNUSED},																		// 187
	{ACP_UNUSED},																		// 188
	{ACP_UNUSED},																		// 189
	{ACP_UNUSED},																		// 190
	{ACP_UNUSED},																		// 191
	{ACP_UNUSED},																		// 192
	{ACP_UNUSED},																		// 193
	{ACP_UNUSED},																		// 194
	{ACP_UNUSED},																		// 195
	{ACP_UNUSED},																		// 196
	{ACP_UNUSED},																		// 197
	{ACP_UNUSED},																		// 198
	{ACP_UNUSED},																		// 199
	{ACP_UNUSED},																		// 200
	{ACP_UNUSED},																		// 201
	{ACP_UNUSED},																		// 202
	{ACP_UNUSED},																		// 203
	{ACP_UNUSED},																		// 204
	{ACP_UNUSED},																		// 205
	{ACP_UNUSED},																		// 206
	{ACP_UNUSED},																		// 207
	{ACP_UNUSED},																		// 208
	{ACP_UNUSED},																		// 209
	{ACP_UNUSED},																		// 210
	{ACP_UNUSED},																		// 211
	{ACP_UNUSED},																		// 212
	{ACP_UNUSED},																		// 213
	{ACP_UNUSED},																		// 214
	{ACP_UNUSED},																		// 215
	{ACP_UNUSED},																		// 216
	{ACP_UNUSED},																		// 217
	{ACP_UNUSED},																		// 218
	{ACP_UNUSED},																		// 219
	{ACP_UNUSED},																		// 220
	{ACP_UNUSED},																		// 221
	{ACP_UNUSED},																		// 222
	{ACP_UNUSED},																		// 223
	{ACP_UNUSED},																		// 224
	{ACP_UNUSED},																		// 225
	{ACP_UNUSED},																		// 226
	{ACP_UNUSED},																		// 227
	{ACP_UNUSED},																		// 228
	{ACP_UNUSED},																		// 229
	{ACP_UNUSED},																		// 230
	{ACP_UNUSED},																		// 231
	{ACP_UNUSED},																		// 232
	{ACP_UNUSED},																		// 233
	{ACP_UNUSED},																		// 234
	{ACP_UNUSED},																		// 235
	{ACP_UNUSED},																		// 236
	{ACP_UNUSED},																		// 237
	{ACP_UNUSED},																		// 238
	{ACP_UNUSED},																		// 239
	{ACP_UNUSED},																		// 240
	{ACP_UNUSED},																		// 241
	{ACP_UNUSED},																		// 242
	{ACP_UNUSED},																		// 243
	{ACP_UNUSED},																		// 244
	{ACP_UNUSED},																		// 245
	{ACP_UNUSED},																		// 246
	{ACP_UNUSED},																		// 247
	{NOP2, 0, ACP_NO_PAYLOAD, acp_handler_nop},											// 248
	{ACP_UNUSED},																		// 249
	{ACP_UNUSED},																		// 250
	{ACP_UNUSED},																		// 251
	{ACP_UNUSED},																		// 252
	{ACP_UNUSED},																		// 253
	{ACP_UNUSED},																		// 254
	{ACP_UNUSED},																		// 255
};
