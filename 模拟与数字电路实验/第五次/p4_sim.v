`timescale 1ns / 1ps
module test_bench();
reg [2:0] in;
wire [7:0] out;
decoder decoder(.in(in),.out(out));
initial
begin
    in=3'b000; #20;
    in=3'b001; #20;
    in=3'b010; #20;
    in=3'b011; #20;
    in=3'b100; #20;
    in=3'b101; #20;
    in=3'b110; #20;
    in=3'b111; #20;
    $finish;
end
endmodule
