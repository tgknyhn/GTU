module mux2x1_3bit(input1, input2, select, result);
// Inputs
input select;
input [2:0] input1;
input [2:0] input2;
// Output
output [2:0] result;

mux2x1 mux1(input1[0], input2[0], select, result[0]),
		 mux2(input1[1], input2[1], select, result[1]), 
		 mux3(input1[2], input2[2], select, result[2]);


endmodule 