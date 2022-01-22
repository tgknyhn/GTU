module register(read_reg1, read_reg2, write_reg, write_data, RegWrite, read_data1, read_data2, clock);
// Inputs
input [2:0] read_reg1;
input [2:0] read_reg2;
input [2:0] write_reg;
input [31:0]write_data;
input RegWrite, clock;
// Outputs
output [31:0] read_data1;
output [31:0] read_data2;

// Register Memory
reg [31:0] registers [7:0];

assign read_data1 = registers[read_reg1];
assign read_data2 = registers[read_reg2];

always @(posedge clock) begin
	if(RegWrite && write_reg != 3'b000)
		registers[write_reg] = write_data;
end

endmodule 