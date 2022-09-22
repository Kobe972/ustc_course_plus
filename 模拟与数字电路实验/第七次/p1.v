`timescale 1ns / 1ps

module p1(
input [3:0]sw,input clk,
output [7:0]led
    );
dist_mem_gen_0 dist_mem_gen_0(.a(4'b0),.d(8'b0),.we(1'b0),.clk(clk),.dpra(sw),.dpo(led));
endmodule
