`define DELAY 20
module sub_32bit_tb();
// Registers
reg signed [31:0] value1;
reg signed [31:0] value2;
// Wire
wire signed [31:0] result;
wire overflow;

// Calling adder_32bit
sub_32bit sub32(value1, value2, result, overflow);

// Delay - Giving Inputs
initial begin
        value1   = 32'd222222; 
	     value2   = 32'd200000;
	 
#`DELAY value1   = 32'b10000000000000000000000000000000; 
	     value2   = 32'b01111111111111111111111111111111; 
 
#`DELAY value1   = 32'd199999999; 
	     value2   = 32'd1; 

#`DELAY value1   = 32'd90000000; 
	     value2   = 32'd11111111; 

#`DELAY value1   = 32'b00000000000000000000000000000001; 
        value2   = 32'b00000000000000000000000000000010; 

#`DELAY value1   = 32'd98888888; 
        value2   = 32'd10000091; 

#`DELAY value1   = 32'b11111111111111111111111111111110; 
	     value2   = 32'b01010101010101010101010101010101; 

#`DELAY value1   = 32'd100000000; 
        value2   = 32'd200000000; 
end

initial begin
$monitor("\nTime  : %1d\nvalue1: %32b (%8h) (%11d)\nvalue2: %32b (%8h) (%11d)\nresult: %32b (%8h) (%11d) - overflow: %1b", 
			$time, 
			value1, value1, value1, 
			value2, value2, value2,
			result, result, result,
			overflow);
end

endmodule