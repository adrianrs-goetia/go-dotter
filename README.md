# GO-DOTTER 
## trial project -- 3rd person ps1 based MEDIEVIL thingy
### using Godot 4.2.2 
https://github.com/godotengine/godot/releases/download/4.2.2-stable/Godot_v4.2.2-stable_win64.exe.zip \
https://github.com/godotengine/godot/releases/download/4.2.2-stable/Godot_v4.2.2-stable_export_templates.tpz \
testing some stuff inside of Godot with GDExtensions for creating plugins. 

## Setup
### Linux
```
source scripts/setup_environment.sh
```

## Build
### SCons
```
scons target=editor|template_release|template_debug dev_build=yes
```

## GAME
3rd person
Canvas for playing around with modelling and physics
Whack object with stick
Parry mechanic -> parry tennis balls || nukes


## godot-cpp
### branch 4.2 https://github.com/godotengine/godot-cpp/tree/4.2?tab=readme-ov-file
Symbolic link your local godot-cpp clone, here in the root folder 


## Links to cool stuff or tutorials

#### GDExtension getting started
https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html#doc-gdextension-cpp-example

#### Building GDExtension / Plugin with cmake tutorial
https://thatonegamedev.com/cpp/cmake/godot-native-using-cmake/ \
https://www.machinemakesnoise.com/posts/how-to-start-making-gdextensions-for-godot/#fresh-gdextension 


#### stdout color
https://unix.stackexchange.com/questions/105568/how-can-i-list-the-available-color-names
https://ss64.com/nt/syntax-ansi.html


#### debug draw 3d
https://github.com/DmitriySalnikov/godot_debug_draw_3d


## Conventions
### Class Naming
CategoryContextFunction, e.g. ComponentsParryInstigator

Category:\
    Component: Entity, Module, Part, Unit, Block.\
    System: Manager, Processor, Controller, Orchestrator.\
    Event: Signal, Message, Action, Trigger.\

Context:\
    Parry: Deflect, Guard, Counter, Block.\
    Damage: Hit, Impact, Harm, Injury, Attack.\
    UI: Interface, Panel, Widget, HUD.\

Function:\
    Target: Receiver, Object, Goal, Focus.\
    Instigator: Trigger, Initiator, Source, Actor.\
    Handler: Manager, Processor, Delegate, Executor.\