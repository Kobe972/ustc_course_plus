module test(
input clk,rst,timer_pulse
output led);
reg [1:0]curr_state;
reg [1:0]next_state;
always@(*)
begin
	if(timer_pulse)
	begin
		case(curr_state)
			2'b00:next_state=2'b01;
			2'b01:next_state=2'b10;
			2'b10:next_state=2'b11;
			2'b11:next_state=2'b00;
		endcase
	else
		next_state=curr_state;
end
always@(posedge clk or posedge rst)
begin
	if(rst)
		curr_state<=2'b00;
	else
		curr_state<=next_state
end
assign led=(curr_state==2'b11)?2'b1:1'b0;
endmodule