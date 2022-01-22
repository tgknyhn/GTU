`define DELAY 20
module ALUcontrol_tb();
// Registers
reg  [2:0] ALUOp;
reg  [2:0] func;
// Wire
wire [2:0] ALUctr;
wire [2:0] dummyData;

// Calling the ALUcontrol module
ALUcontrol ac(ALUOp, func, ALUctr);

// Delay - Giving Inputs
initial begin
		  ALUOp = 3'b000; func = 3'b000;
#`DELAY ALUOp = 3'b000; func = 3'b001;
#`DELAY ALUOp = 3'b000; func = 3'b010;
#`DELAY ALUOp = 3'b000; func = 3'b011;
#`DELAY ALUOp = 3'b000; func = 3'b100;
#`DELAY ALUOp = 3'b000; func = 3'b101;
#`DELAY ALUOp = 3'b001; func = dummyData;
#`DELAY ALUOp = 3'b010; func = dummyData;
#`DELAY ALUOp = 3'b011; func = dummyData;
#`DELAY ALUOp = 3'b100; func = dummyData;
#`DELAY ALUOp = 3'b101; func = dummyData;
#`DELAY ALUOp = 3'b101; func = dummyData;
#`DELAY ALUOp = 3'b110; func = dummyData;
#`DELAY ALUOp = 3'b001; func = dummyData;
#`DELAY ALUOp = 3'b001; func = dummyData;

end

// Printing the results
initial begin
$monitor("Time: %3d | ALUOp: %3b | Function: %3b\nALUctr: %3b\n", $time, ALUOp, func, ALUctr);
end

endmodule 