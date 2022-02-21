# Breadboard Computer v2

This project is related to building hardware more than a software project.  There will be some code added to this project later on for programming the firmware and downloading a program, but that will be in the final stages.

This project is inspired by [Ben Eater's Breadboard Computer](https://eater.net/8bit).  Currently as this get started, I have his computer nearly complete and I am working on the final connections and debugging before I get into the control logic.

As usual with my personal projects, I am compelled to keep a journal of this progress.  As usual with my journals, I will not go back and edit anything historical as these are my continuing thoughts rather than documentation of the final product.


## Flags Register

The flags register is the next outcome from the ALU.  I expect that I should be able to keep the flags limited to 8-bits, but it will take 2 breadboards to implement.


---

### 2022-Feb-19

So, the first order of business here will be to decide on the flags and what operations I will need to perform on each.  The following table will summarize my thinking:

| Flag             | Set | Clear | Latch |
|:----------------:|:---:|:-----:|:-----:|
| Interrupt Enable |  Y  |   Y   |       |
| Carry            |  Y  |   Y   |   Y   |
| Zero             |     |       |   Y   |
| Parity           |     |       |   Y   |
| Sign             |     |       |   Y   |
| Overflow         |     |   Y   |   Y<sup>*</sup>  |


> Overflow Detection – Overflow occurs when:
>
> * Two negative numbers are added and an answer comes positive
> * Two positive numbers are added and an answer comes negative

All of the Latched flags will be latched at the same time.  That said, I may need to add an additional control flag to control the `V` flag (<sup>*</sup> which we may not want to latch for every ALU operation).

Going back to the conditions to be met:
* If `A7 == 0 && B7 == 0 && ALU7 == 1` ==> Set `V`
* If `A7 == 1 && B7 == 1 && ALU7 == 0` ==> Set `V`

This should happen on ADD, ADC, SUB, and SBB only.

Put another way: `V = (((A7 NOR B7) AND ALU7) OR ((A7 AND B7) AND (NOT ALU7))) AND V`<sub>`Control`</sub> (when it is supposed to be set).

I think this, then, is doable.  But I may need more than 2 breadboards to get it done:
* `Z` is handled on the ALU Control Module
* `P` is handled on the ALU Control Module
* `C` is handled on the ALU Control Module
* `S` is trivial (`S = ALU7`)
* `V` will need to be handled on the Flags Module
* `I` will be handled in the Flags Module (and may move to the Control Module if I run out of space)

I wanted to list out the components I need, but I will have to do some drawing for each flag.  Each one will be different.  So, I will need to spend a little time drawing to make sure I can handle each properly before I aggregate the components I need.


---

### 2022-Feb-20

Well, I have just realized that I do not have enough ICs on the ALU to properly handle all 3 flags the way I thought.  So, what do I need on the Flags Module?

* General Register
  * 74LS245 - Tri-state buffer
  * 10-LED Graph
  * 74LS74 - Dual D Flip-Flop
    * `Z` Flag
    * `P` Flag
  * 74LS74 - Dual D Flip-Flop
    * `C` Flag
    * `S` Flag
  * 74LS74 - Dual D Flip-Flop
    * `V` Flag
    * `I` Flag
  * 74LS04 - Hex Inverter
    * Control Buffer Output
    * Clear `I` Control
    * Set `I` Control
    * Clear `C` Control
    * Set `C` Control
    * Clear `V` Control
  * 74LS08 - Quad AND Gate
    * Latch `C`, `S`, `P`, `Z`
    * Latch `V`
* `V` Flag determination
  * 74LS08 - Quad AND Gate
    * All 4 used for `V`
  * 74LS32 - Quad OR Gate
    * 2 used for `V`
  * 74LS04 - Hex Inverter
    * 2 used for `V`

That's 10 elements on 2 breadboards.  That is also going to be dense.

So, it makes sense to add another row of boards.  Let me get that done.

---

I had an epiphany.  I can use EEPROMs to do all this flags evaluation work and likely get a faster result.  I should be able to include the `V` determination logic and therefore should be able to save a number of ICs (and the extra breadboard I just added).  I still have 7 circuit elements so it should be able to fit comfortably.

So, I will return to the ALU and get that wrapped up.

