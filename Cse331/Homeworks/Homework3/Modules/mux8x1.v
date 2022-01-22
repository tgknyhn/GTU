module mux8x1(input1, input2, input3, input4, input5, input6, input7, input8, select, result);
// Inputs
input [2:0] select;
input input1;
input input2;
input input3;
input input4;
input input5;
input input6;
input input7;
input input8;
// Output
output result;
// Wires
wire mux_result_1;
wire mux_result_2;
wire mux_result_3;
wire mux_result_4;
wire mux_result_5;
wire mux_result_6;

// First we call 2x1 mux 4 times
mux2x1 mux1(input1, input2, select[0], mux_result_1),
		 mux2(input3, input4, select[0], mux_result_2),
		 mux3(input5, input6, select[0], mux_result_3),
		 mux4(input7, input8, select[0], mux_result_4);
		 
// Then we call 2 more times
mux2x1 mux5(mux_result_1, mux_result_2, select[1], mux_result_5),
		 mux6(mux_result_3, mux_result_4, select[1], mux_result_6);

// Final call and result
mux2x1 mux7(mux_result_5, mux_result_6, select[2], result);

endmodule