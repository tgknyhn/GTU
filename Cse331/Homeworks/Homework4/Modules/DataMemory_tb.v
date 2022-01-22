`timescale 1ns / 1ns
`define DELAY 10
module DataMemory_tb();
// Registers
reg MemWrite, MemRead;
reg clock;
reg [31:0] address;
reg [31:0] write_data;
// Wire
wire [31:0] read_data;

// Calling the DataMemory module
DataMemory dm(MemWrite, MemRead, address, write_data, read_data, clock);

initial begin
	clock = 1'b0;
	forever #2 clock = ~clock;
end


// Delay - Giving Inputs
initial begin
// reg($1) = 0xFFFFFFF1
address    = 32'b00000000000000000000000000000001;
write_data = 32'hFFFFFFF1;

        MemWrite = 1'b1; MemRead = 1'b0; // Writing to memory
#`DELAY MemWrite = 1'b0; MemRead = 1'b1; // Reading from the memory

end

// Printing the results
initial begin
$monitor("Time: %3d | Clock: %1d | MemWrite: %1b | MemRead: %1b\nAddress   : %32b\nWrite Data: %32b\nRead Data : %32b", 
			$time, clock, MemWrite, MemRead, 
			address, write_data, read_data);
end

endmodule 