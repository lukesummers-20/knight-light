// boardSPI-TB.sv
// Max De Somma
// mdesomma@g.hmc.edu
// 11/19/24


module boardSPITB();
 // Set up test signals

	logic clk, sdo, done;
	logic [63:0] chessLayout;

 // Instantiate the device under test
 boardSPI dut(.sck(clk), .sdo(sdo), .done(done), .boardState(chessLayout));


 always
   begin
     clk = 1; #5;
     clk = 0; #5;
   end

initial
   begin

	chessLayout = 64'b11111111_11111111_00000000_00000000_00000000_00000000_11111111_11111111;

	done = 0; #27; done = 1;
	
	// test different inputs
	
    end
endmodule
