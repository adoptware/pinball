AUTOMAKE_OPTIONS = gnu

noinst_PROGRAMS = pinball

INCLUDES = -I../base -I../addon -I../visitor
LDADD = ../base/libemilia_base.a ../addon/libemilia_addon.a ../visitor/libemilia_visitor.a

pinball_SOURCES = \
ArmBehavior.cpp     BumperBehavior.h  Menu.cpp     Score.h \
ArmBehavior.h       CaveBehavior.cpp  Menu.h       SwitchBehavior.cpp \
BounceBehavior.cpp  CaveBehavior.h    Pinball.cpp  SwitchBehavior.h \
BounceBehavior.h    LockBehavior.cpp  Pinball.h    TriggerBehavior.cpp \
BumperBehavior.cpp  LockBehavior.h    Score.cpp    TriggerBehavior.h \
Loader.cpp Loader.h
