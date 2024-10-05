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
    "src/turretnode.cpp"
]
sourcefiles = [os.path.join(current_dir, f) for f in sourcefiles]
CPPPATH = ['include']

def configure_environment(env: SConsEnvironment, libs: list[str], args):
    e = env.Clone()
    e.VariantDir(build_dir, "gameplay_cpp/src", False)
    e.Append(CPPPATH=os.path.join(current_dir, 'include'))
    e["use_hot_reload"] = True # hot reload by default for gameplay_cpp
    lib_file = e.File("lib{}.{}.{}.{}".format(lib_name, env["platform"], env["target"], env["arch"]) + env["SHLIBSUFFIX"])
    e.SharedLibrary(
        target=lib_file,
        # Mapping source files to object through variant dir
        source=[os.path.join(build_dir, os.path.basename(src)) for src in sourcefiles],
    )
    e.Append(LIBS=libs)
    return lib_file