module nor_32bit(value1, value2, result);
// Inputs
input [31:0] value1;
input [31:0] value2;
// Output
output [31:0] result;

// Using nor gate 32 times
nor nor_0  (result[0]  , value1[0]  , value2[0]),
	 nor_1  (result[1]  , value1[1]  , value2[1]),
	 nor_2  (result[2]  , value1[2]  , value2[2]),
	 nor_3  (result[3]  , value1[3]  , value2[3]),
	 nor_4  (result[4]  , value1[4]  , value2[4]),
	 nor_5  (result[5]  , value1[5]  , value2[5]),
	 nor_6  (result[6]  , value1[6]  , value2[6]),
	 nor_7  (result[7]  , value1[7]  , value2[7]),
	 nor_8  (result[8]  , value1[8]  , value2[8]),
	 nor_9  (result[9]  , value1[9]  , value2[9]),
	 nor_10 (result[10] , value1[10] , value2[10]),
	 nor_11 (result[11] , value1[11] , value2[11]),
	 nor_12 (result[12] , value1[12] , value2[12]),
	 nor_13 (result[13] , value1[13] , value2[13]),
	 nor_14 (result[14] , value1[14] , value2[14]),
	 nor_15 (result[15] , value1[15] , value2[15]),
	 nor_16 (result[16] , value1[16] , value2[16]),
	 nor_17 (result[17] , value1[17] , value2[17]),
	 nor_18 (result[18] , value1[18] , value2[18]),
	 nor_19 (result[19] , value1[19] , value2[19]),
	 nor_20 (result[20] , value1[20] , value2[20]),
	 nor_21 (result[21] , value1[21] , value2[21]),
	 nor_22 (result[22] , value1[22] , value2[22]),
	 nor_23 (result[23] , value1[23] , value2[23]),
	 nor_24 (result[24] , value1[24] , value2[24]),
	 nor_25 (result[25] , value1[25] , value2[25]),
	 nor_26 (result[26] , value1[26] , value2[26]),
	 nor_27 (result[27] , value1[27] , value2[27]),
	 nor_28 (result[28] , value1[28] , value2[28]),
	 nor_29 (result[29] , value1[29] , value2[29]),
	 nor_30 (result[30] , value1[30] , value2[30]),
	 nor_31 (result[31] , value1[31] , value2[31]);

endmodule