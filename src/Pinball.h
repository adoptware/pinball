#define PBL_NULL						0x0
#define PBL_LOCK						0x1
#define PBL_BALL_1					0x2
#define PBL_BALL_2					0x4
#define PBL_BALL_3					0x8
#define PBL_BALL_4					0x10
#define PBL_WALLS						0x20	
#define PBL_WALLS_ONE				0x40
#define PBL_UNACTIVE_ARM		0x80
#define PBL_ACTIVE_ARM			0x100

#define PBL_BUMPER					0x400
#define PBL_TRAP_BOUNCE			0x800
#define PBL_GROUP1					0x1000
#define PBL_GROUP2					0x2000
#define PBL_GROUP3					0x4000
#define PBL_GROUP4					0x8000
#define PBL_TRAP            0x10000

#define PBL_TYPE_ARMBEH      1
#define PBL_TYPE_BOUNCEBEH   2
#define PBL_TYPE_BUMPERBEH   3
#define PBL_TYPE_CAVEBEH     4
#define PBL_TYPE_EYEBEH      5
#define PBL_TYPE_LOCKBEH     6
#define PBL_TYPE_STATEBEH    7
#define PBL_TYPE_SWITCHBEH   8
#define PBL_TYPE_TRIGGERBEH  9
#define PBL_TYPE_SCRIPTBEH   10
#define PBL_TYPE_FAKEMODULEBEH  11

#define PBL_SIG_NULL            -1

#define PBL_SIG_RESET_ALL				666
#define PBL_SIG_TILT            667

#define PBL_SIG_LNUDGE          671
#define PBL_SIG_RNUDGE          672
#define PBL_SIG_BNUDGE          673
#define PBL_SIG_TNUDGE          674
#define PBL_SIG_LEFTARM_ON      681
#define PBL_SIG_RIGHTARM_ON     682
#define PBL_SIG_MULTIBALL_OFF   691
#define PBL_SIG_EXTRABALL       692

// #define PBL_SIG__grp_type_act/un    (grp)(type)(0)(1=on,2=off)

#define PBL_SIG_LINUX_L_ON        1101          
#define PBL_SIG_LINUX_L_OFF       1102          
#define PBL_SIG_LINUX_I_ON        1201          
#define PBL_SIG_LINUX_I_OFF       1202          
#define PBL_SIG_LINUX_N_ON        1301          
#define PBL_SIG_LINUX_N_OFF       1302          
#define PBL_SIG_LINUX_U_ON        1401          
#define PBL_SIG_LINUX_U_OFF       1402          
#define PBL_SIG_LINUX_X_ON        1501          
#define PBL_SIG_LINUX_X_OFF       1502          
#define PBL_SIG_LINUX_ALL         1109

#define PBL_SIG_TUX_T_ON          1601          
#define PBL_SIG_TUX_T_OFF         1602          
#define PBL_SIG_TUX_U_ON          1701          
#define PBL_SIG_TUX_U_OFF         1702          
#define PBL_SIG_TUX_X_ON          1801          
#define PBL_SIG_TUX_X_OFF         1802          
#define PBL_SIG_TUX_ALL           1609

#define PBL_SIG_FACTOR_1          2101
#define PBL_SIG_FACTOR_2          2201
#define PBL_SIG_FACTOR_3          2301
#define PBL_SIG_FACTOR_4          2401
#define PBL_SIG_FACTOR_5          2501
#define PBL_SIG_FACTOR_6          2601
#define PBL_SIG_FACTOR_7          2701
#define PBL_SIG_FACTOR_8          2801
#define PBL_SIG_FACTOR_9          2901
#define PBL_SIG_FACTOR_10         2011

#define PBL_SIG_BALL1_ON          3101
#define PBL_SIG_BALL1_OFF         3102
#define PBL_SIG_BALL2_ON          3201
#define PBL_SIG_BALL2_OFF         3202
#define PBL_SIG_BALL3_ON          3301
#define PBL_SIG_BALL3_OFF         3302
#define PBL_SIG_BALL4_ON          3401
#define PBL_SIG_BALL4_OFF         3402
#define PBL_SIG_BALL_ALL_OFF      3109

#define PBL_SIG_LOOP_1_ON         4101
#define PBL_SIG_LOOP_1_OFF        4102
#define PBL_SIG_LOOP_2_ON         4201
#define PBL_SIG_LOOP_2_OFF        4202
#define PBL_SIG_LOOP_3_ON         4301
#define PBL_SIG_LOOP_3_OFF        4302
#define PBL_SIG_LOOP_4_ON         4401
#define PBL_SIG_LOOP_4_OFF        4402
#define PBL_SIG_LOOP_5_ON         4501
#define PBL_SIG_LOOP_5_OFF        4502

#define PBL_SIG_LEFT_STOPPER_ON   4301
#define PBL_SIG_LEFT_STOPPER_OFF  4302
#define PBL_SIG_RIGHT_STOPPER_ON  4401
#define PBL_SIG_RIGHT_STOPPER_OFF 4402

#define PBL_SIG_LOCK_1            5101
#define PBL_SIG_LOCK_2            5201
#define PBL_SIG_LOCK_3            5301
#define PBL_SIG_LOCK_4            5401
#define PBL_SIG_LOCK_RELEASE      5501
#define PBL_SIG_HEAD              5601

#define PBL_SIG_BOOT_B_ON         7101
#define PBL_SIG_BOOT_B_OFF        7102
#define PBL_SIG_BOOT_O_ON         7201
#define PBL_SIG_BOOT_O_OFF        7202
#define PBL_SIG_BOOT_2_ON         7301
#define PBL_SIG_BOOT_2_OFF        7302
#define PBL_SIG_BOOT_T_ON         7401
#define PBL_SIG_BOOT_T_OFF        7402
#define PBL_SIG_BOOT_ALL          7109

#define PBL_SIG_BUMPER_ON         9101

#define PBL_SIG_JACKPOT           9201

#define PBL_SIG_CAVE_ON           8101
#define PBL_SIG_CAVE_OFF          8102

/*

#define PBL_SIG_HIT_CAVE    						1002
#define PBL_SIG_CAVE_OUT 		    		1012
#define PBL_SIG_ACTIVATE_EXTRABALL 	1003
#define PBL_SIG_EXTRABALL						1013
#define PBL_SIG_JACKPOT							1004
#define PBL_SIG_NEW_TARGET					1060
#define PBL_SIG_TARGET_1 						1005
#define PBL_SIG_TARGET_2 						1006

#define PBL_SIG_LOCK_1 							2001
#define PBL_SIG_LOCK_2 							2002
#define PBL_SIG_ACTIVATE_LOCK  			2010
#define PBL_SIG_RELEASE_LOCK  			2020
#define PBL_SIG_NEW_LOCK    				2666

#define PBL_SIG_LEFT_LOOP   				3001
#define PBL_SIG_RIGHT_LOOP					3002
#define PBL_SIG_ACTIVATE_LEFT_LOOP	3011
#define PBL_SIG_ACTIVATE_RIGHT_LOOP	3012

#define PBL_SIG_ACTIVATE_BALL_1			4011
#define PBL_SIG_ACTIVATE_BALL_2			4012
#define PBL_SIG_ACTIVATE_BALL_3			4013
#define PBL_SIG_ACTIVATE_BALL_4			4014
#define PBL_SIG_BALL_1_DEAD					4021
#define PBL_SIG_BALL_2_DEAD					4022
#define PBL_SIG_BALL_3_DEAD					4023
#define PBL_SIG_BALL_4_DEAD					4024
#define PBL_SIG_MULTIBALL_DEAD			4666

#define PBL_SIG_MISSION_1						5001
#define PBL_SIG_MISSION_2						5002

#define PBL_LINUX_L_ON              6001

*/
