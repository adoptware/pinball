#define PBL_NULL			0x0
#define PBL_LOCK			0x1
#define PBL_BALL	                0x2
#define PBL_WALLS			0x20	
#define PBL_WALLS_ONE			0x40
#define PBL_UNACTIVE_ARM		0x80
#define PBL_ACTIVE_ARM			0x100

#define PBL_BUMPER			0x400
#define PBL_TRAP_BOUNCE			0x800
#define PBL_GROUP1			0x1000
#define PBL_GROUP2			0x2000
#define PBL_GROUP3			0x4000
#define PBL_GROUP4			0x8000
#define PBL_TRAP                        0x10000
#define PBL_PLUNGER                     0x20000

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
#define PBL_TYPE_PLUNGERBEH  12

#define PBL_SIG_NULL            -1

#define PBL_SIG_RESET_ALL	666
#define PBL_SIG_TILT            667
#define PBL_SIG_GAME_OVER       668
#define PBL_SIG_GAME_START      669
#define PBL_SIG_GAME_PAUSE      670
#define PBL_SIG_TILT_WARNING    671
#define PBL_SIG_CLEAR_TEXT      672

#define PBL_SIG_LNUDGE          681
#define PBL_SIG_RNUDGE          682
#define PBL_SIG_BNUDGE          683
#define PBL_SIG_TNUDGE          684
#define PBL_SIG_LEFTARM_ON      691
#define PBL_SIG_RIGHTARM_ON     692
//#define PBL_SIG_MULTIBALL_OFF   771

// #define PBL_SIG__grp_type_act/un    (grp)(type)(0)(1=on,2=off)

#define PBL_SIG_BALL_ON          3101
#define PBL_SIG_BALL_OFF         3102
//#define PBL_SIG_ALLBALLS_OFF      3109

#define PBL_SIG_EXTRABALL						1013

/*

#define PBL_SIG_HIT_CAVE    				1002
#define PBL_SIG_CAVE_OUT 		    		1012
#define PBL_SIG_ACTIVATE_EXTRABALL 	1003
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
