module ALUcontrol(ALUOp, func, ALUctr);
// Inputs
input  [2:0] ALUOp;
input  [2:0] func;
// Output
output [2:0] ALUctr;
// Wires - Condition wires for ALUctr
wire ctr2_and0, ctr2_and1, ctr2_and2, ctr2_and3;
wire ctr1_and0, ctr1_and1, ctr1_and2;
wire ctr0_and0, ctr0_and1;
// Wires - Not wires for ALUOp and func
wire [2:0] ALUOpNot;
wire [2:0] funcNot;

// First assigning "not" values
not not0(ALUOpNot[0], ALUOp[0]),
	 not1(ALUOpNot[1], ALUOp[1]),
	 not2(ALUOpNot[2], ALUOp[2]),
	 not3(funcNot[0], func[0]),
	 not4(funcNot[1], func[1]),
	 not5(funcNot[2], func[2]);

// Determining "and" conditions for ALUctr[2] 
and and0(ctr2_and0, ALUOpNot[2], ALUOpNot[1], ALUOpNot[0], func[2], funcNot[1]), // P2'.P1'.P0'.F2.F1'
	 and1(ctr2_and1, ALUOpNot[2], ALUOp[1], ALUOp[0]),                            // P2'.P1.P0
	 and2(ctr2_and2, ALUOp[2], ALUOpNot[1], ALUOpNot[0]),                         // P2.P1'.P0' 
	 and3(ctr2_and3, ALUOp[2], ALUOp[1], ALUOpNot[0]);                            // P2.P1.P0'
	 
// Determining "and" conditions for ALUctr[1] 
and and4(ctr1_and0, ALUOpNot[2], ALUOpNot[1], ALUOpNot[0], funcNot[2], func[1]), // P2'.P1'.P0'.F2'.F1
	 and5(ctr1_and1, ALUOp[2], ALUOpNot[1], ALUOp[0]),                            // P2.P1'.P0
	 and6(ctr1_and2, ALUOp[2], ALUOp[1], ALUOpNot[0]);                            // P2.P1.P0' 
	 
// Determining "and" conditions for ALUctr[0]
and and8(ctr0_and0, ALUOpNot[2], ALUOpNot[1], ALUOpNot[0], func[0]), // P2'.P1'.P0'.F0
	 and9(ctr0_and1, ALUOpNot[2], ALUOp[0]);                          // P2'.P0
	 
// Finally, calculating the ALUctr values
or	or0(ALUctr[2], ctr2_and0, ctr2_and1, ctr2_and2, ctr2_and3),
	or1(ALUctr[1], ctr1_and0, ctr1_and1, ctr1_and2),
	or2(ALUctr[0], ctr0_and0, ctr0_and1);
	 
endmodule 