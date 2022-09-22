`timescale 1ns / 1ps

module test_bench();
reg [3:0]sw;
wire [7:0]led;
reg clk;
integer i;
initial
begin
    clk=0;
    forever
    #5 clk=~clk;
end
initial
begin
    sw=4'b0000;
    for(i=0;i<16;i=i+1)
    begin
        sw=sw+4'b0001;
        #5;
    end
end
p1 p1(.sw(sw),.clk(clk),.led(led));
endmodule
