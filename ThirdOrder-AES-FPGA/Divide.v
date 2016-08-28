module Divide(clk,ctrl,S,S0,S1,S2,S3,S4);
  input clk,ctrl;
  input [255:0] S;
  output reg [255:0] S0;
  output reg [255:0] S1;
  output reg [255:0] S2;
  output reg [255:0] S3;
  output reg [255:0] S4;
  always@(posedge clk)
  begin
    if(ctrl)
      begin
        S0<=S;
        S1<=256'h0;
        S2<=256'h0;
        S3<=256'h0;
        S4<=256'h0;
      end
  end
endmodule
