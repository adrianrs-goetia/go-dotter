# GDD -- Game Design Doc

## Overview

### Title
Parrier

### Elevator pitch
Play as a fragile cowboy doll in who wants to fortify his body. Deflect attacks from enemies, 'friendly' characters, and even the world itself on the way to the newest wonder of the wild westerns world. A hydrolic textile primer press. Capable of hardening even the softest of textiles into bullet proof materials. 
 
### Genre
Third person platformer \
Single player

### Selling Points
Fragile player character, but invincible with correct utilization of parry mechanic.  


## Core Gameplay

### Gameplay loop
Collect oils, textiles, seams and deadstock to power up the machinery blocking your way further. 

### Game Objective
#### Primary goals
Reach the end of the level in one piece. Get to the magnificent primer press for your salvation.
#### Secondary goals
Side quests from job posts detailing challenges _(e.g. timed, collect X amount, defeat these enemies)_ giving the player additional resources as a reward.

### Core mechanics
#### Walk & Run
Basic movement along the horizontal plane. 

#### Jump
A small jump giving the player a second in the air. Not meant for major traversals, but to allow for the player to gain the in-air state. Modifying their existing mechanics. 

#### Slice Attack
Swing your massive half scissor, damaging any enemy in front of you as well as cutting up projectiles for additional seameter.

#### Parry
Use the backend of your massive scissors to deflect ANY damage, be it enemy, fall damage or environment. \
On parrying the player will gain some seameter, and given the target objects tag/type the player can;
- Heavy_Dynamic: Will push the player away from the centre. 
- Light_Dynamic: Player will gain a boost in their desired horizontal direction and push the object in the opposite horizontal direction. Player will also receive a vertical boost.
- Static_Object: Player will bounce off it in their desired direction
- Static_Wall: Player will bounce off it given an angle from the surface normal. Limited to avoid going parallell with the wall. 


### Seam meter -- Seameter
Collect seams and dead stock to supercharge your abilities and gain access to some that are exclusively fueled by this. Filled by attacking enemies, parrying. 
Filling the seameter by parrying the player gains access to charged abilities such as;
- Bonus health: Spend seameter to recover health and gain bonus health if already full. The bonus health degrades when moving of committing any actions. As the extra textile rips at the seams. 
- Parry slice: Super parry, destroing any dynamic object and launching the player like it was a Light_Dynamic.

#### Grapplehook
isagrapplehook

## Story & Setting
### Intro
i morket, to headlights staar forran. \
Bossbar, immovable force, et tog kommer mot deg. Parrier den, skutt ut av tunnelen, tilted slik at den vinkler mot sola. Flyr opp mot den, MEN ... PARRY THE SUN \
Destroyed the real world or matrix moment? 


## Level design
Inspired by the mountanous areas of the dessert western, with wide canyons and deep valleys. Given the players vertical movement from the parry _(and grapplinghook)_, many of the areas will feature highly vertical maps which will give the player good room to flex their mechanical ability and put themselves in self imposed fall damage danger. \
Room for dancing in the air and dying from the result. 

Western desserts also feature larger flat areas to force different playstyles to emerge. 

## Art & Visual Design

## Audio Design
Freesound.org

## Technical specifications
Godot game engine

