/***************************************************************************
                          Keyboard.h  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Private.h"

#ifndef ALLEGRO_H
#ifdef SDL_GRX_EM
#define KEY_A                 SDLK_a
#define KEY_B                 SDLK_b
#define KEY_C                 SDLK_c
#define KEY_D                 SDLK_d
#define KEY_E                 SDLK_e
#define KEY_F                 SDLK_f
#define KEY_G                 SDLK_g
#define KEY_H                 SDLK_h
#define KEY_I                 SDLK_i
#define KEY_J                 SDLK_j
#define KEY_K                 SDLK_k
#define KEY_L                 SDLK_l
#define KEY_M                 SDLK_m
#define KEY_N                 SDLK_n
#define KEY_O                 SDLK_o
#define KEY_P                 SDLK_p
#define KEY_Q                 SDLK_q
#define KEY_R                 SDLK_r
#define KEY_S                 SDLK_s
#define KEY_T                 SDLK_t
#define KEY_U                 SDLK_u
#define KEY_V                 SDLK_v
#define KEY_W                 SDLK_w
#define KEY_X                 SDLK_x
#define KEY_Y                 SDLK_y
#define KEY_Z                 SDLK_z
#define KEY_0                 SDLK_0
#define KEY_1                 SDLK_1
#define KEY_2                 SDLK_2
#define KEY_3                 SDLK_3
#define KEY_4                 SDLK_4
#define KEY_5                 SDLK_5
#define KEY_6                 SDLK_6
#define KEY_7                 SDLK_7
#define KEY_8                 SDLK_8
#define KEY_9                 SDLK_9
#define KEY_0_PAD             SDLK_KP0
#define KEY_1_PAD             SDLK_KP1
#define KEY_2_PAD             SDLK_KP2
#define KEY_3_PAD             SDLK_KP3
#define KEY_4_PAD             SDLK_KP4
#define KEY_5_PAD             SDLK_KP5
#define KEY_6_PAD             SDLK_KP6
#define KEY_7_PAD             SDLK_KP7
#define KEY_8_PAD             SDLK_KP8
#define KEY_9_PAD             SDLK_KP9
#define KEY_F1                SDLK_F1
#define KEY_F2                SDLK_F2
#define KEY_F3                SDLK_F3
#define KEY_F4                SDLK_F4
#define KEY_F5                SDLK_F5
#define KEY_F6                SDLK_F6
#define KEY_F7                SDLK_F7
#define KEY_F8                SDLK_F8
#define KEY_F9                SDLK_F9
#define KEY_F10               SDLK_F10
#define KEY_F11               SDLK_F11
#define KEY_F12               SDLK_F12
#define KEY_ESC               SDLK_ESCAPE
#define KEY_TILDE             511
#define KEY_MINUS             SDLK_MINUS
#define KEY_EQUALS            SDLK_EQUALS
#define KEY_BACKSPACE         SDLK_BACKSPACE
#define KEY_TAB               SDLK_TAB
#define KEY_OPENBRACE         SDLK_LEFTPAREN
#define KEY_CLOSEBRACE        SDLK_RIGHTPAREN
#define KEY_ENTER             SDLK_RETURN
#define KEY_COLON             SDLK_COLON
#define KEY_QUOTE             SDLK_BACKQUOTE
#define KEY_BACKSLASH         SDLK_BACKSLASH
#define KEY_BACKSLASH2        SDLK_BACKSLASH
#define KEY_COMMA             SDLK_COMMA
#define KEY_STOP              511
#define KEY_SLASH             SDLK_SLASH
#define KEY_SPACE             SDLK_SPACE
#define KEY_INSERT            SDLK_INSERT
#define KEY_DEL               SDLK_DEL
#define KEY_HOME              SDLK_HOME
#define KEY_END               SDLK_END
#define KEY_PGUP              SDLK_PAGEUP
#define KEY_PGDN              SDLK_PAGEDOWN
#define KEY_LEFT              SDLK_LEFT
#define KEY_RIGHT             SDLK_RIGHT
#define KEY_UP                SDLK_UP
#define KEY_DOWN              SDLK_DOWN
#define KEY_SLASH_PAD         SDLK_KP_DIVIDE
#define KEY_ASTERISK          SDLK_ASTERISK
#define KEY_MINUS_PAD         SDLK_KP_MINUS
#define KEY_PLUS_PAD          SDLK_KP_PLUS
#define KEY_DEL_PAD           SDLK_KP_DEL
#define KEY_ENTER_PAD         SDLK_KP_ENTER
#define KEY_PRTSCR            SDLK_PRINT
#define KEY_PAUSE             SDLK_BREAK
#define KEY_YEN               511
#define KEY_YEN2              511

#define KEY_MODIFIERS         511

#define KEY_LSHIFT            SDLK_LSHIFT
#define KEY_RSHIFT            SDLK_RSHIFT
#define KEY_LCONTROL          SDLK_LCTRL
#define KEY_RCONTROL          SDLK_RCTRL
#define KEY_ALT               SDLK_LALT
#define KEY_ALTGR             SDLK_RALT
#define KEY_LWIN              SDLK_LSUPER
#define KEY_RWIN              SDLK_RSUPER
#define KEY_MENU              SDLK_RMETA
#define KEY_SCRLOCK           SDLK_SCROLLOCK
#define KEY_NUMLOCK           SDLK_NUMLOCK
#define KEY_CAPSLOCK          SDLK_CAPSLOCK

#define KEY_MAX               512
#endif // SDL_GRX_EM
#endif // ALLEGRO_H

/**
  *@author Henrik Enqvist
  */

class Keyboard {
	public:
		Keyboard();
		~Keyboard();
		static void clear();
		static void poll();
		static bool isKeyDown(int piKey);
		static SDLKey waitForKey();
};

#endif // KEYBOARD_H
