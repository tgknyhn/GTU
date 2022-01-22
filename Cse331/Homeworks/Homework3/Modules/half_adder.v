module half_adder(value1, value2, sum, carry_out);
// Inputs
input  value1;
input  value2;
// Outputs
output sum;
output carry_out;

// Half Adder
and a(carry_out, value1, value2);
xor x(sum      , value1, value2);

endmodule