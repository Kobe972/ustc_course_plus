`timescale 1ns / 1ps

module test_bench();
reg clk;
reg rst;
wire [2:0]hexplay_an;
wire [3:0]hexplay_data;
initial
forever
    #1 clk=~clk;
initial
begin
    clk=0;
    rst=1;
    #5 rst=0;
    
end

p3 p3(.clk(clk),.rst(rst),.hexplay_an(hexplay_an),.hexplay_data(hexplay_data));
endmodule
