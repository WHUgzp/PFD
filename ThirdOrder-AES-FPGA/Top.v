module Top(rst,clk,S,x0,x1,x2,x3,x4,y0,y1,y2,y3,y4);
  input rst,clk;
  input [7:0] x0,x1,x2,x3,x4;
  input [255:0] S;
  output y0,y1,y2,y3,y4;
  wire divide_en,address_en;
  wire adjust_en;
  wire refresh_en;
  wire [2:0] adjust_en1;
  wire [255:0] d_s0,d_s1,d_s2,d_s3,d_s4;
  wire [255:0] a_s0 ;
  wire [255:0] a_s1 ;
  wire [255:0] a_s2 ;
  wire [255:0] a_s3 ;
  wire [255:0] a_s4 ;
  wire [255:0] r_s0 ;
  wire [255:0] r_s1 ;
  wire [255:0] r_s2 ;
  wire [255:0] r_s3 ;
  wire [255:0] r_s4 ;
  Control Control(.clk(clk),.rst(rst),.divide_en(divide_en),.adjust_en(adjust_en),.adjust_en1(adjust_en1),.refresh_en(refresh_en),.address_en(address_en));
  
  
  Divide Divide(.clk(clk),.ctrl(divide_en),.S(S),.S0(d_s0),.S1(d_s1),.S2(d_s2),.S3(d_s3),.S4(d_s4));
  
  
  Adjust Adjust(.clk(clk),.ctrl(adjust_en),.ctrl1(adjust_en1),.S0(a_s0),.S1(a_s1),.S2(a_s2),.S3(a_s3),.S4(a_s4),.x0(x0),.x1(x1),.x2(x2),.x3(x3),.x4(x4),.s0(d_s0),.s1(d_s1),.s2(d_s2),.s3(d_s3),.s4(d_s4),.sb0(r_s0),.sb1(r_s1),.sb2(r_s2),.sb3(r_s3),.sb4(r_s4));
  Refresh Refresh(.clk(clk),.ctrl(refresh_en),.s0(a_s0),.s1(a_s1),.s2(a_s2),.s3(a_s3),.s4(a_s4),.S0(r_s0),.S1(r_s1),.S2(r_s2),.S3(r_s3),.S4(r_s4));
  
 
  Look_up Look_up(.clk(clk),.ctrl(address_en),.x(x4),.s0(r_s0),.s1(r_s1),.s2(r_s2),.s3(r_s3),.s4(r_s4),.y0(y0),.y1(y1),.y2(y2),.y3(y3),.y4(y4));
   
endmodule