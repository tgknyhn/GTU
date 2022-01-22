module control(Opcode, RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, ALUOp);
// Inputs
input  [3:0] Opcode;
// Outputs
output [2:0] ALUOp;
output RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;
// Wires
wire [3:0] OpcodeNot;
wire r_types, addi, andi, ori, nori, beq, bne, slti, lw, sw;

// First, assigning OpcodeNot values
not not0(OpcodeNot[0], Opcode[0]),
	 not1(OpcodeNot[1], Opcode[1]),
	 not2(OpcodeNot[2], Opcode[2]),
	 not3(OpcodeNot[3], Opcode[3]);

// Second, finding other wire values
and and0(r_types, OpcodeNot[3], OpcodeNot[2], OpcodeNot[1], OpcodeNot[0]), // 0000 (R-types)
	 and1(addi   , OpcodeNot[3], OpcodeNot[2], OpcodeNot[1], Opcode[0]   ), // 0001 (ADDI)
	 and2(andi   , OpcodeNot[3], OpcodeNot[2], Opcode[1]   , OpcodeNot[0]), // 0010 (ANDI)
	 and3(ori    , OpcodeNot[3], OpcodeNot[2], Opcode[1]   , Opcode[0]   ), // 0011 (ORI)
	 and4(nori   , OpcodeNot[3], Opcode[2]   , OpcodeNot[1], OpcodeNot[0]), // 0100 (NORI)
	 and5(beq    , OpcodeNot[3], Opcode[2]   , OpcodeNot[1], Opcode[0]   ), // 0101 (BEQ)
	 and6(bne    , OpcodeNot[3], Opcode[2]   , Opcode[1]   , OpcodeNot[0]), // 0110 (BNE)
	 and7(slti   , OpcodeNot[3], Opcode[2]   , Opcode[1]   , Opcode[0]   ), // 0111 (SLTI)
	 and8(lw     , Opcode[3]   , OpcodeNot[2], OpcodeNot[1], OpcodeNot[0]), // 1000 (LW)
	 and9(sw     , Opcode[3]   , OpcodeNot[2], OpcodeNot[1], Opcode[0]   ); // 1001 (SW)

// Finally, determining the output values using the truth table I created
or  or0(RegDst  , r_types, 0),
	 or1(ALUSrc  , addi, andi, ori, nori, slti, lw, sw),
	 or2(MemtoReg, lw, 0),
	 or3(RegWrite, r_types, addi, andi, ori, nori, slti, lw),
	 or4(MemRead , lw, 0),
	 or5(MemWrite, sw, 0),
	 or6(Branch  , beq, bne),
	 or7(ALUOp[2], nori, beq, bne, slti),
	 or8(ALUOp[1], andi, ori, slti),
	 or9(ALUOp[0], addi, ori, beq, bne, lw, sw);
	 
endmodule 