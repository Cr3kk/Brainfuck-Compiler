//TODO: Add error handling
//TODO: optimize
//TODO: Add comments

use std::env;
use std::fs;
use std::io::{self, Write};
use std::path::Path;

fn main() {
    change_directory();

    let code = match read_file() {
        Ok(code) => {
            code
        }
        Err(e) => {
            eprintln!("Error reading file: {}", e);
            return;
        }
    };

    let mut code_free = String::new();

    for i in code.chars() {
        if i == '[' || i == ']' || i == '>' || i == '<' || i == '+' || i == '-' || i == '.' || i == ',' {
            code_free.push(i);
        }
    }

    println!("{}\n", code_free);

    let mut memory: [u8; 30000] = [0; 30000];
    let mut cur_pointer: usize = 0;

    execute_code(&code_free, &mut memory, &mut cur_pointer);

    println!("\n");
}

fn change_directory() {
    let vscode_path = Path::new("../");
    if let Err(e) = env::set_current_dir(&vscode_path) {
        eprintln!("Failed to change directory: {}", e);
    } else {
        println!("Successfully changed to directory: {:?}", vscode_path);
    }
}

fn read_file() -> Result<String, std::io::Error> {
    fs::read_to_string("target.bf")
}

fn execute_code(code: &str, memory: &mut [u8; 30000], cur_pointer: &mut usize) {
    let mut instruction_pointer: usize = 0;
    let mut loop_stack = Vec::new();

    while instruction_pointer < code.len() {
        let instruction = code.as_bytes()[instruction_pointer] as char;

        match instruction {
            '>' => *cur_pointer += 1,
            '<' => *cur_pointer -= 1,
            '+' => memory[*cur_pointer] = memory[*cur_pointer].wrapping_add(1),
            '-' => memory[*cur_pointer] = memory[*cur_pointer].wrapping_sub(1),
            '.' => {
                print!("{}", memory[*cur_pointer] as char);
                io::stdout().flush().unwrap();
            }
            ',' => {
                let mut input = String::new();
                io::stdin().read_line(&mut input).expect("Failed to read line");
                if let Some(byte) = input.bytes().next() {
                    memory[*cur_pointer] = byte;
                }
            }
            '[' => {
                if memory[*cur_pointer] == 0 {
                    let mut bracket_count = 1;
                    while bracket_count > 0 {
                        instruction_pointer += 1;
                        if code.as_bytes()[instruction_pointer] as char == '[' {
                            bracket_count += 1;
                        } else if code.as_bytes()[instruction_pointer] as char == ']' {
                            bracket_count -= 1;
                        }
                    }
                } else {
                    loop_stack.push(instruction_pointer);
                }
            }
            ']' => {
                if memory[*cur_pointer] != 0 {
                    if let Some(&loop_start) = loop_stack.last() {
                        instruction_pointer = loop_start;
                    }
                } else {
                    loop_stack.pop();
                }
            }
            _ => {}
        }
        instruction_pointer += 1;
    }
}
