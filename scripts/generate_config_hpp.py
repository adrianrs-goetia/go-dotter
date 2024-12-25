import json
import enum
import os
import re
from argparse import ArgumentParser
import sys

class CodeGenerator():
    def __init__(self):
        self.context_stack: list[str] = []
        self.buffer: str = ""
        self.name: str = ""
    
    def start(self, filepath: str) -> str:
        f = filepath.split('/')[-1]
        self.name = f.split('.')[0].capitalize()
        
        s = "#pragma once\n"
        s += "#include <configHandler.h>\n"
        s += "\n"
        s += "// CODE IS GENERATED FROM A CONFIG JSON\n"
        s += "// CHANGES WILL BE OVERWRITTEN\n"
        s += "//\n"
        s += "// EXAMPLE USE CASE:\n"
        s += "//\n"
        s += "//    param.myvalue();\n"
        s += "//    param.nestedobject.mynestedvalue();\n"
        s += "//\n"
        s += "//    const MyParam::NestedObject& param = param.nestedobject;\n"
        s += "//    param.mynestedvalue();\n"
        s += "\n"
        s += "\n"
        s += f"struct {self.name}Param {{\n"
        self.buffer += s

    def end(self) -> str:
        self.buffer +=  f"\n}}\n {self.name.lower()}param;\n\n"
    
    def push_object(self, key: str) -> str:
        self.context_stack.append(key)
        self.buffer +=  f"struct {key.capitalize()}{{\n"
    
    def pop(self) -> str:
        if not self.context_stack:
            raise Exception("Cannot pop empty stack")
        last = self.context_stack.pop()
        space = '  ' * len(self.context_stack)
        comment = [k for k in self.context_stack]
        self.buffer +=  f"}}\n//{comment}\n{space}{last};\n\n"
    
    def add_value(self, key, val):
        suffix = '#'
        if isinstance(val, int):
            suffix = 'I'
        elif isinstance(val, bool):
            suffix = 'B'
        elif isinstance(val, float):
            suffix = 'F'
        elif isinstance(val, str):
            suffix = 'S'
        else:
            raise Exception(f"Wrong type for add_value: {type(val)}")
        
        config_key: str = ""
        self.context_stack.append(key)
        for i, k in enumerate(self.context_stack):
            config_key += f"\"{str(k)}\""
            if i < len(self.context_stack) - 1:
                config_key += ", "
        self.context_stack.pop()

        self.buffer += f"auto {key}() const {{ return GETPARAMGLOBAL_{suffix}({config_key}); }}\n"
    

def generate_hpp_from_json(json_file: str, output_file: str):
    print("Generate hpp from json")

    # create hpp file
    if str(output_file).split('.')[1] != 'hpp':
        print("Expected generated file to have hpp suffix")
        sys.exit(1)
    dirs = os.path.dirname(output_file)
    os.makedirs(dirs, exist_ok=True)
    with open(output_file, 'w') as hppfile:

        ##
        # parse json
        def remove_comments(json_string) -> str:
            json_string = re.sub(r"//.*", "", json_string)
            json_string = re.sub(r"/\*.*?\*/.*", "", json_string, flags=re.DOTALL)
            return json_string

        try:
            with open(json_file, 'r') as f:
                data = json.loads(remove_comments(f.read()))

                codegen = CodeGenerator()

                def _read_object(key, obj):
                    codegen.push_object(key)
                    for k2, v2 in obj.items():
                        if isinstance(v2, dict):
                            _read_object(k2, v2)
                            continue
                        codegen.add_value(k2, v2)
                    codegen.pop()

                codegen.start(json_file)
                for key, value in data.items():
                    if isinstance(value, dict):
                        _read_object(key, value)
                        continue
                    codegen.add_value(key, value)
                codegen.end()

                hppfile.write(codegen.buffer)
        except FileNotFoundError:
            print(f"Error: File '{json_file}' not found.")
            sys.exit(2)
        except json.JSONDecodeError as e:
            print(f"Error: Invalid JSON format in file '{json_file}': {e}")
            sys.exit(3)
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
            sys.exit(4)

if __name__ == "__main__":
    # always run script from project root
    if os.getcwd().split('/')[-1] != "go-dotter":
        print("Run this from project root: /go-dotter")
        sys.exit(1)

    # get args
    parser = ArgumentParser()
    parser.add_argument("-i", dest="input", help="json file to generate c++ types from", required=True)
    parser.add_argument("-o", dest="output", help="where to write the new file to", required=True)
    args = parser.parse_args()
    generate_hpp_from_json(args.input, args.output)