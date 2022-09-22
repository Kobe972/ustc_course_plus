`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/03/26 16:23:39
// Design Name: 
// Module Name: tb
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


module tb();
reg             clk,rst;
wire            fpga_rx,fpga_tx;
wire            tb_rd_vld;
wire    [7:0]   tb_rd_data;
reg     [7:0]   tb_wr_data;
reg             tb_wr_ready;
wire            tb_wr_en;
reg     [7:0]   sw;
task putchar;
input     [7:0] char;
    begin
    @(posedge clk);
    tb_wr_ready    = 1'b1;
    tb_wr_data  = char;
    wait(tb_wr_en);
    @(posedge clk);
    tb_wr_ready    = 1'b0;
    end
endtask
initial
begin
    #10 rst = 1;
    #100 rst    = 0;
end
initial
begin
    clk = 0;
    forever
    #5 clk = ~clk;
end

initial
begin
    sw  = 8'h0f;
end

initial
begin
#1000;
    putchar("r");
    putchar("b");
    putchar(" ");
    putchar("0");
    putchar("1");
    putchar(8'h0a);
    #1000000;
    putchar(8'h0a);
    #1000000;
    putchar("r");
    putchar("b");
    putchar(" ");
    putchar("0");
    putchar("1");
    putchar(8'h0a);
end

top     top(
.clk    (clk),
.rst    (rst),
.rx     (fpga_rx),
.tx     (fpga_tx),
.led    (),
.sw     (sw)
);
rx                  tb_rx(
.clk                (clk),
.rst                (rst),
.rx                 (fpga_tx),
.rx_vld             (tb_rd_vld),
.rx_data            (tb_rd_data)
);                     
tx                  tb_tx(
.clk                (clk),
.rst                (rst),
.tx                 (fpga_rx ),
.tx_ready           (tb_wr_ready),
.tx_rd              (tb_wr_en),
.tx_data            (tb_wr_data)
);



endmodule
