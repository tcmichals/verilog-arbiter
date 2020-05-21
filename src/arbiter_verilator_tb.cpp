#include <stdlib.h>
#include <memory>
#include <iostream>
#include "Varbiter_verilator_tb.h"
#include "verilated.h"
#include "verilated_vcd_c.h"


#define TIME_INC 10
#define CYCLE_TIME (400)
#define RESET_TIME (100)



bool done = false;
void INThandler(int signal)
{
  printf("\nCaught ctrl-c\n");
  done = true;
}

enum class which_cyc
{
    None,
    CYC_1,
    CYC_2_PREP,
    CYC_2,
    DONE,

};

int main(int argc, char **argv) 
{
	// Initialize Verilators variables
	Verilated::commandArgs(argc, argv);

	// Create an instance of our module under test
	std::unique_ptr<Varbiter_verilator_tb> tb = std::make_unique< Varbiter_verilator_tb>();
    std::unique_ptr<VerilatedVcdC> tfp=  std::make_unique<VerilatedVcdC>();
    Verilated::traceEverOn(true);

    uint32_t main_time = 0; 
    uint32_t time_invoke = 0;

    which_cyc cyc(which_cyc::None);

    tb->trace (tfp.get(), 99);

    tfp->open("arbiter_verilator_tb.vcd");
    tb->wb_rst = 1;
    tb->wb_clk = 0;
    tb->wbm_cyc_i = 0;
	// Tick the clock until we are done
	while(!Verilated::gotFinish() && !done) 
    {
		tb->eval();
       		 if (tfp)
		    tfp->dump(main_time);


        switch( cyc)
        {
            case which_cyc::CYC_1:
                if ( tb->wbs_cyc_o == 0)
                {
                    std::cout << "grant worked CYC 0" << std::endl;
                     cyc = which_cyc::CYC_2_PREP;
                     tb->wbm_cyc_i = 0;
                     time_invoke = main_time + CYCLE_TIME;
                }
                else if ( time_invoke < main_time )
                {
                    std::cerr << "Grant failed" << std::endl;
                    tb->wbm_cyc_i = 0;
                    done = true;
                }
            break;
            case which_cyc::CYC_2_PREP:

                if ( time_invoke < main_time )
                {
                    tb->wbm_cyc_i = 2;
                    time_invoke = main_time + CYCLE_TIME ;
                    cyc = which_cyc::CYC_2;
                    std::cout << "Second test" << std::endl;
                }
            break;

            case which_cyc::CYC_2:
                if ( tb->wbs_cyc_o == 1)
                {
                    std::cout << "grant worked CYC 1" << std::endl;
                    cyc = which_cyc::DONE;
		     time_invoke = main_time + CYCLE_TIME;
                }
                else if ( time_invoke < main_time )
                {
                    std::cerr << "Grant failed" << std::endl;
                    tb->wbm_cyc_i = 0;
                    done = true;
                }

            break;

            case which_cyc::None:
		 if(!tb->wb_rst)
                {
                    tb->wbm_cyc_i = 0;
                    time_invoke = main_time + CYCLE_TIME;
                    cyc = which_cyc::CYC_1;
                }
            break;

            case which_cyc::DONE:
                 if ( time_invoke < main_time )
                {
                    done = true;
                }

            break;

        }

        tb->wb_rst = main_time < RESET_TIME;
	tb->wb_clk = !tb->wb_clk;
        main_time += TIME_INC;
		
	} 
    tfp->close();

    return EXIT_SUCCESS;
}
