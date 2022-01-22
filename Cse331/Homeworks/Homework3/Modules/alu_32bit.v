module alu_32bit(value1, value2, select, result);
// Inputs
input [2:0]  select;
input [31:0] value1;
input [31:0] value2;
// Output
output [31:0] result;
// Wires
wire [31:0] ADD;
wire [31:0] XOR;
wire [31:0] SUB;
wire [31:0] MULT;
wire [31:0] SLT;
wire [31:0] NOR;
wire [31:0] AND;
wire [31:0] OR;
// Temp
wire add_carry_out;
wire add_overflow;
wire sub_overflow;

// Module results
adder_32bit add32(value1, value2, ADD, add_carry_out, 1'b0, add_overflow);
xor_32bit   xor32(value1, value2, XOR);
sub_32bit   sub32(value1, value2, SUB, overflow);
slt_32bit   slt32(value1, value2, SLT);
nor_32bit   nor32(value1, value2, NOR);
and_32bit   and32(value1, value2, AND);
or_32bit    or32 (value1, value2, OR );

// Adding results to the mux8x1_32bit
mux8x1_32bit mux32(ADD, XOR, SUB, MULT, SLT, NOR, AND, OR, select, result);	

endmodule 