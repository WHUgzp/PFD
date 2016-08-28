module Adjust(clk,ctrl,ctrl1,S0,S1,S2,S3,S4,x0,x1,x2,x3,x4,s0,s1,s2,s3,s4,sb0,sb1,sb2,sb3,sb4);
  input clk,ctrl;
  input [255:0]  s0,s1,s2,s3,s4;
  input [255:0] sb0,sb1,sb2,sb3,sb4;
  input [7:0] x0,x1,x2,x3,x4;
  input [2:0] ctrl1;
  output reg [255:0] S0,S1,S2,S3,S4;
  integer i,j,k,l;
  always@(posedge clk)
  begin
    if(ctrl)
      begin
          case(ctrl1)
            3'b000:begin for(i=0;i<256;i=i+1)
                          begin
                            S0[i]<=s0[i^x0];
                            S1[i]<=s1[i^x0];
                            S2[i]<=s2[i^x0];
                            S3[i]<=s3[i^x0];
                            S4[i]<=s4[i^x0];
                          end
                  end
           3'b001:begin for(j=0;j<256;j=j+1)
                          begin
                            S0[j]<=sb0[j^x1];
                            S1[j]<=sb1[j^x1];
                            S2[j]<=sb2[j^x1];
                            S3[j]<=sb3[j^x1];
                            S4[j]<=sb4[j^x1];
                          end
                  end
          3'b010:begin for(k=0;k<256;k=k+1)
                          begin
                            S0[k]<=sb0[k^x2];
                            S1[k]<=sb1[k^x2];
                            S2[k]<=sb2[k^x2];
                            S3[k]<=sb3[k^x2];
                            S4[k]<=sb4[k^x2];
                          end
                  end
          3'b011:begin for(l=0;l<256;l=l+1)
                          begin
                            S0[l]<=sb0[l^x3];
                            S1[l]<=sb1[l^x3];
                            S2[l]<=sb2[l^x3];
                            S3[l]<=sb3[l^x3];
                            S4[l]<=sb4[l^x3];
                          end
                 end
          endcase
  end
end
endmodule