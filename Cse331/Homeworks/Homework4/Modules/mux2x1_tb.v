`define DELAY 20
module mux2x1_tb();
// Registers
reg input1;
reg input2;
reg select;
// Wire
wire result;

// Calling mux2x1
mux2x1 mux(input1, input2, select, result);

initial begin
input1 = 1'b1; 
input2 = 1'b0;

select = 1'b0;
#`DELAY
select = 1'b1;
end


initial begin
$monitor("\nTime  : %1d\ninput1: %1b  -  input2: %1b\nselect: %1b\nresult: %1b",
			$time,
			input1,
			input2,
			select,
			result);
end

endmodule