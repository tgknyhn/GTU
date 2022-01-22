module half_adder_tb();
// Registers
reg value1;
reg value2;
// Wires
wire sum;
wire carry_out;

// Calling half adder
half_adder ha(value1, value2, sum, carry_out);

// Delay - Giving Inputs
initial begin
#0  value1 = 1'b0; value2 = 1'b0;
#20 value1 = 1'b0; value2 = 1'b1;
#20 value1 = 1'b1; value2 = 1'b0;
#20 value1 = 1'b1; value2 = 1'b1;
end

// Printing results to the monitor
initial begin
$monitor("Time: %2d, value1: %1b, value2: %1b, sum: %1b, carry_out: %1b", 
			$time, 	
			value1, 
			value2, 
			sum, 
			carry_out);
end

endmodule