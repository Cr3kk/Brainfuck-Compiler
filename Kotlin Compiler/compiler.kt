//TODO: add error handling

import java.nio.file.Paths
import java.nio.file.Files

fun executeCode(code: String, memory: ByteArray) {
    var curPointer: Int = 0
    var instructionPointer = 0

    // Preprocess loop jumps
    val jumpTable = mutableMapOf<Int, Int>()
    val openBracketStack = mutableListOf<Int>()
    for (i in code.indices) {
        when (code[i]) {
            '[' -> openBracketStack.add(i)
            ']' -> {
                val openIndex = openBracketStack.removeAt(openBracketStack.size - 1)
                jumpTable[openIndex] = i
                jumpTable[i] = openIndex
            }
        }
    }

    while (instructionPointer < code.length) {
        when (code[instructionPointer]) {
            '>' -> curPointer = (curPointer + 1) % memory.size // moves the pointer to the right, or wraps around to 0
            '<' -> curPointer = (curPointer - 1 + memory.size) % memory.size // moves the pointer to the left, or wraps around to the end
            '+' -> memory[curPointer] = (memory[curPointer] + 1).toByte() // increments the value at the current pointer by 1
            '-' -> memory[curPointer] = (memory[curPointer] - 1).toByte() // decrements the value at the current pointer by 1
            '.' -> print(memory[curPointer].toInt().toChar())
            ',' -> {
                print("Input: ")
                val input = readLine() //TODO: Fix not getting input
                if (!input.isNullOrEmpty()) {
                    memory[curPointer] = input[0].code.toByte()
                }
            }
            '[' -> if (memory[curPointer] == 0.toByte()) {
                instructionPointer = jumpTable[instructionPointer] ?: instructionPointer // goes to the next instruction if the current value is 0
            }
            ']' -> if (memory[curPointer] != 0.toByte()) {
                instructionPointer = jumpTable[instructionPointer] ?: instructionPointer // goes to the next instruction if the current value is not 0
            }
        }
        instructionPointer++
    }
}

fun main() {
    val code = Files.readString(Paths.get("../target.bf")) // reads target file
    val validChars = setOf('+', '-', '>', '<', '.', ',', '[', ']') // sets characters that can be interpreted, otherwise throws the characters away as comments 
    // TODO: add comments to show in the terminal
    val codeFree = code.filter { it in validChars }

    println("$codeFree\n")

    val memory = ByteArray(30000) // can be changed if more or less memory is desired

    executeCode(codeFree, memory) // run the compiler

    println("\n") // newline at the end for readability
}
