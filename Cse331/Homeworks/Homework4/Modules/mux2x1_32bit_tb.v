`define DELAY 20
module mux2x1_32bit_tb();
// Registers
reg select;
reg [31:0] input1;
reg [31:0] input2;
// wire
wire [31:0] result;

// Calling mux2x1_32bit
mux2x1_32bit mux(input1, input2, select, result);


initial begin
input1 = 32'hAAAAAAAA; 
input2 = 32'h11111111;

$monitor("Inputs(32 bit -> hexadecimal)\n------\n0) %8h \n1) %8h \n",
			input1,
			input2);

select = 1'b0;
#`DELAY
select = 1'b1;
end

initial begin
#5 $monitor("\nTime  : %1d\nselect: %1b\nresult: %8h", $time, select, result);
end

endmodule 