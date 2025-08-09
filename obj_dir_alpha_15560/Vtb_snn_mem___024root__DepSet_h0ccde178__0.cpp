// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_snn_mem.h for the primary calling header

#include "Vtb_snn_mem__pch.h"
#include "Vtb_snn_mem__Syms.h"
#include "Vtb_snn_mem___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__act(Vtb_snn_mem___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb_snn_mem___024root___eval_triggers__act(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_triggers__act\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.setBit(0U, ((IData)(vlSelfRef.tb_snn_mem__DOT__clk) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_snn_mem__DOT__clk__0))));
    vlSelfRef.__VactTriggered.setBit(1U, ((~ (IData)(vlSelfRef.tb_snn_mem__DOT__rstn)) 
                                          & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_snn_mem__DOT__rstn__0)));
    vlSelfRef.__VactTriggered.setBit(2U, vlSelfRef.__VdlySched.awaitingCurrentTime());
    vlSelfRef.__Vtrigprevexpr___TOP__tb_snn_mem__DOT__clk__0 
        = vlSelfRef.tb_snn_mem__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_snn_mem__DOT__rstn__0 
        = vlSelfRef.tb_snn_mem__DOT__rstn;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_snn_mem___024root___dump_triggers__act(vlSelf);
    }
#endif
}
