// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_snn_mem.h for the primary calling header

#ifndef VERILATED_VTB_SNN_MEM___024ROOT_H_
#define VERILATED_VTB_SNN_MEM___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_snn_mem__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_snn_mem___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ tb_snn_mem__DOT__clk;
    CData/*0:0*/ tb_snn_mem__DOT__rstn;
    CData/*0:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_hbe3549b5__0;
    CData/*0:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_he4874af4__0;
    CData/*0:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__0;
    CData/*0:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__1;
    CData/*0:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h4ba361bc__0;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_snn_mem__DOT__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_snn_mem__DOT__rstn__0;
    CData/*0:0*/ __VactContinue;
    SData/*15:0*/ tb_snn_mem__DOT____Vlvbound_hd49f5adf__0;
    SData/*15:0*/ tb_snn_mem__DOT____Vlvbound_hf699eec9__0;
    SData/*15:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local;
    SData/*15:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h4b083022__0;
    SData/*15:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_he5d2718e__0;
    SData/*15:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__0;
    SData/*15:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__1;
    SData/*15:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h4c05f569__0;
    VlWide<3>/*95:0*/ tb_snn_mem__DOT__spikes_vec;
    IData/*31:0*/ tb_snn_mem__DOT__T;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__sum32_local;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__bias;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h71df13b1__0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h3fcb0b31__0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h4e2408c2__0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__1;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT____Vlvbound_hef173a4e__0;
    IData/*31:0*/ __VactIterCount;
    QData/*47:0*/ tb_snn_mem__DOT__event_vec_reg;
    VlUnpacked<QData/*47:0*/, 65536> tb_snn_mem__DOT__events_mem;
    VlUnpacked<SData/*15:0*/, 4608> tb_snn_mem__DOT__weights_mem;
    VlUnpacked<SData/*15:0*/, 96> tb_snn_mem__DOT__vth_mem;
    VlUnpacked<SData/*15:0*/, 4608> tb_snn_mem__DOT__dut__DOT__weights_rom;
    VlUnpacked<SData/*15:0*/, 96> tb_snn_mem__DOT__dut__DOT__vth_rom;
    VlUnpacked<SData/*15:0*/, 96> tb_snn_mem__DOT__dut__DOT__V_q14;
    VlUnpacked<IData/*31:0*/, 96> tb_snn_mem__DOT__dut__DOT__refrac;
    VlUnpacked<CData/*0:0*/, 96> tb_snn_mem__DOT__dut__DOT__active;
    VlUnpacked<IData/*31:0*/, 96> tb_snn_mem__DOT__dut__DOT__leak32;
    VlUnpacked<IData/*31:0*/, 96> tb_snn_mem__DOT__dut__DOT__acc32;
    VlUnpacked<SData/*15:0*/, 96> tb_snn_mem__DOT__dut__DOT__V_next;
    std::string tb_snn_mem__DOT__whex;
    std::string tb_snn_mem__DOT__vthx;
    std::string tb_snn_mem__DOT__evhex;
    std::string tb_snn_mem__DOT__outcsv;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_ha77c2d59__0;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<3> __VactTriggered;
    VlTriggerVec<3> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtb_snn_mem__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtb_snn_mem___024root(Vtb_snn_mem__Syms* symsp, const char* v__name);
    ~Vtb_snn_mem___024root();
    VL_UNCOPYABLE(Vtb_snn_mem___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
