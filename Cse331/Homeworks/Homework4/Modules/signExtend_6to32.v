module signExtend_6to32(value, result);
input  [5:0]  value ;
output [31:0] result;

or or0 (result[0] , value[0], 0),
	or1 (result[1] , value[1], 0),
	or2 (result[2] , value[2], 0),
	or3 (result[3] , value[3], 0),
	or4 (result[4] , value[4], 0),
	or5 (result[5] , value[5], 0),
	or6 (result[6] , value[5], 0),
	or7 (result[7] , value[5], 0),
	or8 (result[8] , value[5], 0),
	or9 (result[9] , value[5], 0),
	or10(result[10], value[5], 0),
	or11(result[11], value[5], 0),
	or12(result[12], value[5], 0),
	or13(result[13], value[5], 0),
	or14(result[14], value[5], 0),
	or15(result[15], value[5], 0),
	or16(result[16], value[5], 0),
	or17(result[17], value[5], 0),
	or18(result[18], value[5], 0),
	or19(result[19], value[5], 0),
	or20(result[20], value[5], 0),
	or21(result[21], value[5], 0),
	or22(result[22], value[5], 0),
	or23(result[23], value[5], 0),
	or24(result[24], value[5], 0),
	or25(result[25], value[5], 0),
	or26(result[26], value[5], 0),
	or27(result[27], value[5], 0),
	or28(result[28], value[5], 0),
	or29(result[29], value[5], 0),
	or30(result[30], value[5], 0),
	or31(result[31], value[5], 0);

endmodule 