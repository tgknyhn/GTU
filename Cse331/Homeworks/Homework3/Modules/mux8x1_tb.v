`define DELAY 20
module mux8x1_tb();
// registers
reg [2:0] select;
reg input1;
reg input2;
reg input3;
reg input4;
reg input5;
reg input6;
reg input7;
reg input8;
// wire
wire result;

// Calling 8x1 mux
mux8x1 mux(input1, input2, input3, input4, input5, input6, input7, input8, select, result);

initial begin
input1 = 1'b1; 
input2 = 1'b1;
input3 = 1'b1;
input4 = 1'b0;
input5 = 1'b0;
input6 = 1'b1;
input7 = 1'b0;
input8 = 1'b1;

select = 3'b000;
#`DELAY
select = 3'b001;
#`DELAY
select = 3'b010;
#`DELAY
select = 3'b011;
#`DELAY
select = 3'b100;
#`DELAY
select = 3'b101;
#`DELAY
select = 3'b110;
#`DELAY
select = 3'b111;
end

initial begin
$monitor("\nTime  : %1d\ninput0: %1b - input1: %1b - input2: %1b - input3: %1b - input4: %1b - input5: %1b - input6: %1b - input7: %1b\nselect: %3b\nresult: %1b",
			$time,
			input1, input2, input3, input4, input5, input6, input7, input8,
			select,
			result);
end

endmodule