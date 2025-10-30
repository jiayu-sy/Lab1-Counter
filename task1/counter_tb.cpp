/* testbench file */
/* template for all other testbench files*/
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int j = 3;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vcounter* top = new Vcounter;

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i=0; i<300; i++) {
        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++) { // simulmated clk 0,1,0,1
            tfp->dump (2*i + clk); 
            top->clk = !top->clk; // actual clock 1, 0, 1, 0
            top->eval ();
        }
        top->rst = (i<2) | (i==20);
        if (top->count == 0x9) j--;
        top->en = (i>4) & (j!=2) & (j!=1);
        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}