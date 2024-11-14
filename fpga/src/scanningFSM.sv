// scanningFSM.sv
// Max De Somma
// mdesomma@g.hmc.edu
// 11/3/24

module scanningFSM(
	input logic clk, reset,
	input logic [7:0] c,
	output logic [7:0] r,
    output logic [63:0] chessLayout
);

    logic ready = 0;
	logic [3:0] state, nextState, s;

    logic [63:0] chessPieces;

    // defining states
	parameter S0 = 4'b0000;
	parameter S1 = 4'b0001;
	parameter S2 = 4'b0010;
	parameter S3 = 4'b0011;
	parameter S4 = 4'b0100;
	parameter S5 = 4'b0101;
	parameter S6 = 4'b0110;
	parameter S7 = 4'b0111;

    // FSM logic
	always_ff @(posedge clk) begin  //putback reset if needed
		if (reset) state <= S0;
		else state <= nextState;

        if(ready == 1 && state == S0) begin
            chessLayout <= chessPieces;
        end
	end

    assign r[0] = (state == S0);
    assign r[1] = (state == S1);
    assign r[2] = (state == S2);
    assign r[3] = (state == S3);
    assign r[4] = (state == S4);
    assign r[5] = (state == S5);
    assign r[6] = (state == S6);
    assign r[7] = (state == S7);

	always_comb
		case (state)
			//row 0
			S0: begin
                nextState = S1;
                chessPieces[63:56] = c;
            end
			S1: begin
                nextState = S2;
                chessPieces[55:48] = c;
            end
			S2: begin 
                nextState = S3;
                chessPieces[47:40] = c;
            end
			S3: begin
                nextState = S4;
                chessPieces[39:32] = c;
            end
			S4: begin
                nextState = S5;
                chessPieces[31:24] = c;
            end
			S5: begin
                nextState = S6;
                chessPieces[23:16] = c;
            end
			S6: begin
                nextState = S7;
                chessPieces[15:8] = c;
            end
			S7: begin
                nextState = S0;
                chessPieces[7:0] = c;
                ready = ready + 1;
            end
			default: nextState = S0;
		endcase
        
endmodule