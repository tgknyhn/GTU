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
value1 = 32'h77777777;
value2 = 32'h0F0F0F0F;

select = 3'b000; // ADD
#`DELAY
select = 3'b001; // XOR
#`DELAY
select = 3'b010; // SUB
#`DELAY
select = 3'b011; // MULT
#`DELAY
select = 3'b100; // SLT
#`DELAY
select = 3'b101; // NOR
#`DELAY
select = 3'b110; // AND
#`DELAY
select = 3'b111; // OR

#`DELAY
// Second test values
value1 = 32'hAAAAAAAA;
value2 = 32'h33333333;

select = 3'b000; // ADD
#`DELAY
select = 3'b001; // XOR
#`DELAY
select = 3'b010; // SUB
#`DELAY
select = 3'b011; // MULT
#`DELAY
select = 3'b100; // SLT
#`DELAY
select = 3'b101; // NOR
#`DELAY
select = 3'b110; // AND
#`DELAY
select = 3'b111; // OR
end

initial begin
$monitor("Time  : %1d - Select: %3b\nvalue1: %32b (%8h) (%11d)\nvalue2: %32b (%8h) (%11d)\nresult: %32b (%8h) (%11d)\n",
			$time,
			select,
			value1, value1, value1,
			value2, value2, value2,
			result, result, result);
end


endmodule 