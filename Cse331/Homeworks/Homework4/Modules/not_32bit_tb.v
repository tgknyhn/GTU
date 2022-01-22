`define DELAY 20
module not_32bit_tb();
// Register
reg [31:0] value;
// Wire
wire [31:0] result;

// Calling not_32bit
not_32bit not32(value, result);

// Delay - Giving Inputs
initial begin
        value   = 32'hF000F000; 
	 
#`DELAY value   = 32'hFFFF0000; 
 
#`DELAY value   = 32'h0F0F0F0F; 

#`DELAY value   = 32'hAAAAAAAA; 
end

initial begin
   $monitor("\nTime   : %1d\nvalue  : %32b (%8h) (%10d)\nresult : %32b (%8h) (%10d)", 
	$time,
	value , value , value,
	result, result, result);
end


endmodule