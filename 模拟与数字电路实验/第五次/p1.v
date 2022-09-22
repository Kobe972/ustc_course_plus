`timescale 1ns / 1ps
module test_bench();
reg a,b;
initial
begin
    a=1;
    b=0;
end
initial
begin
    #200;
    a=0;
end
initial
begin
    #100;
    b=1;
    #175;
    b=0;
    #75;
    b=1;
end
endmodule
