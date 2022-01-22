module full_adder_tb();
// Registers
reg value1;
reg value2;
reg carry_in;
// Wires
wire sum;
wire carry_out;

// Calling full adder
full_adder fa(value1, value2, sum, carry_out, carry_in);

// Delay - Giving Inputs
initial begin
#0  value1 = 1'b0; value2 = 1'b0; carry_in = 1'b0;
#20 value1 = 1'b0; value2 = 1'b0; carry_in = 1'b1;
 
#20 value1 = 1'b0; value2 = 1'b1; carry_in = 1'b0;
#20 value1 = 1'b0; value2 = 1'b1; carry_in = 1'b1;

#20 value1 = 1'b1; value2 = 1'b0; carry_in = 1'b0;
#20 value1 = 1'b1; value2 = 1'b0; carry_in = 1'b1;

#20 value1 = 1'b1; value2 = 1'b1; carry_in = 1'b0; 
#20 value1 = 1'b1; value2 = 1'b1; carry_in = 1'b1;
end


// Printing results to the monitor
initial begin
$monitor("Time: %3d, value1: %1b, value2: %1b, carry_in: %1b, sum: %1b, carry_out: %1b", 
			$time, 
			value1, 
			value2, 
			carry_in, 
			sum, 
			carry_out);
end

endmodule