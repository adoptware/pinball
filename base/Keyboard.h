/***************************************************************************
                          Keyboard.h  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifndef PRIVATE_H
#error Must include Private.h before Keyboard.h
#endif

#if EM_USE_SDL
#include <SDL.h>
#define EMKey SDLKey
#endif // EM_USE_SDL

#if EM_USE_ALLEGRO
#include <allegro.h>

#define EMKey int

#define SDLK_a KEY_A
#define SDLK_b KEY_B
#define SDLK_c KEY_C
#define SDLK_d KEY_D
#define SDLK_e KEY_E
#define SDLK_f KEY_F
#define SDLK_g KEY_G
#define SDLK_h KEY_H
#define SDLK_i KEY_I
#define SDLK_j KEY_J
#define SDLK_k KEY_K
#define SDLK_l KEY_L
#define SDLK_m KEY_M
#define SDLK_n KEY_N
#define SDLK_o KEY_O
#define SDLK_p KEY_P
#define SDLK_q KEY_Q
#define SDLK_r KEY_R
#define SDLK_s KEY_S
#define SDLK_t KEY_T
#define SDLK_u KEY_U
#define SDLK_v KEY_V
#define SDLK_w KEY_W
#define SDLK_x KEY_X
#define SDLK_y KEY_Y
#define SDLK_z KEY_Z
#define SDLK_0 KEY_0
#define SDLK_1 KEY_1
#define SDLK_2 KEY_2
#define SDLK_3 KEY_3
#define SDLK_4 KEY_4
#define SDLK_5 KEY_5
#define SDLK_6 KEY_6
#define SDLK_7 KEY_7
#define SDLK_8 KEY_8
#define SDLK_9 KEY_9

#define SDLK_ESCAPE KEY_ESC

#define SDLK_RETURN KEY_ENTER

#define SDLK_SPACE KEY_SPACE
#define SDLK_INSERT KEY_INSERT
#define SDLK_DELETE KEY_DEL
#define SDLK_HOME KEY_HOME
#define SDLK_END KEY_END
#define SDLK_PAGEUP KEY_PGUP
#define SDLK_PAGEDOWN KEY_PGDN
#define SDLK_LEFT KEY_LEFT
#define SDLK_RIGHT KEY_RIGHT
#define SDLK_UP KEY_UP
#define SDLK_DOWN KEY_DOWN

#define SDLK_LSHIFT KEY_LSHIFT
#define SDLK_RSHIFT KEY_RSHIFT
#define SDLK_LCTRL KEY_LCONTROL
#define SDLK_RCTRL KEY_RCONTROL
#define SDLK_COMPOSE KEY_ALTGR

#endif // EM_USE_ALLEGRO

#ifndef KEY_MAX
#define KEY_MAX 512
#endif

/** @author Henrik Enqvist */

class Keyboard {
 public:
  Keyboard();
  ~Keyboard();
  static void clear();
  static void poll();
  static bool isKeyDown(int piKey);
  static EMKey waitForKey();
#if EM_USE_SDL
 private:
  static bool m_abKey[KEY_MAX];
#endif
};

#endif // KEYBOARD_H
