module nextPC_tb();
// Registers
reg branch, clock;
reg [31:0] branchAddress;
// Wire
wire [31:0] nextPC;

// Calling the nextPC module
nextPC next(branch, branchAddress, nextPC, clock);

initial begin
	clock = 1'b0;
	forever #2 clock = ~clock;
end

// Giving Inputs
initial begin
// First it starts from pc = 32'd0 without branch signal
branch        = 1'b0;
branchAddress = 32'd24;
#8;
// Now giving a branch signal to it
branch = 1'b1;
#4 
// Incrementing once more
branch = 1'b0;
#4;
end

always @(posedge clock) begin
	$display("Time: %3d | Clock: %1b | Branch: %1b\nBranch Address: %32b (%2h)\nNext PC       : %32b (%2h)\n", 
			$time, clock, branch, 
			branchAddress, branchAddress,
			nextPC, nextPC);
	if(next.NextPC > 40)
		$finish;
end

endmodule 