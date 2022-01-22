module InstructionMemory(read_address, instruction);
// Input
input [31:0] read_address;
// Output
output reg [15:0] instruction;
// Instruction Memory
reg [15:0] memory [29:0];

// Reading next instruction from memory and assigning it into output instruction
always @(*) begin
	instruction[15:0] = memory[read_address];
end
		
endmodule 