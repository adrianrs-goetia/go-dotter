from SCons.Script.SConscript import SConsEnvironment
from SCons.Variables import Variables, PathVariable
import os
import glob
import re

project_name = "gameplay_cpp"
lib_name = "gameplay_cpp"
current_dir = os.path.dirname(os.path.abspath(__file__))

def get_object_files(env: SConsEnvironment):
    # Just smooth brain glob all files in src/ and create object files within the build directory
    sources = glob.glob(os.path.join(current_dir, "src/**/**.cpp"), recursive=True)
    objs = []
    for src in sources:
        obj_target = os.path.join(env.get("obj_dir"), os.path.relpath(src) + ".o")
        objs.append(env.Object(target=obj_target, source=src))
    return objs

def get_library_version() -> str:
    with open("gameplay_cpp/version.h", "r") as f:
        header_content = f.read()

    major_match = re.search(r"#define .*_MAJOR (\d+)", header_content)
    minor_match = re.search(r"#define .*_MINOR (\d+)", header_content)
    patch_match = re.search(r"#define .*_PATCH (\d+)", header_content)

    major_value = int(major_match.group(1)) if major_match else 0
    minor_value = int(minor_match.group(1)) if minor_match else 0
    patch_value = int(patch_match.group(1)) if patch_match else 0

    return f"{major_value}.{minor_value}.{patch_value}" 

def get_libname_base() -> str:
    return f"lib{lib_name}_v{get_library_version()}"

def get_libname(env: SConsEnvironment, target: str) -> str:
    return "{}_{}.{}.{}".format(get_libname_base(), env["platform"], target, env["arch"]) + env["SHLIBSUFFIX"]

def generate_gdextension_file(env: SConsEnvironment):
    with open(f"{current_dir}/templates/gdextension", "r") as template:
        template_content = template.read()
        with open(f"{current_dir}/addons/gameplay_cpp/gameplaycpp.gdextension", "w+") as gdextension:
            gdextension.truncate(0)
            template_buffer = template_content
            template_buffer = re.sub(pattern="@libname_editor@", repl=f"{get_libname_base()}_", string=template_buffer)
            template_buffer = re.sub(pattern="@libname_debug@", repl=f"{get_libname_base()}_", string=template_buffer)
            template_buffer = re.sub(pattern="@libname_release@", repl=f"{get_libname_base()}_", string=template_buffer)
            gdextension.write(template_buffer)
            

def configure_environment(env: SConsEnvironment, libs: list[str], args):
    e = env.Clone()
    e.Append(CPPPATH=os.path.join(current_dir, 'src'))
    if e.get("is_msvc", False):
        e.Replace(CXXFLAGS=["/std:c++20"])
    else:
        e.Replace(CXXFLAGS=["-std=c++20"])
    #e.Append(CXXFLAGS=["-Wno-template-id-cdtor"])
    ##
    # Turned out to be unstable. Causing editor to forget parameters when releoading the library. 
    # e["use_hot_reload"] = True

    generate_gdextension_file(e)
    lib_file = e.File(get_libname(e, env["target"]))
    
    e.SharedLibrary(
        target=lib_file,
        source=get_object_files(e),
    )
    e.Append(LIBS=libs)
    return lib_file