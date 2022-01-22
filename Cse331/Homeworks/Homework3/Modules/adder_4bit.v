module adder_4bit(value1, value2, sum, carry_out, carry_in, overflow);
// Inputs
input [3:0] value1;
input [3:0] value2;
input carry_in;
// Outputs
output [3:0] sum;
output carry_out;
output overflow;
// Wires - Intermediate carries
wire c1;
wire c2;
wire c3;

// Calling full adder 4 times
full_adder fa_1(value1[0], value2[0], sum[0], c1       , carry_in),
			  fa_2(value1[1], value2[1], sum[1], c2       , c1      ),
           fa_3(value1[2], value2[2], sum[2], c3       , c2      ),
           fa_4(value1[3], value2[3], sum[3], carry_out, c3      );

// Checking if overflow happened or not
xor xor1(overflow, c3, carry_out);

endmodule