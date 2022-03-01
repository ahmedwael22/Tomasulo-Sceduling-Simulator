# Tomasulo-Sceduling-Simulator
Simulator for Tomasulo's Algorithm for Dynamic Scheduling of Instructions

## Simulation
Single-issue processor following the non-speculative version of Tomasulo's Algorithm
Issue: 1 cycle
Execute: N cycles specified by user
Write: 1 cycle

### Assumptions
1. Fetching and Decoding are assumed to take 0 cycles
2. Separate data and instruction memory (i.e. PC can be 100 and we can have data at address 100 as well).
3. Only one instruction can write anything per clock cycle.

### Final Note
For each instruction, the number of reservations stations is specified by user, as is the number of cycles for each instruction

## Output
1. Table listing the clock cycle time at which each instruction was: issued, started execution, finished execution, and was written.
2. Total execution time expressed as the number of cycles spanned
3. Instructions per Cycle Rate

## Supported Instructions
Supports LW, SW, JALR, RET, ADD, NEG, ADDI, DIV

### Detailed Description of Supported Instructions
LW
Format: **LW rd, imm(rs1)**
Loads value from memory into rd. Memory address is formed by adding imm with contents of rs1, where imm is a 7-bit signed immediate value (ranging from -64 to 63).

SW
Format: **SW rs2, imm(rs1)**
Stores value from rs2 into memory. Memory address is computed as in the case of  the load word instruction

JALR
Format: **JALR rs1**
Stores the value of PC+1 (value program counter register + 1) in R1 (Register 1) and branches (unconditionally) to the address in rs1

RET
Format: **RET**
Branches (unconditionally) to the address stored in R1

ADD
Format: **ADD rd, rs1, rs2**
Adds the value of rs1 and rs2 storing the result in rd

NEG
Format: **NEG rd, rs1**
Inverts the sign of rs1 storing the result in rd

ADDI
Format: **ADDI rd, rs1, imm**
Adds the value of rs1 to imm storing the result in rd

DIV
Format: **DIV rd, rs1, rs2**
Divides rs1 by rs2 storing the result in r
