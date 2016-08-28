`define s0 4'b0000
`define s1 4'b0001
`define s2 4'b0010
`define s3 4'b0011
`define s4 4'b0100
`define s5 4'b0101
`define s6 4'b0110
`define s7 4'b0111
`define s8 4'b1000
`define s9 4'b1001
`define s10 4'b1010
module Control(clk,rst,divide_en,adjust_en,adjust_en1,refresh_en,address_en);
  input clk,rst;
  output reg divide_en,adjust_en,refresh_en,address_en;
  output reg [2:0] adjust_en1;
  reg [3:0] state=`s0;
  always@(posedge clk)
  begin
  if(rst)
   begin
     state<=`s0;
     divide_en<=0;
     adjust_en<=0;
     refresh_en<=0;
     address_en<=0;
   end
  else
    begin
      case(state)
        `s0:begin divide_en<=1;state<=`s1;end
          
        `s1:begin adjust_en<=1;refresh_en<=0;adjust_en1<=3'b000;state<=`s2;end  //use x0
        `s2:begin adjust_en<=0;refresh_en<=1;state<=`s3;end
          
        `s3:begin adjust_en<=1;refresh_en<=0;adjust_en1<=3'b001;state<=`s4;end  //use x1
        `s4:begin adjust_en<=0;refresh_en<=1;state<=`s5;end
          
          
        `s5:begin adjust_en<=1;refresh_en<=0;adjust_en1<=3'b010;state<=`s6;end  //use x2
        `s6:begin adjust_en<=0;refresh_en<=1;state<=`s7;end
        
        `s7:begin adjust_en<=1;refresh_en<=0;adjust_en1<=3'b011;state<=`s8;end   //use x3
        `s8:begin adjust_en<=0;refresh_en<=1;state<=`s9;end
          
          
        `s9:begin address_en<=1;refresh_en<=0;state<=`s10;address_en<=1;end  //get y0...y4
        `s10:begin state<=`s0;end
        endcase
    end
end
endmodule

