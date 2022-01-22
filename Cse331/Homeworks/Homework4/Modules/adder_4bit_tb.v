module adder_4bit_tb();
// Registers
reg signed [3:0] value1;
reg signed [3:0] value2;
reg carry_in;
// Wires
wire signed [3:0] sum;
wire carry_out;
wire overflow;

// Calling adder_4bit
adder_4bit add(value1, value2, sum, carry_out, carry_in, overflow);

// Delay - Giving Inputs
initial begin
#0  value1 = 4'b0000; value2 = 4'b0001; carry_in = 1'b0;
#20 value1 = 4'b0010; value2 = 4'b0011; carry_in = 1'b1;
 
#20 value1 = 4'b0100; value2 = 4'b0101; carry_in = 1'b0;
#20 value1 = 4'b0110; value2 = 4'b0111; carry_in = 1'b1;

#20 value1 = 4'b1000; value2 = 4'b1001; carry_in = 1'b0;
#20 value1 = 4'b1010; value2 = 4'b1011; carry_in = 1'b1;

#20 value1 = 4'b1100; value2 = 4'b1101; carry_in = 1'b0; 
#20 value1 = 4'b1110; value2 = 4'b1111; carry_in = 1'b1;
end

initial begin
$monitor("Time: %3d, value1: %4b(%2d), value2: %4b(%2d), carry_in: %1b, sum: %4b(%2d), carry_out: %1b, overflow: %1b", 
			$time, 
			value1, value1, 
			value2, value2, 
			carry_in, 
			sum, sum, 
			carry_out,
			overflow);
end

endmodule