from SCons.Script.SConscript import SConsEnvironment
from SCons.Variables import Variables, PathVariable
import os
import glob

project_name = "gameplay_cpp"
lib_name = "gameplay_cpp"
current_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = "bin/obj"

def get_object_files(env: SConsEnvironment):
    # Just smooth brain glob all files in src/ and create object files within the build directory
    sources = glob.glob(os.path.join(current_dir, "src/**/**.cpp"), recursive=True)
    objs = []
    for src in sources:
        obj_target = os.path.join(build_dir, os.path.basename(src) + ".o")
        objs.append(env.Object(target=obj_target, source=src))
    return objs

def configure_environment(env: SConsEnvironment, libs: list[str], args):
    e = env.Clone()
    e.Append(CPPPATH=os.path.join(current_dir, 'src'))
    e["use_hot_reload"] = True # hot reload by default for gameplay_cpp
    lib_file = e.File("lib{}.{}.{}.{}".format(lib_name, env["platform"], env["target"], env["arch"]) + env["SHLIBSUFFIX"])
    
    e.SharedLibrary(
        target=lib_file,
        source=get_object_files(e),
    )
    e.Append(LIBS=libs)
    return lib_file