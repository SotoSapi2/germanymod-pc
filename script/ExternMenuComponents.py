import re
from pathlib import Path

file_structure = """// Auto generated with ExternMenuComponents.py
#include "../framework/UIFramework.hpp"

namespace Menu
{
    extern bool gMenuShown;

    void INIT();
    (REPLACE_ME)
}
"""

def get_project_path():
    return Path(__file__).parent.parent

def get_src_path():
    return get_project_path() / "src"

def exclude_menu_functions(input_code):
    pattern = r"#pragma region MenuFunctions([\s\S]*?)#pragma endregion"
    replacement = r""

    return re.sub(pattern, replacement, input_code)

def generate_file_structure(input_code):
    return file_structure.replace("(REPLACE_ME)", input_code)

def extern_menu_variable(input_code):
    pattern = r"((?!\s).+)\s(\w+)\s*=[\s\S]*?;"
    replacement = r"extern \1 \2;"

    return re.sub(pattern, replacement, input_code)

def extern_menu_componenets(input_code):
    pattern = r"(\w+)\s+(\w+)\s*\([\s\S]*?\);"
    replacement = r"extern \1 \2;"

    return re.sub(pattern, replacement, input_code)

def start_gen():
    src_path = get_src_path()
    game_path = src_path / "runtime/game/"

    menu_source = game_path / "Menu.cpp"
    menu_header = game_path / "Menu.hpp"

    try:
        input_code = open(menu_source, "r").read()
        input_code = exclude_menu_functions(input_code)

        region_pattern = "#pragma region MenuComponent([\s\S]*?)#pragma endregion"
        region_match = re.search(region_pattern, input_code, re.ASCII)

        if region_match:
            input_code = region_match.group(1)
            input_code = extern_menu_variable(input_code)
            input_code = extern_menu_componenets(input_code)
            input_code = generate_file_structure(input_code)
            open(menu_header, "w").write(input_code)
        else:
            print("No 'MenuComponent' pragma region found.")

    except FileNotFoundError:
        msg = str(game_path / ".cpp")
        print(f"Error: File '{msg}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

start_gen()