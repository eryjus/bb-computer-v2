# Breadboard Computer v2

This project is related to building hardware more than a software project.  There will be some code added to this project later on for programming the firmware and downloading a program, but that will be in the final stages.

This project is inspired by [Ben Eater's Breadboard Computer](https://eater.net/8bit).  Currently as this get started, I have his computer nearly complete and I am working on the final connections and debugging before I get into the control logic.

As usual with my personal projects, I am compelled to keep a journal of this progress.  As usual with my journals, I will not go back and edit anything historical as these are my continuing thoughts rather than documentation of the final product.


## Assembler

The breadboard computer will need to be able to handle programs that are significantly larger.  To hand-assemble them will not be a good thing.  So, I will need to write an assembler for the toolchain.


### 2022-Mar-19

So, as I am thinking about how to test the hardware, I feel I need to be able to handle much more granular iterative development and testing.  At the moment I have the Program Counter, Memory Address Register, Memory Module, 2 Temporary Registers, and the ALU built out -- all with relatively little testing.  So, I need to "drop back 10 and punt" a little bit.

This is why I decided to start an assembler for the toolchain.  There will be other components as well -- such as something to build a microcode binary and programs for the computer for exercising the system.

But I also may want to extract this assembler from this project and use it in the next computer build, so I want to make it flexible as well.

I start with the basic output messaging.


---

### 2022-Mar-20

OK, so do I want to figure out how to hand-scan the assembler (not really) or do I want to use `flex` to tokenize the input?

I'm thinking I do want to do this since it will make scanning and tokenizing the input far simpler.  Then, I should be able to hand-code the parser.  However, this also eliminates the goal to make the assembler configurable.


---

### 2022-Mar-21

OK, so let's go with `flex`.  The first thing I want to do is to be able to include other files.  So, the first thing will be to implement the `.include` directive.

---

That is working well enough.  I have comments supported as well.

Next, to implement some search paths in the source files as well, so I do not need to type the full path in the source.  Later, I will add command line parameters to set this table as well.

---

OK, I have that working now.  I can specify the paths in the source from which to add additional search paths.  This is cool.

Now, I have cleaned up some of the error conditions and improved the messaging.  Now, on to defining the architecture using an `.architecture` directive.  Should I make it just an `.arch` directive?  I think so.

A strong point to make here is that only some directives will be available from within the `.arch` include file.

I think I am going to have to make the EOL be a part of the semantic analysis, meaning I need to return it as a token.


---

### 2022-Mar-22

What do do next today?  I do have the `TOK_EOL` handled in the scanner thus far.  So, I think the next thing is going to be to start a top-down parser.  This will mean I need my own `main` function, not the one supplied by flex.

OK, what the hell is returning a `'0'`?

```
Unimplemented token is parser (software/assembler/parser.c:27): 0
```

Ahhh...  it's `<<EOF>>`.

OK, so let's try to get a register defined.

When I define a register, do I want to specify how many bytes wide it is?  If I consider James Sharman's computer (2 different register sizes), then yes I do.  Since I would not want that to be in hex, I need to discern between hex and decimal numbers.  I think I will implement the prefix `0x`, and for that matter, `0b` and `0` (octal).

But this also means I will need to convert the numbers to a proper foundational type (not string) and then return that value rather than the string value to the parser.

I can now parse the file, but I have no storage for registers.  I need to build that out.  But before I do that, I should handle several error conditions.  This means I really need to get some error reporting APIs in place.

OK, I have the registers being captured in a register table.  I am also capturing the number of bits wide each register is and I can report on them at the end.  All is working well.

So, the next thing will be to parse the memory for the architecture (and create a buffer in which to write the bytes).  After that will be a list of opcodes and how to interpret them.

All in all, since I am hand-coding the parser, I should be able to keep this generic enough that I can generally support any architecture.  Now, I may need to enhance the tool set to handle other arch requirements....  But the foundation will be there.


---

### 2022-Mar-23

I took care of the memory ranges.  Right now, each range remains distinct; I have made no effort to optimize/combine adjacent memory ranges into a single range.  Right now I am going for function, not speed.  I am a bad programmer in that performance should always be considered, but how much time am I going to invest to improve speed to save how much time on the back end?  And introduce how many obscure bugs?  The brute-force method is elegant enough in this case.

So, now for the really hard task: defining the opcodes.  Generalizing, opcodes come in 3 forms:
1. just an opcode (such as `nop`) with no parameters
2. an opcode with 1 or 2 register parameters (`psh a` or `mov b,a`)
3. an opcode with 1 or 2 parameters where the last parameter is a numeric value (`jmp 0x1234` or `add a,3`)

Now, the number can also be the name of a label, or a location in memory.  I could also provide directives for symbolic constants as well (`.equ` comes to mind).

OK, focus, Adam.  1 thing at a time: opcodes.  For now, I will only support numbers, not labels.


---

### 2022-Mar-26

I have been working on the assembler a little bit each day over the last few days.  However, I have come to the conclusion that I am doing it wrong.

I have been breaking up the opcode definition into parts in order to be able to store it like a compiler would.  However, that's really not what I am wanting to do.  I really need to be able to clean up the definition (remove extra spaces, change the case, etc.) so that I can match the string easily when actually assembling.

This, then, will mean that I will need to write a function to match a line from the assembly code to an op-code to define what the resulting binary code will be.

The first thing I need to do is to write a function to clean up a string to make it into a standard format.  A standard format will be:
* no leading or trailing spaces
* only 1 space between the mnemonic and its operands
* no spaces between the operands or its components
* only 1 replacement value per mnemonic will be allowed, identified by `$(8)` or `$(16)`, depending on size allowed

This also means I will be able to take a line to be assembled and convert the any constant number to be either `$(8)` or `$(16)` depending on the constant's size and search for the proper opcode to use.  I will be able to fall-back to searching for a larger constant size (an 8-bit will fit into a 16-bit constant) if not found.

---

This settles the architecture definition and how to load up assembler with what its capabilities are.  Now, for the actual assembler.  Since I am only going to be outputting a binary file (not ELF or anything fancy), this should be easy to deal with.

I need to be able to position in the output binary.  Typically, this is done using the `.org` directive.  I am going to make the rule that the position can only be the same place or forward.  Going backward in memory is not allowed.  So, I am going to need to maintain a position in memory and let the `.org` directive update it.


---

### 2022-Mar-27

I completed a ton of coding yesterday and today I actually have a reasonably functioning assembler.  It does not protect me from myself and there is a whole lot of error checking that is needed for this to be considered robust, but I can create a binary instruction set.

I just need to output the results to a file.

Done.

I am going to prepare to commit these changes.  The problem here is that the assembler is largely undocumented and still needs some work.  But this is a huge update!



