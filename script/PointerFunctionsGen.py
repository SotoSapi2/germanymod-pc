import re
from pathlib import Path

header_code = """// Auto-generated with PointerFunctionsGen.py
#pragma once
#include <IL2CPP.hpp>
#include "Structs.hpp"
#include "../structures/PointerWrapper.hpp"
"""

def get_project_path():
    return Path(__file__).parent.parent

def get_src_path():
    return get_project_path() / "src"

def generate_extra_header_body(input_code):
    global header_code

    remove_pattern = r"#include \".*\"";
    output = re.sub(remove_pattern, "", input_code);

    return header_code + output

def generate_extern_pointer_fields(input_code):
    pattern = r"Pointer<(.*?)>+\s(.+)*(\([\s\S]*?\));";
    replacement = r"extern Pointer<\1> \2;";
    return re.sub(pattern, replacement, input_code);

def generate_pointer_definator(input_code):
    namespace_pattern = r"namespace\s+(.*?)\s*\{([^}]*)\}"
    pointer_pattern = r"Pointer<.*?>+\s(.+)[;| ]"

    output = "\n\nnamespace PointerFunctions\n{\n\tstatic void INIT()\n\t{\n"

    for namespace, body in re.findall(namespace_pattern, input_code):
        if namespace == "PointerFunctions":
            continue
        
        output += f"\t\t// {namespace}\n"
        for field_name in re.findall(pointer_pattern, body):
            output += f"\t\t{namespace}::{field_name}.Define();\n"
        
        output += "\n"
        # string = 
        # print(string)
    return output + "\t}\n}"

def start_gen():
    src_path = get_src_path()
    game_path = src_path / "runtime/game/"

    executable_file = game_path / "import/PointerFunctions.cpp"
    header_file = game_path / "import/PointerFunctions.hpp"

    try:
        input_code = open(executable_file, "r").read()
            
        header_output = generate_extra_header_body(
            generate_extern_pointer_fields(input_code)
        )
        
        header_output += generate_pointer_definator(header_output)
        
        open(header_file, "w").write(header_output)

    except FileNotFoundError:
        msg = str(game_path / "PointerFunctions.cpp")
        print(f"Error: File '{msg}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

start_gen()