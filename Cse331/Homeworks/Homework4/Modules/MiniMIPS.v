module MiniMIPS(clock);
// Only one input for MiniMIPS module :)
input clock;

// ** Other Modules' Wires **

// nextPC
wire [31:0] NextPC;
wire [31:0] extendedValue;
// Instruction Memory
wire [15:0] instruction;
// Register
wire [2:0]  write_reg;
wire [31:0] write_data;
wire [31:0] read_data1;
wire [31:0] read_data2;
// Control Unit
wire [2:0] ALUOp;
wire RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
// ALU Control Unit
wire [2:0] ALUctr;
// ALU
wire [31:0] value2;
wire [31:0] result;
// Data Memory
wire [31:0] read_data;

// *********************************************************************** //

// Before calling NextPC I need to get branchAddress from last instruction
signExtend_6to32 extend(instruction[5:0], extendedValue);

// Part 1 : Calling NextPC module to calculate next instruction address
nextPC npc(Branch, extendedValue, NextPC, clock);

// Part 2 : Calling Instruction Memory module to get next instruction from memory
InstructionMemory im(NextPC, instruction);

// Part 3 : Calling Control module to create signals according to instruction
control cm(instruction[15:12], RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, ALUOp);

// Before calling register I need to find write_reg value by using a 2x1 3 bit multiplexer
// If RegDst is 0 choose rt, if RegDst is 1 choose rd
mux2x1_3bit muxReg(instruction[8:6], instruction[5:3], RegDst, write_reg);

// Part 4 : Calling Register module to read/write data from/to registers according to instruction type and control signals
register rm(instruction[11:9], instruction[8:6], write_reg, write_data, RegWrite, read_data1, read_data2, clock);

// Part 5 : Calling ALU Control module to decide which operation ALU is going to do
ALUcontrol aluCtrl(ALUOp, instruction[2:0], ALUctr);

// Before calling ALU, we know that first input will be rs. But second input can be rt data or an immediate value
// To decide it I will use ALUsrc signal
// If ALUsrc is 0 choose rt value, else if ALUsrc is 1 choose immediate value
mux2x1_32bit muxALU(read_data2, extendedValue, ALUSrc, value2);
 
// Part 6 : Calling ALU since we know what operation will be done
alu_32bit alu32(read_data1, value2, ALUctr, result);

// Part 7 : Calling Data Memory because now we can use result of the ALU operation
DataMemory data(MemWrite, MemRead, result, read_data2, read_data, clock);

// After that I need to find write_data value. To find it I need to use 2x1 32 bit multiplexer
// If MemtoReg is 0 choose ALU Result, else if MemtoReg is 1 choose Read_data value from DataMemory module
mux2x1_32bit muxData(result, read_data, MemtoReg, write_data);

endmodule 