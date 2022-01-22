module mux2x1_32bit(input1, input2, select, result);
// Inputs
input select;
input [31:0] input1;
input [31:0] input2;
// Output
output [31:0] result;

mux2x1 mux1 (input1[0] , input2[0] , select, result[0] ),
		 mux2 (input1[1] , input2[1] , select, result[1] ), 
		 mux3 (input1[2] , input2[2] , select, result[2] ), 
		 mux4 (input1[3] , input2[3] , select, result[3] ), 
		 mux5 (input1[4] , input2[4] , select, result[4] ), 
		 mux6 (input1[5] , input2[5] , select, result[5] ), 
		 mux7 (input1[6] , input2[6] , select, result[6] ), 
		 mux8 (input1[7] , input2[7] , select, result[7] ), 
		 mux9 (input1[8] , input2[8] , select, result[8] ), 
		 mux10(input1[9] , input2[9] , select, result[9] ), 
		 mux11(input1[10], input2[10], select, result[10]),
		 mux12(input1[11], input2[11], select, result[11]),
		 mux13(input1[12], input2[12], select, result[12]),
		 mux14(input1[13], input2[13], select, result[13]),
		 mux15(input1[14], input2[14], select, result[14]),
		 mux16(input1[15], input2[15], select, result[15]),
		 mux17(input1[16], input2[16], select, result[16]),
		 mux18(input1[17], input2[17], select, result[17]),
		 mux19(input1[18], input2[18], select, result[18]),
		 mux20(input1[19], input2[19], select, result[19]),
		 mux21(input1[20], input2[20], select, result[20]),
		 mux22(input1[21], input2[21], select, result[21]),
		 mux23(input1[22], input2[22], select, result[22]),
		 mux24(input1[23], input2[23], select, result[23]),
		 mux25(input1[24], input2[24], select, result[24]),
		 mux26(input1[25], input2[25], select, result[25]),
		 mux27(input1[26], input2[26], select, result[26]),
		 mux28(input1[27], input2[27], select, result[27]),
		 mux29(input1[28], input2[28], select, result[28]),
		 mux30(input1[29], input2[29], select, result[29]),
		 mux31(input1[30], input2[30], select, result[30]),
		 mux32(input1[31], input2[31], select, result[31]);
		 
endmodule