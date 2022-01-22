`define DELAY 20
module mux8x1_32bit_tb();
// Registers
reg [2:0]  select;
reg [31:0] input1;
reg [31:0] input2;
reg [31:0] input3;
reg [31:0] input4;
reg [31:0] input5;
reg [31:0] input6;
reg [31:0] input7;
reg [31:0] input8;
// wire
wire [31:0] result;

// Calling mux8x1_32bit
mux8x1_32bit mux(input1, input2, input3, input4, input5, input6, input7, input8, select, result);


initial begin
input1 = 32'hFFFFFFFF; 
input2 = 32'hEEEEEEEE;
input3 = 32'hDDDDDDDD;
input4 = 32'hCCCCCCCC;
input5 = 32'hBBBBBBBB;
input6 = 32'hAAAAAAAA;
input7 = 32'h99999999;
input8 = 32'h88888888;

$monitor("Inputs(32 bit -> hexadecimal)\n------\n0) %8h \n1) %8h \n2) %8h \n3) %8h \n4) %8h \n5) %8h \n6) %8h \n7) %8h \n",
			input1,
			input2,
			input3,
			input4,
			input5,
			input6,
			input7,
			input8);

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
#5 $monitor("\nTime  : %1d\nselect: %3b\nresult: %8h", $time, select, result);
end


endmodule
