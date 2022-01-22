module adder_32bit(value1, value2, sum, carry_out, carry_in, overflow);
// Inputs
input [31:0] value1;
input [31:0] value2;
input carry_in;
// Outputs
output [31:0] sum;
output carry_out;
output overflow;
// Wires - intermediate carries
wire c_4_7;
wire c_8_11;
wire c_12_15;
wire c_16_19;
wire c_20_23;
wire c_24_27;
wire c_28_31;
wire temp_overflow;

// Calling adder_4bit 8 times
adder_4bit add_1(value1[3:0]  , value2[3:0]  , sum[3:0],   c_4_7  ,   carry_in, temp_overflow),
			  add_2(value1[7:4]  , value2[7:4]  , sum[7:4],   c_8_11 ,   c_4_7   , temp_overflow),
			  add_3(value1[11:8] , value2[11:8] , sum[11:8],  c_12_15,   c_8_11  , temp_overflow),
			  add_4(value1[15:12], value2[15:12], sum[15:12], c_16_19,   c_12_15 , temp_overflow),
			  add_5(value1[19:16], value2[19:16], sum[19:16], c_20_23,   c_16_19 , temp_overflow),
			  add_6(value1[23:20], value2[23:20], sum[23:20], c_24_27,   c_20_23 , temp_overflow),
			  add_7(value1[27:24], value2[27:24], sum[27:24], c_28_31,   c_24_27 , temp_overflow),
			  add_8(value1[31:28], value2[31:28], sum[31:28], carry_out, c_28_31 , overflow);

endmodule