module not_32bit(value, result);
// Input
input [31:0] value;
// Output
output [31:0] result;

// Using not gate 32 times
not not_0  (result[0]  , value[0]  ),
	 not_1  (result[1]  , value[1]  ),
	 not_2  (result[2]  , value[2]  ),
	 not_3  (result[3]  , value[3]  ),
	 not_4  (result[4]  , value[4]  ),
	 not_5  (result[5]  , value[5]  ),
	 not_6  (result[6]  , value[6]  ),
	 not_7  (result[7]  , value[7]  ),
	 not_8  (result[8]  , value[8]  ),
	 not_9  (result[9]  , value[9]  ),
	 not_10 (result[10] , value[10] ),
	 not_11 (result[11] , value[11] ),
	 not_12 (result[12] , value[12] ),
	 not_13 (result[13] , value[13] ),
	 not_14 (result[14] , value[14] ),
	 not_15 (result[15] , value[15] ),
	 not_16 (result[16] , value[16] ),
	 not_17 (result[17] , value[17] ),
	 not_18 (result[18] , value[18] ),
	 not_19 (result[19] , value[19] ),
	 not_20 (result[20] , value[20] ),
	 not_21 (result[21] , value[21] ),
	 not_22 (result[22] , value[22] ),
	 not_23 (result[23] , value[23] ),
	 not_24 (result[24] , value[24] ),
	 not_25 (result[25] , value[25] ),
	 not_26 (result[26] , value[26] ),
	 not_27 (result[27] , value[27] ),
	 not_28 (result[28] , value[28] ),
	 not_29 (result[29] , value[29] ),
	 not_30 (result[30] , value[30] ),
	 not_31 (result[31] , value[31] );

endmodule