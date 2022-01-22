module DataMemory(MemWrite, MemRead, address, write_data, read_data, clock);
// Inputs
input MemWrite, MemRead;
input clock;
input [31:0] address;
input [31:0] write_data;
// Output
output reg [31:0] read_data;
// 8 register, each with 32 bits of data
reg [31:0] memory [31:0];

always @(posedge clock) begin
	if(MemWrite == 1)
		memory[address] = write_data[31:0];
end

always @(clock) begin
	if(MemRead == 1)
		read_data[31:0] = memory[address];
end


endmodule 