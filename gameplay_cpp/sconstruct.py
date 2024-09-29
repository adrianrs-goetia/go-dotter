from SCons.Script.SConscript import SConsEnvironment
from SCons.Variables import Variables, PathVariable
import os

project_name = "gameplay_cpp"
lib_name = "gameplay_cpp"
current_dir = os.path.dirname(os.path.abspath(__file__))
# default_output_dir = os.path.join(current_dir, "addons", "gameplay_cpp", "libs")
# default_output_dir = os.path.join(os.path.dirname(current_dir), "libs/")
src = [
    "src/camerapivot.cpp",
    "src/core.cpp",
    "src/grapplecomponent.cpp",
    "src/inputcomponent.cpp",
    "src/mainnode.cpp",
    "src/parrycomponent.cpp",
    "src/playernode.cpp",
    "src/playerstates.cpp",
    "src/register_types.cpp",
]
src = [os.path.join(current_dir, f) for f in src]
CPPPATH = ['include']

def configure_environment(env: SConsEnvironment, libs: list[str], args):
    current_cpppath: list = env["CPPPATH"]
    current_cpppath.append(os.path.join(current_dir, 'include'))
    env["CPPPATH"] = current_cpppath

    e = env.Clone()
    lib_file = e.File("lib{}.{}.{}.{}".format(lib_name, env["platform"], env["target"], env["arch"]) + env["SHLIBSUFFIX"])
    e.SharedLibrary(
        target=lib_file,
        source=src,
    )
    e.Append(LIBS=libs)
    return lib_file