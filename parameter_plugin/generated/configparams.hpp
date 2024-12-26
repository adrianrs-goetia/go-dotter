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
inline static auto length() { return GETPARAMGLOBAL_F("player", "parry", "pre", "length"); }
inline static auto lift() { return GETPARAMGLOBAL_F("player", "parry", "pre", "lift"); }
}
//['player', 'parry']
    pre;

struct Post{
inline static auto freezetime() { return GETPARAMGLOBAL_F("player", "parry", "post", "freezetime"); }
inline static auto timeout() { return GETPARAMGLOBAL_F("player", "parry", "post", "timeout"); }
}
//['player', 'parry']
    post;

struct Jump{
inline static auto freezetime() { return GETPARAMGLOBAL_F("player", "parry", "jump", "freezetime"); }
inline static auto impulse() { return GETPARAMGLOBAL_F("player", "parry", "jump", "impulse"); }
inline static auto doubleJumpImpulse() { return GETPARAMGLOBAL_F("player", "parry", "jump", "doubleJumpImpulse"); }
inline static auto timeout() { return GETPARAMGLOBAL_F("player", "parry", "jump", "timeout"); }
inline static auto intagibilityTime() { return GETPARAMGLOBAL_F("player", "parry", "jump", "intagibilityTime"); }
inline static auto doubleJumpHorizontalStrength() { return GETPARAMGLOBAL_F("player", "parry", "jump", "doubleJumpHorizontalStrength"); }
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

