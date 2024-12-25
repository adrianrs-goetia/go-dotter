#pragma once
#include <configHandler.h>

// CODE IS GENERATED FROM A CONFIG JSON
// CHANGES WILL BE OVERWRITTEN

struct Param {
struct {
auto xMaxRotation() const { return GETPARAMGLOBAL_F("camera", "xMaxRotation"); }
auto xMinRotation() const { return GETPARAMGLOBAL_F("camera", "xMinRotation"); }
struct {
auto xInverted() const { return GETPARAMGLOBAL_I("camera", "mnk", "xInverted"); }
auto xMultiplier() const { return GETPARAMGLOBAL_F("camera", "mnk", "xMultiplier"); }
auto yInverted() const { return GETPARAMGLOBAL_I("camera", "mnk", "yInverted"); }
auto yMultiplier() const { return GETPARAMGLOBAL_F("camera", "mnk", "yMultiplier"); }
} mnk; 
struct {
auto xInverted() const { return GETPARAMGLOBAL_I("camera", "keyboardOnly", "xInverted"); }
auto xMultiplier() const { return GETPARAMGLOBAL_F("camera", "keyboardOnly", "xMultiplier"); }
auto yInverted() const { return GETPARAMGLOBAL_I("camera", "keyboardOnly", "yInverted"); }
auto yMultiplier() const { return GETPARAMGLOBAL_F("camera", "keyboardOnly", "yMultiplier"); }
} keyboardOnly; 
struct {
auto xInverted() const { return GETPARAMGLOBAL_I("camera", "joypad", "xInverted"); }
auto xMultiplier() const { return GETPARAMGLOBAL_F("camera", "joypad", "xMultiplier"); }
auto yInverted() const { return GETPARAMGLOBAL_I("camera", "joypad", "yInverted"); }
auto yMultiplier() const { return GETPARAMGLOBAL_F("camera", "joypad", "yMultiplier"); }
} joypad; 
} camera; 
auto gravityConstant() const { return GETPARAMGLOBAL_F("gravityConstant"); }
struct {
auto walkSpeed() const { return GETPARAMGLOBAL_F("player", "walkSpeed"); }
auto sprintSpeed() const { return GETPARAMGLOBAL_F("player", "sprintSpeed"); }
auto coyoteframes() const { return GETPARAMGLOBAL_I("player", "coyoteframes"); }
auto floorMaxAngle() const { return GETPARAMGLOBAL_F("player", "floorMaxAngle"); }
auto terminalVelocity() const { return GETPARAMGLOBAL_F("player", "terminalVelocity"); }
auto sprintTimeScale() const { return GETPARAMGLOBAL_F("player", "sprintTimeScale"); }
auto onGroundAcceleration() const { return GETPARAMGLOBAL_F("player", "onGroundAcceleration"); }
auto onGroundDeceleration() const { return GETPARAMGLOBAL_F("player", "onGroundDeceleration"); }
auto jumpStrength() const { return GETPARAMGLOBAL_F("player", "jumpStrength"); }
auto inAirAcceleration() const { return GETPARAMGLOBAL_F("player", "inAirAcceleration"); }
auto inAirDeceleration() const { return GETPARAMGLOBAL_F("player", "inAirDeceleration"); }
auto gravityScale() const { return GETPARAMGLOBAL_F("player", "gravityScale"); }
struct {
auto rootRotationSpeed() const { return GETPARAMGLOBAL_F("player", "animation", "rootRotationSpeed"); }
} animation; 
struct {
auto enabled() const { return GETPARAMGLOBAL_I("player", "grapple", "enabled"); }
auto launchStrength() const { return GETPARAMGLOBAL_F("player", "grapple", "launchStrength"); }
} grapple; 
struct {
struct {
auto cooldown() const { return GETPARAMGLOBAL_F("player", "parry", "pre", "cooldown"); }
auto stateTime() const { return GETPARAMGLOBAL_F("player", "parry", "pre", "stateTime"); }
auto length() const { return GETPARAMGLOBAL_F("player", "parry", "pre", "length"); }
auto lift() const { return GETPARAMGLOBAL_F("player", "parry", "pre", "lift"); }
} pre; 
struct {
auto freezetime() const { return GETPARAMGLOBAL_F("player", "parry", "post", "freezetime"); }
auto stateTime() const { return GETPARAMGLOBAL_F("player", "parry", "post", "stateTime"); }
} post; 
struct {
auto freezetime() const { return GETPARAMGLOBAL_F("player", "parry", "jump", "freezetime"); }
auto impulse() const { return GETPARAMGLOBAL_F("player", "parry", "jump", "impulse"); }
auto doubleJumpImpulse() const { return GETPARAMGLOBAL_F("player", "parry", "jump", "doubleJumpImpulse"); }
auto stateTime() const { return GETPARAMGLOBAL_F("player", "parry", "jump", "stateTime"); }
auto intagibilityTime() const { return GETPARAMGLOBAL_F("player", "parry", "jump", "intagibilityTime"); }
auto doubleJumpHorizontalStrength() const { return GETPARAMGLOBAL_F("player", "parry", "jump", "doubleJumpHorizontalStrength"); }
} jump; 
} parry; 
struct {
auto cooldown() const { return GETPARAMGLOBAL_F("player", "attack", "cooldown"); }
auto stateLength() const { return GETPARAMGLOBAL_F("player", "attack", "stateLength"); }
auto strength() const { return GETPARAMGLOBAL_F("player", "attack", "strength"); }
auto tmpRotation() const { return GETPARAMGLOBAL_F("player", "attack", "tmpRotation"); }
} attack; 
} player; 
struct {
struct {
auto enabled() const { return GETPARAMGLOBAL_I("npcs", "turret", "enabled"); }
auto firingInterval() const { return GETPARAMGLOBAL_F("npcs", "turret", "firingInterval"); }
auto firingIntervalVariance() const { return GETPARAMGLOBAL_F("npcs", "turret", "firingIntervalVariance"); }
auto firingStrength() const { return GETPARAMGLOBAL_F("npcs", "turret", "firingStrength"); }
} turret; 
struct {
auto lifetime() const { return GETPARAMGLOBAL_F("npcs", "projectile", "lifetime"); }
auto postParryIntagibleTime() const { return GETPARAMGLOBAL_F("npcs", "projectile", "postParryIntagibleTime"); }
} projectile; 
} npcs; 

} param;
