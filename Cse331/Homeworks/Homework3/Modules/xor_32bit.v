module xor_32bit(value1, value2, result);
// Inputs
input [31:0] value1;
input [31:0] value2;
// Output
output [31:0] result;

// Using xor gate 32 times
xor xor_0  (result[0]  , value1[0]  , value2[0]),
	 xor_1  (result[1]  , value1[1]  , value2[1]),
	 xor_2  (result[2]  , value1[2]  , value2[2]),
	 xor_3  (result[3]  , value1[3]  , value2[3]),
	 xor_4  (result[4]  , value1[4]  , value2[4]),
	 xor_5  (result[5]  , value1[5]  , value2[5]),
	 xor_6  (result[6]  , value1[6]  , value2[6]),
	 xor_7  (result[7]  , value1[7]  , value2[7]),
	 xor_8  (result[8]  , value1[8]  , value2[8]),
	 xor_9  (result[9]  , value1[9]  , value2[9]),
	 xor_10 (result[10] , value1[10] , value2[10]),
	 xor_11 (result[11] , value1[11] , value2[11]),
	 xor_12 (result[12] , value1[12] , value2[12]),
	 xor_13 (result[13] , value1[13] , value2[13]),
	 xor_14 (result[14] , value1[14] , value2[14]),
	 xor_15 (result[15] , value1[15] , value2[15]),
	 xor_16 (result[16] , value1[16] , value2[16]),
	 xor_17 (result[17] , value1[17] , value2[17]),
	 xor_18 (result[18] , value1[18] , value2[18]),
	 xor_19 (result[19] , value1[19] , value2[19]),
	 xor_20 (result[20] , value1[20] , value2[20]),
	 xor_21 (result[21] , value1[21] , value2[21]),
	 xor_22 (result[22] , value1[22] , value2[22]),
	 xor_23 (result[23] , value1[23] , value2[23]),
	 xor_24 (result[24] , value1[24] , value2[24]),
	 xor_25 (result[25] , value1[25] , value2[25]),
	 xor_26 (result[26] , value1[26] , value2[26]),
	 xor_27 (result[27] , value1[27] , value2[27]),
	 xor_28 (result[28] , value1[28] , value2[28]),
	 xor_29 (result[29] , value1[29] , value2[29]),
	 xor_30 (result[30] , value1[30] , value2[30]),
	 xor_31 (result[31] , value1[31] , value2[31]);

endmodule