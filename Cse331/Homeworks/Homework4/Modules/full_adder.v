module full_adder(value1, value2, sum, carry_out, carry_in);
// Inputs
input value1;
input value2;
input carry_in;
// Outputs
output sum;
output carry_out;
// Wires
wire temp_sum;
wire first_carry_out;
wire second_carry_out;

// Full Adder
half_adder ha_1(value1  , value2   , temp_sum, first_carry_out );
half_adder ha_2(temp_sum, carry_in , sum     , second_carry_out);

// Final carry_out
or o(carry_out, first_carry_out, second_carry_out);

endmodule