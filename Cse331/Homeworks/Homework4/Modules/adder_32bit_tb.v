`define DELAY 20
module adder_32bit_tb();
// Registers
reg signed [31:0] value1;
reg signed [31:0] value2;
reg carry_in;
// Wires
wire signed [31:0] sum;
wire carry_out;
wire overflow;

// Calling adder_32bit
adder_32bit add(value1, value2, sum, carry_out, carry_in, overflow);

// Delay - Giving Inputs
initial begin
        value1   = 32'd100000; 
	     value2   = 32'd200000; 
	     carry_in = 1'b1;
	 
#`DELAY value1   = 32'b10000000000000000000000000000000; 
	     value2   = 32'b01111111111111111111111111111111; 
	     carry_in = 1'b1;
 
#`DELAY value1   = 32'h12345678; 
	     value2   = 32'h87654321; 
	     carry_in = 1'b0;

#`DELAY value1   = 32'h7FFFFFFF; 
	     value2   = 32'h00000001; 
	     carry_in = 1'b0;

end

initial begin
$monitor("\nTime  : %1d\nvalue1: %32b (%8h) (%11d)\nvalue2: %32b (%8h) (%11d)\nsum   : %32b (%8h) (%11d) \ncarry_in : %1b carry_out: %1b  overflow: %1b", 
			$time, 
			value1, value1, value1, 
			value2, value2, value2,
			sum, sum, sum,
			carry_in, 
			carry_out,
			overflow);
end

endmodule