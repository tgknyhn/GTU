module slt_32bit(value1, value2, slt);
// Inputs
input [31:0] value1;
input [31:0] value2;
// Output
output [31:0] slt;
// Wire
wire _1bit_slt;
wire overflow;
wire [31:0] result;

// value1 - value2
sub_32bit sub32(value1, value2, result, overflow);
// Controlling most significant bit
xor xor32(_1bit_slt, overflow, result[31]);

// 32 bit 2x1 mux to deciding return either 32 bits of 1 or 0
mux2x1_32bit mux(32'h00000000, 32'hFFFFFFFF, _1bit_slt, slt); 


endmodule
