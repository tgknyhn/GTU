module shiftLeft_32bit_tb();
reg  [31:0] value;
wire [31:0] result;

// Calling the shift left operator
shiftLeft_32bit sl32(value, result);

initial begin
	 value = 32'hAAAAAAAA;
#20 value = 32'hCCCC1234;
end

initial begin
$monitor("Time  : %2d\nValue : %32b\nResult: %32b\n", $time, value, result);
end

endmodule 