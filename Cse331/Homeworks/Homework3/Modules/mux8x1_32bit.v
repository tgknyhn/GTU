module mux8x1_32bit(input1, input2, input3, input4, input5, input6, input7, input8, select, result);
// Inputs
input [2:0]  select;
input [31:0] input1;
input [31:0] input2;
input [31:0] input3;
input [31:0] input4;
input [31:0] input5;
input [31:0] input6;
input [31:0] input7;
input [31:0] input8;
// Output
output [31:0] result;

// Calling mux8x1 for every bit
mux8x1 mux1  (input1[0]  , input2[0]  , input3[0]  , input4[0]  , input5[0]  , input6[0]  , input7[0]  , input8[0]  , select , result[0]  ),
		 mux2  (input1[1]  , input2[1]  , input3[1]  , input4[1]  , input5[1]  , input6[1]  , input7[1]  , input8[1]  , select , result[1]  ),
		 mux3  (input1[2]  , input2[2]  , input3[2]  , input4[2]  , input5[2]  , input6[2]  , input7[2]  , input8[2]  , select , result[2]  ),
		 mux4  (input1[3]  , input2[3]  , input3[3]  , input4[3]  , input5[3]  , input6[3]  , input7[3]  , input8[3]  , select , result[3]  ),
		 mux5  (input1[4]  , input2[4]  , input3[4]  , input4[4]  , input5[4]  , input6[4]  , input7[4]  , input8[4]  , select , result[4]  ),
		 mux6  (input1[5]  , input2[5]  , input3[5]  , input4[5]  , input5[5]  , input6[5]  , input7[5]  , input8[5]  , select , result[5]  ),
		 mux7  (input1[6]  , input2[6]  , input3[6]  , input4[6]  , input5[6]  , input6[6]  , input7[6]  , input8[6]  , select , result[6]  ),
		 mux8  (input1[7]  , input2[7]  , input3[7]  , input4[7]  , input5[7]  , input6[7]  , input7[7]  , input8[7]  , select , result[7]  ),
		 mux9  (input1[8]  , input2[8]  , input3[8]  , input4[8]  , input5[8]  , input6[8]  , input7[8]  , input8[8]  , select , result[8]  ),
		 mux10 (input1[9]  , input2[9]  , input3[9]  , input4[9]  , input5[9]  , input6[9]  , input7[9]  , input8[9]  , select , result[9]  ),
		 mux11 (input1[10] , input2[10] , input3[10] , input4[10] , input5[10] , input6[10] , input7[10] , input8[10] , select , result[10] ),
		 mux12 (input1[11] , input2[11] , input3[11] , input4[11] , input5[11] , input6[11] , input7[11] , input8[11] , select , result[11] ),
		 mux13 (input1[12] , input2[12] , input3[12] , input4[12] , input5[12] , input6[12] , input7[12] , input8[12] , select , result[12] ),
		 mux14 (input1[13] , input2[13] , input3[13] , input4[13] , input5[13] , input6[13] , input7[13] , input8[13] , select , result[13] ),
		 mux15 (input1[14] , input2[14] , input3[14] , input4[14] , input5[14] , input6[14] , input7[14] , input8[14] , select , result[14] ),
		 mux16 (input1[15] , input2[15] , input3[15] , input4[15] , input5[15] , input6[15] , input7[15] , input8[15] , select , result[15] ),
		 mux17 (input1[16] , input2[16] , input3[16] , input4[16] , input5[16] , input6[16] , input7[16] , input8[16] , select , result[16] ),
		 mux18 (input1[17] , input2[17] , input3[17] , input4[17] , input5[17] , input6[17] , input7[17] , input8[17] , select , result[17] ),
		 mux19 (input1[18] , input2[18] , input3[18] , input4[18] , input5[18] , input6[18] , input7[18] , input8[18] , select , result[18] ),
		 mux20 (input1[19] , input2[19] , input3[19] , input4[19] , input5[19] , input6[19] , input7[19] , input8[19] , select , result[19] ),
		 mux21 (input1[20] , input2[20] , input3[20] , input4[20] , input5[20] , input6[20] , input7[20] , input8[20] , select , result[20] ),
		 mux22 (input1[21] , input2[21] , input3[21] , input4[21] , input5[21] , input6[21] , input7[21] , input8[21] , select , result[21] ),
		 mux23 (input1[22] , input2[22] , input3[22] , input4[22] , input5[22] , input6[22] , input7[22] , input8[22] , select , result[22] ),
		 mux24 (input1[23] , input2[23] , input3[23] , input4[23] , input5[23] , input6[23] , input7[23] , input8[23] , select , result[23] ),
		 mux25 (input1[24] , input2[24] , input3[24] , input4[24] , input5[24] , input6[24] , input7[24] , input8[24] , select , result[24] ),
		 mux26 (input1[25] , input2[25] , input3[25] , input4[25] , input5[25] , input6[25] , input7[25] , input8[25] , select , result[25] ),
		 mux27 (input1[26] , input2[26] , input3[26] , input4[26] , input5[26] , input6[26] , input7[26] , input8[26] , select , result[26] ),
		 mux28 (input1[27] , input2[27] , input3[27] , input4[27] , input5[27] , input6[27] , input7[27] , input8[27] , select , result[27] ),
		 mux29 (input1[28] , input2[28] , input3[28] , input4[28] , input5[28] , input6[28] , input7[28] , input8[28] , select , result[28] ),
		 mux30 (input1[29] , input2[29] , input3[29] , input4[29] , input5[29] , input6[29] , input7[29] , input8[29] , select , result[29] ),
		 mux31 (input1[30] , input2[30] , input3[30] , input4[30] , input5[30] , input6[30] , input7[30] , input8[30] , select , result[30] ),
		 mux32 (input1[31] , input2[31] , input3[31] , input4[31] , input5[31] , input6[31] , input7[31] , input8[31] , select , result[31] );
		 
endmodule