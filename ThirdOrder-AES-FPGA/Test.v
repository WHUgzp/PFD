module Test;
  reg clk,rst;
  reg [7:0] x0,x1,x2,x3,x4;
  initial
  begin
    clk<=0;
    rst<=1;
    x0<=8'h35;
    x1<=8'h45;
    x2<=8'h89;
    x3<=8'h31;
    x4<=8'h85;
  end
  
  Top_8bit Top_8bit(.rst(rst),.clk(clk),.x0(x0),.x1(x1),.x2(x2),.x3(x3),.x4(x4),.y0(),.y1(),.y2(),.y3(),.y4());  
  always
  begin
  #10 rst<=0;
  forever #100 clk=~clk;
end
endmodule
  
