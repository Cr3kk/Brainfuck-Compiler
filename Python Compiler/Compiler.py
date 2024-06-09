# TODO: add error handling

import os
import array

# Ensure you set the current directory to where your file is located
os.chdir("../")

# Reads the target file to compile
with open("target.bf", "r") as f:
    code = f.read()

# Converts the file to a string and removes all non valid characters (thus 'free' code)
code_free = [i for i in code if i in "><+-.,[]"]

# Prints the file (without comments) (for testing purposes)
print("".join(code_free), "\n")

# Initializes the memory
memory = array.array("B", [0] * 30000)  # Extendable memory (change if more memory is needed)
# Initializes the memory pointer
cur_pointer = 0

# Preprocess the code to find matching brackets
bracket_map = {}
stack = []
for i, command in enumerate(code_free):
    if command == "[":
        stack.append(i)
    elif command == "]":
        start = stack.pop()
        bracket_map[start] = i
        bracket_map[i] = start

# Initializes the instruction pointer
instruction_pointer = 0

while instruction_pointer < len(code_free):
    i = code_free[instruction_pointer]  # Gets the current command

    if i == ">":  # Moves the pointer to the right (+1) or wraps to 0 if at the end
        cur_pointer = (cur_pointer + 1) % len(memory)
    elif i == "<":  # Moves the pointer to the left (-1) or wraps to the end if at 0
        cur_pointer = (cur_pointer - 1) % len(memory)
    elif i == "+":  # Increases the current memory cell by 1
        memory[cur_pointer] = (memory[cur_pointer] + 1) % 256
    elif i == "-":  # Decreases the current memory cell by 1
        memory[cur_pointer] = (memory[cur_pointer] - 1) % 256
    elif i == ".":  # Outputs the current memory cell as a character
        print(chr(memory[cur_pointer]), end="")
    elif i == ",":  # Inputs a character or integer into the current memory cell
        user_input = input("in: ")
        if len(user_input) == 1:
            memory[cur_pointer] = ord(user_input)
        else:
            try:
                value = int(user_input)
                if 0 <= value <= 255:
                    memory[cur_pointer] = value
                else:
                    print("Error: Input must be a single character or an integer between 0 and 255")
            except ValueError:
                print("Error: Invalid input. Input must be a single character or an integer between 0 and 255")
    elif i == "[":  # Detects the start of a loop
        if memory[cur_pointer] == 0:
            instruction_pointer = bracket_map[instruction_pointer]
    elif i == "]":  # Detects the end of a loop
        if memory[cur_pointer] != 0:
            instruction_pointer = bracket_map[instruction_pointer]
    instruction_pointer += 1  # Move to the next command

print("")  # Print a newline at the end, for readability in the terminal
# TODO: add comments to show in the terminal
