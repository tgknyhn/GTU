module sub_32bit(value1, value2, result, overflow);
// Inputs
input [31:0] value1;
input [31:0] value2;
// Output
output [31:0] result;
output overflow;
// Wires
wire [31:0] inverse_value2;
wire temp_carry_out;

// Taking inverse of value2
not_32bit not32(value2, inverse_value2);

// Using 32 bit adder with value1 + (value2' + 1)
adder_32bit add32(value1, inverse_value2, result, temp_carry_out, 1'b1, overflow);

endmodule
