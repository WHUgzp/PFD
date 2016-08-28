module Refresh(clk,ctrl,s0,s1,s2,s3,s4,S0,S1,S2,S3,S4);
  input clk,ctrl;
  input [255:0] s0,s1,s2,s3,s4;
  output reg [255:0] S0,S1,S2,S3,S4;
  wire [255:0] tmp [4:1];
  assign tmp[1]={$random,$random,$random,$random,$random,$random,$random,$random};
  assign tmp[2]={$random,$random,$random,$random,$random,$random,$random,$random};
  assign tmp[3]={$random,$random,$random,$random,$random,$random,$random,$random};
  assign tmp[4]={$random,$random,$random,$random,$random,$random,$random,$random};
  always@(posedge clk)
  begin
    if(ctrl)
      begin
        S0<=s0^tmp[1]^tmp[2]^tmp[3]^tmp[4];
        S1<=s1^tmp[1];
        S2<=s2^tmp[2];
        S3<=s3^tmp[3];
        S4<=s4^tmp[4];
      end
  end
endmodule
