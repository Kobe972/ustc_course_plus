`timescale 1ns / 1ps

module p3(
input clk,rst,
output reg [2:0]hexplay_an,
output reg [3:0]hexplay_data);
wire locked;
reg [23:0]timer_cnt;
reg [15:0]data;
reg [32:0]hexplay_cnt;

always@(posedge clk)
begin
    if(hexplay_cnt>=(20000000/8))
        hexplay_cnt<=0;
    else
        hexplay_cnt<=hexplay_cnt+1;
    if(rst==1)
    begin
        timer_cnt<=0;
        hexplay_cnt<=0;
        hexplay_an<=0;
        data<=0;
        hexplay_data<=16'b0001001000110100;
    end
    if(hexplay_cnt==0)
    begin
        if(hexplay_an==3'b011)
            hexplay_an<=0;
        else
            hexplay_an<=hexplay_an+1;
    end
    case(hexplay_an)
        0:hexplay_data<=data[3:0];
        1:hexplay_data<=data[7:4];
        2:hexplay_data<=data[11:8];
        3:hexplay_data<=data[15:12];
        default:hexplay_data=0;
    endcase
    if(timer_cnt==0)
    begin
        data<=data+1;
        if(data[3:0]==10)
        begin
            data[3:0]<=4'b0000;
            data[7:4]<=data[7:4]+1;
        end
        if(data[7:4]==10)
        begin
            data[7:4]<=4'b0000;
            data[11:8]<=data[11:8]+1;
        end
        if(data[11:8]==6)
        begin
            data[11:8]<=4'b0000;
            data[15:12]<=data[15:12]+1;
        end
        if(data[15:12]==10)
            data[15:12]<=4'b0000;
    end
    if(timer_cnt>=10000000)
        timer_cnt<=0;
    else
        timer_cnt<=timer_cnt+1;
end
endmodule
