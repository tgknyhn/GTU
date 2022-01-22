module nextPC(branch, branchAddress, NextPC, clock);
// Inputs
input branch, clock;
input [31:0] branchAddress;
// Output
output reg [31:0] NextPC;

initial begin
	NextPC = 32'h00000000;
end

always @ (posedge clock) begin
	if(branch)
		NextPC = NextPC + 1 + branchAddress;   // If branch signal is 1, then add branchAddress to nextPC
	else 
		NextPC = NextPC + 1;
end

endmodule 