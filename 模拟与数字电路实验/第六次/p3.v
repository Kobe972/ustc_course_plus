`timescale 1ns / 1ps
module counter(
input clk,
output reg [7:0]led);
reg [31:0] number;
always@(posedge clk)
begin
    led<=number[31:24];
    number<=number+1;
end
endmodule
