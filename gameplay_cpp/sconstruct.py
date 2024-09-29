from SCons.Script.SConscript import SConsEnvironment
from SCons.Variables import Variables, PathVariable
import os

project_name = "gameplay_cpp"
lib_name = "gameplay_cpp"
current_dir = os.path.dirname(os.path.abspath(__file__))
# default_output_dir = os.path.join(current_dir, "addons", "gameplay_cpp", "libs")
default_output_dir = os.path.join(os.path.dirname(current_dir), "project/addons/vendor/libs")
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

def configure_environment(env: SConsEnvironment, libs: list[str], args) -> None:
    current_cpppath: list = env["CPPPATH"]
    current_cpppath.append(os.path.join(current_dir, 'include'))
    env["CPPPATH"] = current_cpppath

    scons_cache_path = os.environ.get("SCONS_CACHE")
    if scons_cache_path is not None:
        env.CacheDir(scons_cache_path)
        env.Decider("MD5")

    lib_filename = "lib{}.{}.{}.{}".format(lib_name, env["platform"], env["target"], env["arch"]) + env["SHLIBSUFFIX"]

    opts = Variables([], args)
    opts.Add(
        PathVariable(
            "addon_output_dir", "Path to the output directory", default_output_dir, PathVariable.PathIsDirCreate
        )
    )
    opts.Update(env)
    env.Help(opts.GenerateHelpText(env))

    # env.Append(LIBS=libs)
    lib_filename = lib_name
    env.SharedLibrary(
        # target=env.File(os.path.join(env["addon_output_dir"], lib_filename)),
        target=lib_filename,
        source=src,
        LIBS=libs,
    )
    env.Append(LINKFLAGS=['-Wl,-z,defs'])