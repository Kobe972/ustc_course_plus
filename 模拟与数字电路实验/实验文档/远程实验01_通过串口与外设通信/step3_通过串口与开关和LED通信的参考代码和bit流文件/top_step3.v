`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/24 10:33:43
// Design Name: 
// Module Name: top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module top(
input               clk,rst,
input               rx,
output              tx,
output reg  [7:0]   led,
input       [7:0]   sw
);
wire                tx_ready;
wire        [7:0]   tx_data;
wire        [7:0]   rx_data;

rx                  rx_inst(
.clk                (clk),
.rst                (rst),
.rx                 (rx),
.rx_vld             (rx_vld),
.rx_data            (rx_data)
);                     
tx                  tx_inst(
.clk                (clk),
.rst                (rst),
.tx                 (tx ),
.tx_ready           (tx_ready),
.tx_rd              (tx_rd),
.tx_data            (tx_data)
);
assign  tx_ready    = rx_vld;
assign  tx_data     = sw;
always@(posedge clk or posedge rst)
begin
    if(rst)
        led <= 8'h0;
    else if((rx_vld)&&(rx_data!=8'h0a))
        led <= rx_data;  
end
endmodule
