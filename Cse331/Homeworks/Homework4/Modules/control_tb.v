`define DELAY 20
module control_tb();
// Register
reg [3:0] Opcode;
// Wires
wire [2:0] ALUOp;
wire RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch;

// Calling the control unit
control c(Opcode, RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, ALUOp);

// Delay - Giving Inputs
initial begin
		  Opcode = 4'b0000;
#`DELAY Opcode = 4'b0001;
#`DELAY Opcode = 4'b0010;
#`DELAY Opcode = 4'b0011;
#`DELAY Opcode = 4'b0100;
#`DELAY Opcode = 4'b0101;
#`DELAY Opcode = 4'b0110;
#`DELAY Opcode = 4'b0111;
#`DELAY Opcode = 4'b1000;
#`DELAY Opcode = 4'b1001;

end

// Printing the results
initial begin
$monitor("Time: %3d | Opcode: %4b\nRegDst: %1b | ALUSrc: %1b | MemtoReg: %1b | RegWrite: %1b | MemRead: %1b | MemWrite: %1b | Branch: %1b | ALUOp: %4b\n", 
			$time, Opcode,
			RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, ALUOp);
end

endmodule 