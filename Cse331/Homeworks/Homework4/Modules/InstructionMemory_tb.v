`define DELAY 10
module InstructionMemory_tb();
// Register
reg [31:0] read_address;
// Wire
wire [15:0] instruction;

// Calling the Instruction Memory module
InstructionMemory im(read_address, instruction);

// Delay - Giving Inputs
initial begin
im.memory[0] = 16'hFFFF;
im.memory[1] = 16'h00FF;
im.memory[2] = 16'hAAAA;

		  read_address = 32'h00000000;
#`DELAY read_address = 32'h00000001;
#`DELAY read_address = 32'h00000002;

end



// Printing the results
initial begin
$monitor("Time        : %2d\nRead Address: %32b\nInstruction : %16b\n", 
			$time, read_address, instruction);
end

endmodule 