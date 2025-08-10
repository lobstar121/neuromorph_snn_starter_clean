// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_snn_mem.h for the primary calling header

#include "Vtb_snn_mem__pch.h"
#include "Vtb_snn_mem___024root.h"

VlCoroutine Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__0(Vtb_snn_mem___024root* vlSelf);
VlCoroutine Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__1(Vtb_snn_mem___024root* vlSelf);

void Vtb_snn_mem___024root___eval_initial(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_initial\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__1(vlSelf);
}

VL_INLINE_OPT VlCoroutine Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__0(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ tb_snn_mem__DOT__ofile;
    tb_snn_mem__DOT__ofile = 0;
    IData/*31:0*/ tb_snn_mem__DOT__unnamedblk3__DOT__t;
    tb_snn_mem__DOT__unnamedblk3__DOT__t = 0;
    IData/*31:0*/ __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i;
    __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i = 0;
    IData/*31:0*/ __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i;
    __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i = 0;
    // Body
    if ((! VL_VALUEPLUSARGS_INN(64, std::string{"WHEX=%s"}, 
                                vlSelfRef.tb_snn_mem__DOT__whex))) {
        vlSelfRef.tb_snn_mem__DOT__whex = std::string{"artifacts/weights.hex"};
    }
    if ((! VL_VALUEPLUSARGS_INN(64, std::string{"VTH=%s"}, 
                                vlSelfRef.tb_snn_mem__DOT__vthx))) {
        vlSelfRef.tb_snn_mem__DOT__vthx = std::string{"artifacts/vth.hex"};
    }
    if ((! VL_VALUEPLUSARGS_INN(64, std::string{"EVHEX=%s"}, 
                                vlSelfRef.tb_snn_mem__DOT__evhex))) {
        vlSelfRef.tb_snn_mem__DOT__evhex = std::string{"artifacts/events_ref.mem"};
    }
    if ((! VL_VALUEPLUSARGS_INN(64, std::string{"OUT=%s"}, 
                                vlSelfRef.tb_snn_mem__DOT__outcsv))) {
        vlSelfRef.tb_snn_mem__DOT__outcsv = std::string{"artifacts/spikes_hw.csv"};
    }
    if ((! VL_VALUEPLUSARGS_INI(32, std::string{"T=%d"}, 
                                vlSelfRef.tb_snn_mem__DOT__T))) {
        vlSelfRef.tb_snn_mem__DOT__T = 0x4cU;
    }
    VL_WRITEF_NX("[TB] WHEX=%@  VTH=%@  EVHEX=%@  OUT=%@  T=%0d\n",0,
                 -1,&(vlSelfRef.tb_snn_mem__DOT__whex),
                 -1,&(vlSelfRef.tb_snn_mem__DOT__vthx),
                 -1,&(vlSelfRef.tb_snn_mem__DOT__evhex),
                 -1,&(vlSelfRef.tb_snn_mem__DOT__outcsv),
                 32,vlSelfRef.tb_snn_mem__DOT__T);
    VL_WRITEF_NX("[TB] loading %@\n",0,-1,&(vlSelfRef.tb_snn_mem__DOT__whex));
    VL_READMEM_N(true, 16, 4608, 0, VL_CVT_PACK_STR_NN(vlSelfRef.tb_snn_mem__DOT__whex)
                 ,  &(vlSelfRef.tb_snn_mem__DOT__weights_mem)
                 , 0, ~0ULL);
    VL_WRITEF_NX("[TB] loading %@\n",0,-1,&(vlSelfRef.tb_snn_mem__DOT__vthx));
    VL_READMEM_N(true, 16, 96, 0, VL_CVT_PACK_STR_NN(vlSelfRef.tb_snn_mem__DOT__vthx)
                 ,  &(vlSelfRef.tb_snn_mem__DOT__vth_mem)
                 , 0, ~0ULL);
    VL_WRITEF_NX("[TB] loading %@\n",0,-1,&(vlSelfRef.tb_snn_mem__DOT__evhex));
    VL_READMEM_N(true, 48, 65536, 0, VL_CVT_PACK_STR_NN(vlSelfRef.tb_snn_mem__DOT__evhex)
                 ,  &(vlSelfRef.tb_snn_mem__DOT__events_mem)
                 , 0, ~0ULL);
    __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 0x1200U, __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i)) {
        if ((0x11ffU >= (0x1fffU & __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i))) {
            vlSelfRef.tb_snn_mem__DOT____Vlvbound_hd49f5adf__0 
                = vlSelfRef.tb_snn_mem__DOT__weights_mem
                [(0x1fffU & __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i)];
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom[(0x1fffU 
                                                              & __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i)] 
                = vlSelfRef.tb_snn_mem__DOT____Vlvbound_hd49f5adf__0;
        } else {
            vlSelfRef.tb_snn_mem__DOT____Vlvbound_hd49f5adf__0 = 0U;
        }
        __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i 
            = ((IData)(1U) + __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk1__DOT__i);
    }
    __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i = 0U;
    while (VL_GTS_III(32, 0x60U, __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i)) {
        if ((0x5fU >= (0x7fU & __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i))) {
            vlSelfRef.tb_snn_mem__DOT____Vlvbound_hf699eec9__0 
                = vlSelfRef.tb_snn_mem__DOT__vth_mem
                [(0x7fU & __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i)];
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__vth_rom[(0x7fU 
                                                          & __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i)] 
                = vlSelfRef.tb_snn_mem__DOT____Vlvbound_hf699eec9__0;
        } else {
            vlSelfRef.tb_snn_mem__DOT____Vlvbound_hf699eec9__0 = 0U;
        }
        __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i 
            = ((IData)(1U) + __Vtask_tb_snn_mem__DOT__load_mems__1__unnamedblk2__DOT__i);
    }
    tb_snn_mem__DOT__ofile = VL_FOPEN_NN(VL_CVT_PACK_STR_NN(vlSelfRef.tb_snn_mem__DOT__outcsv)
                                         , std::string{"w"});
    ;
    if (VL_UNLIKELY(((0U == tb_snn_mem__DOT__ofile)))) {
        VL_WRITEF_NX("[TB][ERROR] cannot open %@ for write\n",0,
                     -1,&(vlSelfRef.tb_snn_mem__DOT__outcsv));
        VL_FINISH_MT("tb_snn_mem.sv", 62, "");
    }
    co_await vlSelfRef.__VtrigSched_ha77c2d59__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_snn_mem.clk)", 
                                                         "tb_snn_mem.sv", 
                                                         66);
    co_await vlSelfRef.__VtrigSched_ha77c2d59__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_snn_mem.clk)", 
                                                         "tb_snn_mem.sv", 
                                                         66);
    co_await vlSelfRef.__VtrigSched_ha77c2d59__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_snn_mem.clk)", 
                                                         "tb_snn_mem.sv", 
                                                         66);
    co_await vlSelfRef.__VtrigSched_ha77c2d59__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_snn_mem.clk)", 
                                                         "tb_snn_mem.sv", 
                                                         66);
    vlSelfRef.tb_snn_mem__DOT__rstn = 1U;
    co_await vlSelfRef.__VtrigSched_ha77c2d59__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge tb_snn_mem.clk)", 
                                                         "tb_snn_mem.sv", 
                                                         68);
    tb_snn_mem__DOT__unnamedblk3__DOT__t = 0U;
    while (VL_LTS_III(32, tb_snn_mem__DOT__unnamedblk3__DOT__t, vlSelfRef.tb_snn_mem__DOT__T)) {
        vlSelfRef.tb_snn_mem__DOT__event_vec = vlSelfRef.tb_snn_mem__DOT__events_mem
            [(0xffffU & tb_snn_mem__DOT__unnamedblk3__DOT__t)];
        co_await vlSelfRef.__VtrigSched_ha77c2d59__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_snn_mem.clk)", 
                                                             "tb_snn_mem.sv", 
                                                             73);
        vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n = 0U;
        while (VL_GTS_III(32, 0x60U, vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n)) {
            VL_FWRITEF_NX(tb_snn_mem__DOT__ofile,"%0d",0,
                          32,(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n)) 
                               && (1U & (vlSelfRef.tb_snn_mem__DOT__spikes_vec[
                                         (3U & (vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n 
                                                >> 5U))] 
                                         >> (0x1fU 
                                             & vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n))))
                               ? 1U : 0U));
            if (VL_UNLIKELY(((0x5fU != vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n)))) {
                VL_FWRITEF_NX(tb_snn_mem__DOT__ofile,",",0);
            }
            vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n 
                = ((IData)(1U) + vlSelfRef.tb_snn_mem__DOT__unnamedblk3__DOT__unnamedblk4__DOT__n);
        }
        VL_FWRITEF_NX(tb_snn_mem__DOT__ofile,"\n",0);
        tb_snn_mem__DOT__unnamedblk3__DOT__t = ((IData)(1U) 
                                                + tb_snn_mem__DOT__unnamedblk3__DOT__t);
    }
    VL_FCLOSE_I(tb_snn_mem__DOT__ofile); VL_WRITEF_NX("[TB] wrote %0d lines to %@\n",0,
                                                      32,
                                                      vlSelfRef.tb_snn_mem__DOT__T,
                                                      -1,
                                                      &(vlSelfRef.tb_snn_mem__DOT__outcsv));
    VL_FINISH_MT("tb_snn_mem.sv", 84, "");
}

VL_INLINE_OPT VlCoroutine Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__1(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (1U) {
        co_await vlSelfRef.__VdlySched.delay(0x3e8ULL, 
                                             nullptr, 
                                             "tb_snn_mem.sv", 
                                             22);
        vlSelfRef.tb_snn_mem__DOT__clk = (1U & (~ (IData)(vlSelfRef.tb_snn_mem__DOT__clk)));
    }
}

void Vtb_snn_mem___024root___act_sequent__TOP__0(Vtb_snn_mem___024root* vlSelf);

void Vtb_snn_mem___024root___eval_act(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_act\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        Vtb_snn_mem___024root___act_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vtb_snn_mem___024root___act_sequent__TOP__0(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___act_sequent__TOP__0\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i;
    tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i = 0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0;
    tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0 = 0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1;
    tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1 = 0;
    IData/*31:0*/ tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2;
    tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2 = 0;
    // Body
    tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 0x60U, tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i)) {
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hbe3549b5__0 = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h71df13b1__0 = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h3fcb0b31__0 = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4b083022__0 = 0U;
        if ((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__active[(0x7fU 
                                                         & tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hbe3549b5__0;
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__leak32[(0x7fU 
                                                         & tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h71df13b1__0;
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h3fcb0b31__0;
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__V_next[(0x7fU 
                                                         & tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4b083022__0;
        }
        tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i 
            = ((IData)(1U) + tb_snn_mem__DOT__dut__DOT__unnamedblk1__DOT__i);
    }
    tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0 = 0U;
    while (VL_GTS_III(32, 0x60U, tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0)) {
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_he4874af4__0 
            = VL_GTES_III(32, 0U, ((0x5fU >= (0x7fU 
                                              & tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0))
                                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__refrac
                                   [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0)]
                                    : 0U));
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__active[(0x7fU 
                                                         & tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_he4874af4__0;
        }
        tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0 
            = ((IData)(1U) + tb_snn_mem__DOT__dut__DOT__unnamedblk2__DOT__n0);
    }
    tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1 = 0U;
    while (VL_GTS_III(32, 0x60U, tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)) {
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4e2408c2__0 
            = VL_MULS_III(32, (IData)(0x3c00U), VL_EXTENDS_II(32,16, 
                                                              ((0x5fU 
                                                                >= 
                                                                (0x7fU 
                                                                 & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                                                                ? 
                                                               vlSelfRef.tb_snn_mem__DOT__dut__DOT__V_q14
                                                               [
                                                               (0x7fU 
                                                                & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                                                                : 0U)));
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__0 = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)(vlSelfRef.tb_snn_mem__DOT__event_vec))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if ((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__leak32[(0x7fU 
                                                         & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4e2408c2__0;
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__0;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 1U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x60U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x60U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 2U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xc0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xc0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 3U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x120U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x120U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 4U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x180U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x180U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 5U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x1e0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x1e0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 6U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x240U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x240U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 7U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x2a0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x2a0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 8U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x300U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x300U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 9U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x360U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x360U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0xaU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x3c0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x3c0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0xbU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x420U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x420U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0xcU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x480U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x480U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0xdU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x4e0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x4e0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0xeU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x540U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x540U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0xfU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x5a0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x5a0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x10U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x600U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x600U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x11U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x660U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x660U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x12U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x6c0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x6c0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x13U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x720U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x720U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x14U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x780U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x780U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x15U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x7e0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x7e0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x16U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x840U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x840U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x17U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x8a0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x8a0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x18U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x900U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x900U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x19U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x960U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x960U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x1aU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x9c0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x9c0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x1bU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xa20U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xa20U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x1cU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xa80U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xa80U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x1dU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xae0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xae0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x1eU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xb40U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xb40U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x1fU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xba0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xba0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x20U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xc00U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xc00U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x21U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xc60U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xc60U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x22U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xcc0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xcc0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x23U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xd20U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xd20U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x24U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xd80U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xd80U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x25U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xde0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xde0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x26U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xe40U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xe40U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x27U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xea0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xea0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x28U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xf00U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xf00U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x29U)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xf60U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xf60U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x2aU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0xfc0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0xfc0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x2bU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x1020U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x1020U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x2cU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x1080U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x1080U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x2dU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x10e0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x10e0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x2eU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x1140U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x1140U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = 0U;
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = 0U;
        if ((1U & (IData)((vlSelfRef.tb_snn_mem__DOT__event_vec 
                           >> 0x2fU)))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local 
                = ((0x11ffU >= (0x1fffU & ((IData)(0x11a0U) 
                                           + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)))
                    ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__weights_rom
                   [(0x1fffU & ((IData)(0x11a0U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))]
                    : 0U);
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local 
                = (((- (IData)((1U & ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local) 
                                      >> 0xfU)))) << 0x10U) 
                   | (IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local));
        }
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 
            = (((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))
                 ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)]
                 : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local);
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32[(0x7fU 
                                                        & tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1;
        }
        tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1 
            = ((IData)(1U) + tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__n1);
    }
    tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2 = 0U;
    while (VL_GTS_III(32, 0x60U, tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2)) {
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__bias 
            = (VL_LTES_III(32, 0U, ((0x5fU >= (0x7fU 
                                               & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2))
                                     ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__leak32
                                    [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2)]
                                     : 0U)) ? 0x2000U
                : 0xffffe000U);
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__sum32_local 
            = (VL_SHIFTRS_III(32,32,32, (((0x5fU >= 
                                           (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2))
                                           ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__leak32
                                          [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2)]
                                           : 0U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__bias), 0xeU) 
               + ((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2))
                   ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__acc32
                  [(0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2)]
                   : 0U));
        vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_he5d2718e__0 
            = (VL_LTS_III(32, 0x7fffU, vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__sum32_local)
                ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__sum32_local)
                              ? 0x8000U : (0xffffU 
                                           & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__sum32_local)));
        if (VL_LIKELY(((0x5fU >= (0x7fU & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2))))) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__V_next[(0x7fU 
                                                         & tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2)] 
                = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_he5d2718e__0;
        }
        tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2 
            = ((IData)(1U) + tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__n2);
    }
}

void Vtb_snn_mem___024root___nba_sequent__TOP__0(Vtb_snn_mem___024root* vlSelf);

void Vtb_snn_mem___024root___eval_nba(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_nba\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_snn_mem___024root___nba_sequent__TOP__0(vlSelf);
        Vtb_snn_mem___024root___act_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vtb_snn_mem___024root___nba_sequent__TOP__0(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___nba_sequent__TOP__0\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*15:0*/ __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v0;
    __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v0 = 0;
    CData/*6:0*/ __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v0;
    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v0 = 0;
    IData/*31:0*/ __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v0;
    __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v0 = 0;
    CData/*6:0*/ __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v0;
    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v0 = 0;
    SData/*15:0*/ __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v1;
    __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v1 = 0;
    CData/*6:0*/ __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v1;
    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v1 = 0;
    IData/*31:0*/ __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v1;
    __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v1 = 0;
    CData/*6:0*/ __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v1;
    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v1 = 0;
    SData/*15:0*/ __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v2;
    __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v2 = 0;
    CData/*6:0*/ __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v2;
    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v2 = 0;
    IData/*31:0*/ __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v2;
    __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v2 = 0;
    CData/*6:0*/ __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v2;
    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v2 = 0;
    // Body
    if (vlSelfRef.tb_snn_mem__DOT__rstn) {
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n = 0U;
        while (VL_GTS_III(32, 0x60U, vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)) {
            if ((((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)) 
                  && vlSelfRef.tb_snn_mem__DOT__dut__DOT__active
                  [(0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)]) 
                 & VL_GTES_III(16, ((0x5fU >= (0x7fU 
                                               & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))
                                     ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__V_next
                                    [(0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)]
                                     : 0U), ((0x5fU 
                                              >= (0x7fU 
                                                  & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))
                                              ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__vth_rom
                                             [(0x7fU 
                                               & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)]
                                              : 0U)))) {
                vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__0 = 1U;
                vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__0 = 0U;
                if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))))) {
                    vlSelfRef.tb_snn_mem__DOT__spikes_vec[(3U 
                                                           & (vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n 
                                                              >> 5U))] 
                        = (((~ ((IData)(1U) << (0x1fU 
                                                & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))) 
                            & vlSelfRef.tb_snn_mem__DOT__spikes_vec[
                            (3U & (vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n 
                                   >> 5U))]) | ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__0) 
                                                << 
                                                (0x1fU 
                                                 & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)));
                }
                if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))))) {
                    __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v0 
                        = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__0;
                    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v0 
                        = (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n);
                    vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__V_q14.enqueue(__VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v0, (IData)(__VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v0));
                }
                vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__0 = 2U;
                if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))))) {
                    __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v0 
                        = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__0;
                    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v0 
                        = (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n);
                    vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__refrac.enqueue(__VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v0, (IData)(__VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v0));
                }
            } else {
                vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__1 = 0U;
                if ((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))) {
                    vlSelfRef.tb_snn_mem__DOT__spikes_vec[(3U 
                                                           & (vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n 
                                                              >> 5U))] 
                        = (((~ ((IData)(1U) << (0x1fU 
                                                & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))) 
                            & vlSelfRef.tb_snn_mem__DOT__spikes_vec[
                            (3U & (vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n 
                                   >> 5U))]) | ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__1) 
                                                << 
                                                (0x1fU 
                                                 & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)));
                    vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__1 
                        = vlSelfRef.tb_snn_mem__DOT__dut__DOT__V_next
                        [(0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)];
                } else {
                    vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__1 = 0U;
                }
                if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))))) {
                    __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v1 
                        = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__1;
                    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v1 
                        = (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n);
                    vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__V_q14.enqueue(__VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v1, (IData)(__VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v1));
                }
                vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__1 
                    = (VL_LTS_III(32, 0U, ((0x5fU >= 
                                            (0x7fU 
                                             & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))
                                            ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__refrac
                                           [(0x7fU 
                                             & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)]
                                            : 0U)) ? 
                       (((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))
                          ? vlSelfRef.tb_snn_mem__DOT__dut__DOT__refrac
                         [(0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n)]
                          : 0U) - (IData)(1U)) : 0U);
                if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n))))) {
                    __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v1 
                        = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__1;
                    __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v1 
                        = (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n);
                    vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__refrac.enqueue(__VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v1, (IData)(__VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v1));
                }
            }
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n 
                = ((IData)(1U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n);
        }
    } else {
        vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n = 0U;
        while (VL_GTS_III(32, 0x60U, vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n)) {
            vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4c05f569__0 = 0U;
            if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n))))) {
                __VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v2 
                    = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4c05f569__0;
                __VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v2 
                    = (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n);
                vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__V_q14.enqueue(__VdlyVal__tb_snn_mem__DOT__dut__DOT__V_q14__v2, (IData)(__VdlyDim0__tb_snn_mem__DOT__dut__DOT__V_q14__v2));
            }
            vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hef173a4e__0 = 0U;
            if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n))))) {
                __VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v2 
                    = vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_hef173a4e__0;
                __VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v2 
                    = (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n);
                vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__refrac.enqueue(__VdlyVal__tb_snn_mem__DOT__dut__DOT__refrac__v2, (IData)(__VdlyDim0__tb_snn_mem__DOT__dut__DOT__refrac__v2));
            }
            vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4ba361bc__0 = 0U;
            if (VL_LIKELY(((0x5fU >= (0x7fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n))))) {
                vlSelfRef.tb_snn_mem__DOT__spikes_vec[(3U 
                                                       & (vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n 
                                                          >> 5U))] 
                    = (((~ ((IData)(1U) << (0x1fU & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n))) 
                        & vlSelfRef.tb_snn_mem__DOT__spikes_vec[
                        (3U & (vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n 
                               >> 5U))]) | ((IData)(vlSelfRef.tb_snn_mem__DOT__dut__DOT____Vlvbound_h4ba361bc__0) 
                                            << (0x1fU 
                                                & vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n)));
            }
            vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n 
                = ((IData)(1U) + vlSelfRef.tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n);
        }
    }
    vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__V_q14.commit(vlSelfRef.tb_snn_mem__DOT__dut__DOT__V_q14);
    vlSelfRef.__VdlyCommitQueuetb_snn_mem__DOT__dut__DOT__refrac.commit(vlSelfRef.tb_snn_mem__DOT__dut__DOT__refrac);
}

void Vtb_snn_mem___024root___timing_resume(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___timing_resume\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        vlSelfRef.__VtrigSched_ha77c2d59__0.resume(
                                                   "@(posedge tb_snn_mem.clk)");
    }
    if ((4ULL & vlSelfRef.__VactTriggered.word(0U))) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_snn_mem___024root___timing_commit(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___timing_commit\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((! (1ULL & vlSelfRef.__VactTriggered.word(0U)))) {
        vlSelfRef.__VtrigSched_ha77c2d59__0.commit(
                                                   "@(posedge tb_snn_mem.clk)");
    }
}

void Vtb_snn_mem___024root___eval_triggers__act(Vtb_snn_mem___024root* vlSelf);

bool Vtb_snn_mem___024root___eval_phase__act(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_phase__act\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<3> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_snn_mem___024root___eval_triggers__act(vlSelf);
    Vtb_snn_mem___024root___timing_commit(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vtb_snn_mem___024root___timing_resume(vlSelf);
        Vtb_snn_mem___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtb_snn_mem___024root___eval_phase__nba(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_phase__nba\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtb_snn_mem___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__nba(Vtb_snn_mem___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__act(Vtb_snn_mem___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb_snn_mem___024root___eval(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_snn_mem___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("tb_snn_mem.sv", 4, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_snn_mem___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("tb_snn_mem.sv", 4, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vtb_snn_mem___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vtb_snn_mem___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtb_snn_mem___024root___eval_debug_assertions(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_debug_assertions\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
