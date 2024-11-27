// scanningFSM.sv
// Max De Somma
// mdesomma@g.hmc.edu
// 11/3/24

module knightLightTop(
	input logic  load, sck, 
	input logic [7:0] c,
	output logic [7:0] r,
	output logic led,
    output logic sdo,
	output logic done
);

    HSOSC hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));  
	logic reset;
	logic [24:0] counter = 0;
	logic toggle = 0;
	logic [63:0] chessLayout;
	always_ff @(posedge int_osc) begin
            counter <= counter + 1;
			if(counter == 300000) begin
					counter <= 0;
					toggle <= ~toggle;
			end
		end
		
	
	assign led = c[7];
	
	scanningFSM a1(toggle, load, c, done, r, chessLayout);
	boardSPI a2(sck, sdo, done, chessLayout);

        
endmodule