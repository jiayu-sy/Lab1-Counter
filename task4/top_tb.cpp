/* testbench file */
/* template for all other testbench files*/
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vtop* top = new Vtop;

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("this is a bomb");

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
        
        //send count value to Vbuddy
        vbdHex(4, (int(top->bcd) >> 16) & 0xF);
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);

        vbdCycle(i+1);
        // end of vbuddy output section
        top->rst = (i<2);
        top->en = vbdFlag();
        top->v = vbdValue();
        if (Verilated::gotFinish()) exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}