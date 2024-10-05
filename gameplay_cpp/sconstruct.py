from SCons.Script.SConscript import SConsEnvironment
from SCons.Variables import Variables, PathVariable
import os

project_name = "gameplay_cpp"
lib_name = "gameplay_cpp"
current_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = "bin/obj"
sourcefiles = [
    "src/camerapivot.cpp",
    "src/core.cpp",
    "src/grapplecomponent.cpp",
    "src/inputcomponent.cpp",
    "src/mainnode.cpp",
    "src/parrycomponent.cpp",
    "src/playernode.cpp",
    "src/playerstates.cpp",
    "src/register_types.cpp",
    "src/test/turretnode.cpp"
]
sourcefiles = [os.path.join(current_dir, f) for f in sourcefiles]

def get_object_files(env: SConsEnvironment):
    objs = []
    for src in sourcefiles:
        obj_target = os.path.join(build_dir, os.path.basename(src) + ".os")
        objs.append(env.Object(target=obj_target, source=src))
    return objs

def configure_environment(env: SConsEnvironment, libs: list[str], args):
    e = env.Clone()
    e.Append(CPPPATH=os.path.join(current_dir, 'include'))
    e["use_hot_reload"] = True # hot reload by default for gameplay_cpp
    lib_file = e.File("lib{}.{}.{}.{}".format(lib_name, env["platform"], env["target"], env["arch"]) + env["SHLIBSUFFIX"])
    e.SharedLibrary(
        target=lib_file,
        source=get_object_files(e),
    )
    e.Append(LIBS=libs)
    return lib_file