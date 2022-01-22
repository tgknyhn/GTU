module MiniMIPS_tb();
// Clock Signal
reg clock;
reg [31:0] r_types = 32'd12;
reg [31:0] i_types = 32'd30;


// Calling the MiniMIPS module
MiniMIPS mips(clock);

initial begin
	$readmemb("instruction.txt", mips.im.memory);
	$readmemb("register.txt",    mips.rm.registers);
	$readmemb("data.txt",        mips.data.memory);
end

initial begin
	clock = 1'b0;
	forever #2 clock = ~clock;
end

always @(posedge clock) begin
	if(r_types > mips.NextPC) begin
		$display("Time  : %4d | Clock: %1d    | PC(int): %2d | Instruction: %16b | ALUctr: %3b", 
					 $time, 
					 clock, 
					 mips.NextPC, 
					 mips.instruction,
					 mips.ALUctr);
		$display("Opcode: %4b | Rs: %3b($%1d) | Rt: %3b($%1d) | Rd: %3b($%1d) | Func: %3b",
					 mips.instruction[15:12], 
					 mips.instruction[11:9], mips.instruction[11:9],
					 mips.instruction[8:6], mips.instruction[8:6],
					 mips.instruction[5:3], mips.instruction[5:3], 
					 mips.instruction[2:0]);
		
		if(mips.NextPC >= 0 && mips.NextPC < 2)
			$display("Operation: AND");
		else if(mips.NextPC >= 2 && mips.NextPC < 4)
			$display("Operation: ADD");
		else if(mips.NextPC >= 4 && mips.NextPC < 6)
			$display("Operation: SUB");
		else if(mips.NextPC >= 6 && mips.NextPC < 8)
			$display("Operation: XOR");
		else if(mips.NextPC >= 8 && mips.NextPC < 10)
			$display("Operation: NOR");
		else if(mips.NextPC >= 10 && mips.NextPC < 12)
			$display("Operation: OR");
		
		$display("Value 1: %32b\nValue 2: %32b\nResult : %32b",
					 mips.read_data1, 
					 mips.value2, 
					 mips.result);
	end
	else if(i_types > mips.NextPC) begin
		$display("Time  : %4d | Clock: %1d    | PC(int): %2d | Instruction: %16b | ALUctr: %3b", 
					 $time, 
					 clock, 
					 mips.NextPC, 
					 mips.instruction,
					 mips.ALUctr);
		$display("Opcode: %4b | Rs: %3b($%1d) | Rt: %3b($%1d) | Immediate  : %6b",
					 mips.instruction[15:12], 
					 mips.instruction[11:9], mips.instruction[11:9],
					 mips.instruction[8:6], mips.instruction[8:6],
					 mips.instruction[5:0]);

	   if(mips.NextPC >= 12 && mips.NextPC < 14)
			$display("Operation: ADDI");
		else if(mips.NextPC >= 14 && mips.NextPC < 16)
			$display("Operation: ANDI");
		else if(mips.NextPC >= 16 && mips.NextPC < 18)
			$display("Operation: ORI");
		else if(mips.NextPC >= 18 && mips.NextPC < 20)
			$display("Operation: NORI");
		else if(mips.NextPC >= 20 && mips.NextPC < 22)
			$display("Operation: BEQ");
		else if(mips.NextPC >= 22 && mips.NextPC < 24)
			$display("Operation: BNE");
		else if(mips.NextPC >= 24 && mips.NextPC < 26)
			$display("Operation: SLTI");
		else if(mips.NextPC >= 26 && mips.NextPC < 28)
			$display("Operation: LW");
		else if(mips.NextPC >= 28 && mips.NextPC < 30)
			$display("Operation: SW");
		
		$display("Value 1: %32b\nValue 2: %32b\nResult : %32b",
					 mips.read_data1, 
					 mips.value2, 
					 mips.result);
	end
	else begin
		$writememb("registerOut.txt", mips.rm.registers);
		$writememb("dataOut.txt",     mips.data.memory);
		$finish;
	end
	
end

endmodule 