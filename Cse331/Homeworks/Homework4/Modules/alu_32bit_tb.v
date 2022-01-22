`define DELAY 20
module alu_32bit_tb();
// Registers
reg [2:0]  select;
reg signed [31:0] value1;
reg signed [31:0] value2;
// Wire
wire signed [31:0] result;

// Calling alu_32bit
alu_32bit alu32(value1, value2, select, result);

initial begin
// First test values
value1 = 32'b00000000000000000000000000110011;
value2 = 32'b00000000000000000000000000101001;

select = 3'b000; // AND
#`DELAY
select = 3'b001; // ADD
#`DELAY
select = 3'b010; // SUB
#`DELAY
select = 3'b011; // XOR
#`DELAY
select = 3'b100; // NOR
#`DELAY
select = 3'b101; // OR
#`DELAY
select = 3'b110; // SLT
#`DELAY
select = 3'b111; // MULT

#`DELAY
// Second test values
value1 = 32'hAAAAAAAA;
value2 = 32'h33333333;

select = 3'b000; // AND
#`DELAY
select = 3'b001; // ADD
#`DELAY
select = 3'b010; // SUB
#`DELAY
select = 3'b011; // XOR
#`DELAY
select = 3'b100; // NOR
#`DELAY
select = 3'b101; // OR
#`DELAY
select = 3'b110; // SLT
#`DELAY
select = 3'b111; // MULT
end

initial begin
$monitor("Time  : %1d - Select: %3b\nvalue1: %32b \nvalue2: %32b \nresult: %32b \n",
			$time,
			select,
			value1,
			value2,
			result	);
end


endmodule 