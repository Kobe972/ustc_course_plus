`timescale 1ns / 1ps

module p2(
input [7:0]sw,
input clk,
output reg [2:0]an,
output reg [3:0]hexplay_data);
reg [32:0] hexplay_cnt;
always@(posedge clk) begin
	if (hexplay_cnt >= (2000000 / 8))
		hexplay_cnt <= 0;
	else
		hexplay_cnt <= hexplay_cnt + 1;
end
always@(posedge clk)
begin
    if(hexplay_cnt==0)
    begin
        if(an==3'b000)
            an<=3'b001;
        else
            an<=3'b000;
    end
end
always@(posedge clk)
begin
    if(an==3'b000)
        hexplay_data<=sw[3:0];
    else
        hexplay_data<=sw[7:4];
end 
endmodule
