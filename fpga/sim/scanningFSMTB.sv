// scanningFSM-TB.sv
// Max De Somma
// mdesomma@g.hmc.edu
// 11/17/24

// This is a test bench module that tests the chess scanning is correctly switched 
module scanningFSMTB();
 // Set up test signals

	logic clk, reset;
	logic [7:0] c;
	logic [7:0] r;
	logic [63:0] chessLayout;

 // Instantiate the device under test
 scanningFSM dut(.clk(clk), .reset(reset), .c(c), .r(r), .chessLayout(chessLayout));


 always
   begin
     clk = 1; #5;
     clk = 0; #5;
   end

initial
   begin
	reset = 1; #27; reset = 0;
	
	// test different inputs
	c = 8'b11111111;
	#10;
	c = 8'b11110111;
	#10;
	c = 8'b00001000;
	#10;
	c = 8'b00000000;
	#10
	c = 8'b00000000;
	#10
	c = 8'b00000000;
	#10
	c = 8'b11111111;
	#10
	c = 8'b11111111;
	#10;
    end
endmodule
