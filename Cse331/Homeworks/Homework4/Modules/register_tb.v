module register_tb();
// Registers
reg [2:0] read_reg1;
reg [2:0] read_reg2;
reg [2:0] write_reg;
reg [31:0]write_data;
reg RegWrite, clock;
// Wires
wire [31:0] read_data1;
wire [31:0] read_data2;

// Calling the register module
register r(read_reg1, read_reg2, write_reg, write_data, RegWrite, read_data1, read_data2, clock);

initial begin
	clock = 1'b0;
	forever #2 clock = ~clock;
end

// Giving Inputs
initial begin
// I will read first and second registers
read_reg1  = 3'b001;
read_reg2  = 3'b010;
// First, writing to the first register 
write_reg  = 3'b001;
write_data = 32'hFFFF1111;
RegWrite   = 1'b1;
#4;
// Second, reading from registers
RegWrite = 1'b0;
#4;
// Then, writing to the second register
write_reg  = 3'b010;
write_data = 32'hFFFF0000;
RegWrite = 1'b1;
#4;
// Final reading
RegWrite = 1'b0;
#4;

end

// Printing the results
initial begin
$monitor("Time: %3d | Clock: %1b | RegWrite: %1b | Read Register 1: %3b | Read Register 2: %3b | Write Register: %3b\nWrite Data : %32b\nRead Data 1: %32b\nRead Data 2: %32b", 
			$time, clock, RegWrite, read_reg1, read_reg2, write_reg, 
			write_data, read_data1, read_data2);
end

endmodule 