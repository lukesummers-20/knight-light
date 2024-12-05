// boardSPI.sv
// Max De Somma
// mdesomma@g.hmc.edu
// 11/19/24

module boardSPI(
	input  logic sck, 
    output logic sdo,
    input  logic done,
    input  logic [63:0] boardState);


    logic        sdodelayed, wasdone;
    logic [63:0] boardStatecaptured;
               
    // assert load
    // then deassert load, wait until done
    // then apply 64 sclks to shift out boardstate
    // SPI mode is equivalent to cpol = 0, cpha = 0 since data is sampled on first edge and the first
    // edge is a rising edge (clock going from low in the idle state to high).
    always_ff @(posedge sck)
        if (!wasdone) boardStatecaptured = boardState;
        else          {boardStatecaptured} = {boardStatecaptured[62:0], 1'b1};         
    
    // sdo should change on the negative edge of sck
    always_ff @(negedge sck) begin
        wasdone = done;
        sdodelayed = boardStatecaptured[62];
    end
    
    // when done is first asserted, shift out msb before clock edge
    assign sdo = (done & !wasdone) ? boardState[63] : sdodelayed;
endmodule
