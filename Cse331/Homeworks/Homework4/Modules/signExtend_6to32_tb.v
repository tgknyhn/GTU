module signExtend_6to32_tb();
reg [5:0]  value;
wire[31:0] result;

// Calling the sign extend module
signExtend_6to32 extend(value, result);

initial begin
	 value = 6'b011101;
#20 value = 6'b100010;
end

initial begin
$monitor("Time  : %2d\nValue : %32b\nResult: %32b\n", $time, value, result);
end


endmodule 