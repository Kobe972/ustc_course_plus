`timescale 1ns / 1ps
module test_bench();
reg clk,rst_n,d;
wire q;
d_ff_r d_ff_r(.clk(clk),.rst_n(rst_n),.d(d),.q(q));
initial clk=0;
always #5 clk=~clk;
initial
begin
    rst_n=0;
    d=0;
end
initial
begin
    #27;
    rst_n=1;
end
initial
begin
    #13;
    d=1;
    #24;
    d=0;
end
endmodule
