import re
import os
import time 
import math

def extract_stack_trace(file_path):
    # Define the regex pattern
    pattern = r"========== OUTPUTTING STACK TRACE ==================([\s\S]*?)========== END OF STACKTRACE ==========="

    # Read the file content
    with open(file_path, 'r') as file:
        content = file.read()

    # Search for the pattern
    match = re.search(pattern, content, re.DOTALL)
    if match:
        # Extract the stack trace group
        stack_trace = match.group(1)
        return stack_trace.strip()
    else:
        return None

# Example usage
if __name__ == "__main__":
    file_path = os.path.expandvars(r"%appdata%\\..\\LocalLow\\Pixel Gun Team\\Pixel Gun 3D\\Player.log")
    
    stack_trace = extract_stack_trace(file_path)

    if stack_trace:
        print("Crash Stacktrace:")
        print(stack_trace)
    else:
        print("No stack trace found in the file.")