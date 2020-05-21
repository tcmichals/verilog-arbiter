

`default_nettype none


module arbiter_verilator_tb(input wire wb_clk,
                            input wire wb_rst,
                            input [1:0]     wbm_cyc_i,
                            output          wbs_cyc_o
                            );

localparam num_masters = 2;
localparam master_sel_bits = num_masters > 1 ? $clog2(num_masters) : 1;


    /* verilator lint_off UNUSED */
    wire [num_masters-1:0]     grant;
    wire [master_sel_bits-1:0] master_sel;
    wire active;

    reg [1:0]     wbm_cyc_i_debug;
    wire not_equal = wbm_cyc_i_debug != wbm_cyc_i;
    initial begin
      wbm_cyc_i_debug = 2'b0;
    end
    

    arbiter
     #(.NUM_PORTS (num_masters))
   UUT  (.clk(wb_clk),
      .rst(wb_rst),
      .request(wbm_cyc_i),
      .grant(grant),
      .select(master_sel),
      .active(active));
  
  always @(posedge wb_clk) begin
    if (wb_rst)
        wbm_cyc_i_debug <= 2'b0;
      else begin
        if ( not_equal) 
          $display("wbm_cyc_i %02d", wbm_cyc_i);
          wbm_cyc_i_debug <= wbm_cyc_i;
      end

  end
assign wbs_cyc_o = wbm_cyc_i[master_sel] & active;


endmodule
//eof
