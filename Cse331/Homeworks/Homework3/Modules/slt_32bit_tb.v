`define DELAY 20
module slt_32bit_tb();
// Registers
reg signed [31:0] value1;
reg signed [31:0] value2;
// Wire
wire [31:0] slt;

// Calling slt_32bit
slt_32bit slt32(value1, value2, slt);

initial begin
			value1 = 32'h000FFFF0;
			value2 = 32'h000FFFFF;

#`DELAY  value1 = 32'h40000000;
			value2 = 32'h3FFFFFFF;
			
#`DELAY  value1 = 32'h00000002;
			value2 = 32'h00000001;

#`DELAY  value1 = 32'h00000001;
			value2 = 32'h00000002;
			
#`DELAY  value1 = 32'hFFF00000;
			value2 = 32'h00000000;		
end

initial begin
   $monitor("\nTime   : %1d\nvalue1 : %32b (%8h) (%10d)\nvalue2 : %32b (%8h) (%10d)\nslt    : %32b", 
	$time,
	value1, value1, value1,
	value2, value2, value2,
	slt);
end


endmodule