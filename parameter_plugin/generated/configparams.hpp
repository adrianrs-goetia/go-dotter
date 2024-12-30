#pragma once

#include <configHandler.h>

// CODE IS GENERATED FROM A CONFIG JSON -> file: project/config.json
// CHANGES WILL BE OVERWRITTEN
//
//
// EXAMPLE USE CASE:
//
//    ParamType::myvalue();
//    ParamType::nestedobject::mynestedvalue();
//
//    MyParam::NestedObject param;
//    param.mynestedvalue();


struct ConfigParam {
struct Camera{
inline static auto xMaxRotation() { return GETPARAMGLOBAL_F("camera", "xMaxRotation"); }
inline static auto xMinRotation() { return GETPARAMGLOBAL_F("camera", "xMinRotation"); }
struct Mnk{
inline static auto xInverted() { return GETPARAMGLOBAL_B("camera", "mnk", "xInverted"); }
inline static auto xMultiplier() { return GETPARAMGLOBAL_F("camera", "mnk", "xMultiplier"); }
inline static auto yInverted() { return GETPARAMGLOBAL_B("camera", "mnk", "yInverted"); }
inline static auto yMultiplier() { return GETPARAMGLOBAL_F("camera", "mnk", "yMultiplier"); }
}
//['camera']
  mnk;

struct Keyboardonly{
inline static auto xInverted() { return GETPARAMGLOBAL_B("camera", "keyboardOnly", "xInverted"); }
inline static auto xMultiplier() { return GETPARAMGLOBAL_F("camera", "keyboardOnly", "xMultiplier"); }
inline static auto yInverted() { return GETPARAMGLOBAL_B("camera", "keyboardOnly", "yInverted"); }
inline static auto yMultiplier() { return GETPARAMGLOBAL_F("camera", "keyboardOnly", "yMultiplier"); }
}
//['camera']
  keyboardOnly;

struct Joypad{
inline static auto xInverted() { return GETPARAMGLOBAL_B("camera", "joypad", "xInverted"); }
inline static auto xMultiplier() { return GETPARAMGLOBAL_F("camera", "joypad", "xMultiplier"); }
inline static auto yInverted() { return GETPARAMGLOBAL_B("camera", "joypad", "yInverted"); }
inline static auto yMultiplier() { return GETPARAMGLOBAL_F("camera", "joypad", "yMultiplier"); }
}
//['camera']
  joypad;

}
//[]
camera;

inline static auto gravityConstant() { return GETPARAMGLOBAL_F("gravityConstant"); }
struct Player{
struct Seameter{
inline static auto base() { return GETPARAMGLOBAL_I("player", "seameter", "base"); }
inline static auto max() { return GETPARAMGLOBAL_I("player", "seameter", "max"); }
}
//['player']
  seameter;

inline static auto walkSpeed() { return GETPARAMGLOBAL_F("player", "walkSpeed"); }
inline static auto sprintSpeed() { return GETPARAMGLOBAL_F("player", "sprintSpeed"); }
inline static auto coyoteframes() { return GETPARAMGLOBAL_I("player", "coyoteframes"); }
inline static auto floorMaxAngle() { return GETPARAMGLOBAL_F("player", "floorMaxAngle"); }
inline static auto terminalVelocity() { return GETPARAMGLOBAL_F("player", "terminalVelocity"); }
inline static auto sprintTimeScale() { return GETPARAMGLOBAL_F("player", "sprintTimeScale"); }
inline static auto onGroundAcceleration() { return GETPARAMGLOBAL_F("player", "onGroundAcceleration"); }
inline static auto onGroundDeceleration() { return GETPARAMGLOBAL_F("player", "onGroundDeceleration"); }
inline static auto jumpStrength() { return GETPARAMGLOBAL_F("player", "jumpStrength"); }
inline static auto inAirAcceleration() { return GETPARAMGLOBAL_F("player", "inAirAcceleration"); }
inline static auto inAirDeceleration() { return GETPARAMGLOBAL_F("player", "inAirDeceleration"); }
inline static auto gravityScale() { return GETPARAMGLOBAL_F("player", "gravityScale"); }
struct Animation{
inline static auto rootRotationSpeed() { return GETPARAMGLOBAL_F("player", "animation", "rootRotationSpeed"); }
inline static auto parryDrive() { return GETPARAMGLOBAL_I("player", "animation", "parryDrive"); }
inline static auto onGroundDrive() { return GETPARAMGLOBAL_I("player", "animation", "onGroundDrive"); }
inline static auto inAirDrive() { return GETPARAMGLOBAL_I("player", "animation", "inAirDrive"); }
inline static auto attackDrive() { return GETPARAMGLOBAL_I("player", "animation", "attackDrive"); }
inline static auto attackSpeed() { return GETPARAMGLOBAL_F("player", "animation", "attackSpeed"); }
}
//['player']
  animation;

struct Grapple{
inline static auto enabled() { return GETPARAMGLOBAL_B("player", "grapple", "enabled"); }
inline static auto launchStrength() { return GETPARAMGLOBAL_F("player", "grapple", "launchStrength"); }
}
//['player']
  grapple;

struct Parry{
struct Pre{
inline static auto cooldown() { return GETPARAMGLOBAL_F("player", "parry", "pre", "cooldown"); }
inline static auto timeout() { return GETPARAMGLOBAL_F("player", "parry", "pre", "timeout"); }
struct Onground{
struct Slide{
inline static auto basespeed() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "slide", "basespeed"); }
inline static auto inactivebasespeed() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "slide", "inactivebasespeed"); }
inline static auto acceleration() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "slide", "acceleration"); }
inline static auto deceleration() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "slide", "deceleration"); }
inline static auto turnspeed() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "slide", "turnspeed"); }
}
//['player', 'parry', 'pre', 'onground']
        slide;

inline static auto length() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "length"); }
inline static auto lift() { return GETPARAMGLOBAL_F("player", "parry", "pre", "onground", "lift"); }
}
//['player', 'parry', 'pre']
      onground;

struct Inair{
inline static auto length() { return GETPARAMGLOBAL_F("player", "parry", "pre", "inair", "length"); }
inline static auto lift() { return GETPARAMGLOBAL_F("player", "parry", "pre", "inair", "lift"); }
struct Impulse{
inline static auto xz() { return GETPARAMGLOBAL_F("player", "parry", "pre", "inair", "impulse", "xz"); }
inline static auto y() { return GETPARAMGLOBAL_F("player", "parry", "pre", "inair", "impulse", "y"); }
}
//['player', 'parry', 'pre', 'inair']
        impulse;

}
//['player', 'parry', 'pre']
      inair;

}
//['player', 'parry']
    pre;

struct Post{
inline static auto timeout() { return GETPARAMGLOBAL_F("player", "parry", "post", "timeout"); }
struct Inair{
inline static auto gravityScaleOverride() { return GETPARAMGLOBAL_F("player", "parry", "post", "inair", "gravityScaleOverride"); }
}
//['player', 'parry', 'post']
      inair;

}
//['player', 'parry']
    post;

struct Jump{
struct Impulse{
inline static auto xz() { return GETPARAMGLOBAL_F("player", "parry", "jump", "impulse", "xz"); }
inline static auto y() { return GETPARAMGLOBAL_F("player", "parry", "jump", "impulse", "y"); }
}
//['player', 'parry', 'jump']
      impulse;

inline static auto timeout() { return GETPARAMGLOBAL_F("player", "parry", "jump", "timeout"); }
}
//['player', 'parry']
    jump;

}
//['player']
  parry;

struct Attack{
inline static auto cooldown() { return GETPARAMGLOBAL_F("player", "attack", "cooldown"); }
inline static auto timeout() { return GETPARAMGLOBAL_F("player", "attack", "timeout"); }
inline static auto strength() { return GETPARAMGLOBAL_F("player", "attack", "strength"); }
inline static auto tmpRotation() { return GETPARAMGLOBAL_F("player", "attack", "tmpRotation"); }
inline static auto upAngle() { return GETPARAMGLOBAL_F("player", "attack", "upAngle"); }
}
//['player']
  attack;

}
//[]
player;

struct Npcs{
struct Turret{
inline static auto enabled() { return GETPARAMGLOBAL_B("npcs", "turret", "enabled"); }
inline static auto firingInterval() { return GETPARAMGLOBAL_F("npcs", "turret", "firingInterval"); }
inline static auto firingIntervalVariance() { return GETPARAMGLOBAL_F("npcs", "turret", "firingIntervalVariance"); }
inline static auto firingStrength() { return GETPARAMGLOBAL_F("npcs", "turret", "firingStrength"); }
}
//['npcs']
  turret;

struct Projectile{
inline static auto lifetime() { return GETPARAMGLOBAL_F("npcs", "projectile", "lifetime"); }
inline static auto postParryIntagibleTime() { return GETPARAMGLOBAL_F("npcs", "projectile", "postParryIntagibleTime"); }
}
//['npcs']
  projectile;

}
//[]
npcs;


};

