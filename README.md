## Code Overview
The FPGA folder contains the HDL for the FPGA design. The src folder contains the SPI, FSM, and top level module for the FPGA. The sim folder contains the test benches used to test these modules. The MCU folder contains the C design implemented on the MCU. The lib folder contains the header files, and the src folder contains the implementations of the design and its functions. 

### FPGA
The FPGA uses a FSM to interface with the hall effect sensors within a chess board, and sends the data of the board state to the MCU over SPI.

### MCU
The MCU initializes the necessary variables to carry out the logic for a chess game, and uses the board states it receives over SPI to update its variables with the appropriate moves and calculations. 