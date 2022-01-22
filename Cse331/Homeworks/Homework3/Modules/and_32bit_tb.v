`define DELAY 20
module and_32bit_tb();
// Registers
reg [31:0] value1;
reg [31:0] value2;
// Wire
wire [31:0] result;

// Calling and_32bit
and_32bit and32(value1, value2, result);

// Delay - Giving Inputs
initial begin
        value1   = 32'hF000F000; 
	     value2   = 32'h0F001000; 
	 
#`DELAY value1   = 32'hFFFF0000; 
	     value2   = 32'h0000FFFF; 
 
#`DELAY value1   = 32'h0F0F0F0F; 
	     value2   = 32'h0A0A0A0A; 

#`DELAY value1   = 32'hAAAAAAAA; 
        value2   = 32'hFFFFFFFF;  
end

initial begin
   $monitor("\nTime   : %1d\nvalue1 : %32b (%8h) (%10d)\nvalue2 : %32b (%8h) (%10d)\nresult : %32b (%8h) (%10d)", 
	$time,
	value1, value1, value1,
	value2, value2, value2,
	result, result, result);
end


endmodule