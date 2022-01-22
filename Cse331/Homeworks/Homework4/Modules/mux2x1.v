module mux2x1(input1, input2, select, result);
// Inputs
input input1;
input input2;
input select;
// Output
output result;
// Wires
wire inverse_select;
wire and1_result;
wire and2_result;

// Inner calculations
not not1(inverse_select, select);
and and1(and1_result, input2, select);
and and2(and2_result, input1, inverse_select);

// Result
or or1(result, and1_result, and2_result);

endmodule