// scanningFSM.sv
// Max De Somma
// mdesomma@g.hmc.edu
// 11/3/24

module scanningFSM(
	input logic clk, reset,
	input logic [7:0] c,
    output logic done,
	output logic [7:0] r,
    output logic [63:0] chessLayout
);

    logic [7:0] ready;
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
    parameter S8 = 4'b1000;
	parameter S9 = 4'b1001;
	parameter S10 = 4'b1010;
	parameter S11 = 4'b1011;
	parameter S12 = 4'b1100;
	parameter S13 = 4'b1101;
	parameter S14 = 4'b1110;
	parameter S15 = 4'b1111;

    // FSM logic
	always_ff @(posedge clk or posedge reset) begin
		if (reset) begin 
			state <= S0;
			done <= 0;  // Reset done signal to 0 during reset
			ready <= 0;
			chessLayout <= 64'b0; // Reset chessLayout explicitly to zero
		end else begin
			if (ready == 16) begin
				chessLayout <= chessPieces;
				done <= 1;  // Set done when the scanning is completed
				state <= nextState;
				ready <= 0;  // Reset ready after completion
			end else begin 
				state <= nextState;
				ready <= ready + 1;
				done <= 0;  // Ensure done is cleared while in progress
			end
		end
	end

    assign r[0] = ((state == S0) || (state == S1));
    assign r[1] = ((state == S2) || (state == S3));
    assign r[2] = ((state == S4) || (state == S5));
    assign r[3] = ((state == S6) || (state == S7));
    assign r[4] = ((state == S8) || (state == S9));
    assign r[5] = ((state == S10) || (state == S11));
    assign r[6] = ((state == S12) || (state == S13));
    assign r[7] = ((state == S14) || (state == S15));

	always_comb
		case (state)
			//row 0
			S0: begin
                nextState = S1;
				//ready = 0;
            end
            S1: begin
                nextState = S2;
                chessPieces[63:56] = c;
				//ready = 0;
            end

			S2: begin
                nextState = S3;
            end
            S3: begin
                nextState = S4;
                chessPieces[55:48] = c;
            end

			S4: begin 
                nextState = S5;
            end
            S5: begin 
                nextState = S6;
                chessPieces[47:40] = c;
            end

			S6: begin
                nextState = S7;
            end
            S7: begin
                nextState = S8;
                chessPieces[39:32] = c;
            end

			S8: begin
                nextState = S9;
            end
            S9: begin
                nextState = S10;
                chessPieces[31:24] = c;
            end

			S10: begin
                nextState = S11;
            end
            S11: begin
                nextState = S12;
                chessPieces[23:16] = c;
            end

			S12: begin
                nextState = S13;
            end
            S13: begin
                nextState = S14;
                chessPieces[15:8] = c;
            end

			S14: begin
                nextState = S15;
            end
            S15: begin
                nextState = S0;
                chessPieces[7:0] = c;
				// maybe change ready in flip flop
            end
			default: nextState = S0;
		endcase
        
endmodule