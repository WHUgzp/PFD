module Look_up(clk,ctrl,x,s0,s1,s2,s3,s4,y0,y1,y2,y3,y4);
  input clk,ctrl;
  input [7:0] x;
  input [255:0] s0,s1,s2,s3,s4;
  output reg y0,y1,y2,y3,y4;
  always@(posedge clk)
  begin
    if(ctrl)
      begin
        y0<=s0[x];
        y1<=s1[x];
        y2<=s2[x];
        y3<=s3[x];
        y4<=s4[x];
      end
  end
endmodule 


