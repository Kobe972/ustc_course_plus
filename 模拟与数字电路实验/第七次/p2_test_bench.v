`timescale 1ns / 1ps

module test_banch();
reg clk;
reg [7:0]sw;
wire [2:0]an;
wire [3:0]hexplay_data;
integer i;
initial
begin
    sw=0;
    for(i=8'b0;i<=8'b11111111;i=i+1)
    begin
        sw=sw+1;
        #10;
    end
end
initial
begin
    clk=0;
    forever
    #1 clk=~clk;
end
p2 p2(.sw(sw),.clk(clk),.an(an),.hexplay_data(hexplay_data));
endmodule
