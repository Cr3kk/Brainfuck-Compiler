//TODO: add error handling
//TODO: optimize
//TODO: add comments

using System;
using System.IO;

namespace C__Compiler
{
    class Program
    {
        public static void Main()
        {
            string code = ReadFile();
            string code_free = "";

            for (int i = 0; i < code.Length; i++)
            {
                if (code[i] == '+' || code[i] == '-' || code[i] == '>' || code[i] == '<' || code[i] == '.' || code[i] == ',' || code[i] == '[' || code[i] == ']')
                {
                    code_free += code[i];
                }
            }

            Console.WriteLine(code_free + "\n");

            byte[] memory = new byte[30000];
            int curPointer = 0;
            ExecuteCode(code_free, memory, curPointer);
            Console.WriteLine("");
        }

        public static string ReadFile()
        {
            string filePath = "../target.bf";

            if (File.Exists(filePath))
            {
                string return_line = "";
                string[] lines = File.ReadAllLines(filePath);
                if (lines.Length > 0)
                {
                    foreach (string line in lines)
                    {
                        return_line += line;
                    }
                    string return_line2 = return_line.Replace(" ", string.Empty);
                    return return_line2;
                }
                else 
                {
                    return "File is empty";
                }

            } 
            else 
            {
                return "File not found";
            }
        }

        public static void ExecuteCode(string code, byte[] memory, int curPointer)
        {
            int instructionPointer = 0;
            int loopStart = 0; // Initialize loopStart variable

            while (instructionPointer < code.Length)
            {
                char i = code[instructionPointer]; // Declare 'i' variable

                switch (i)
                {
                    case '+':
                        memory[curPointer]++;
                        break;
                    case '-':
                        memory[curPointer]--;
                        break;
                    case '>':
                        curPointer++;
                        break;
                    case '<':
                        curPointer--;
                        break;
                    case '.':
                        Console.Write((char)memory[curPointer]);
                        break;
                    case ',':
                        memory[curPointer] = (byte)Console.Read(); //TODO: fix this shit, it dont work
                        break;
                    case '[':
                        if (memory[curPointer] == 0)
                        {
                            int bracketCount = 1;
                            while (bracketCount > 0)
                            {
                                instructionPointer++;
                                if (code[instructionPointer] == '[')
                                {
                                    bracketCount++;
                                }
                                else if (code[instructionPointer] == ']') // Fixed 'elif' to 'else if'
                                {
                                    bracketCount--;
                                }
                            }
                        }
                        else 
                        {
                            loopStart = instructionPointer;
                        }
                        break;
                    case ']':
                        if (memory[curPointer] != 0)
                        {
                            int bracketCount = 1;
                            while (bracketCount > 0)
                            {
                                instructionPointer--;
                                if (code[instructionPointer] == '[')
                                {
                                    bracketCount--;
                                }
                                else if (code[instructionPointer] == ']') // Fixed 'elif' to 'else if'
                                {
                                    bracketCount++;
                                }
                            }
                        }
                        break;
                    default:
                        Console.WriteLine("Error");
                        break;
                }
                instructionPointer++;
            }
        }
    }
}
