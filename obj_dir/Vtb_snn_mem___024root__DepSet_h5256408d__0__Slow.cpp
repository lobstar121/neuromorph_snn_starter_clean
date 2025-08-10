// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_snn_mem.h for the primary calling header

#include "Vtb_snn_mem__pch.h"
#include "Vtb_snn_mem___024root.h"

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_static__TOP(Vtb_snn_mem___024root* vlSelf);

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_static(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_static\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_snn_mem___024root___eval_static__TOP(vlSelf);
    vlSelfRef.__Vtrigprevexpr___TOP__tb_snn_mem__DOT__clk__0 = 0U;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_snn_mem__DOT__rstn__0 = 0U;
}

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_static__TOP(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_static__TOP\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_snn_mem__DOT__clk = 0U;
    vlSelfRef.tb_snn_mem__DOT__rstn = 0U;
}

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_final(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_final\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__stl(Vtb_snn_mem___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_snn_mem___024root___eval_phase__stl(Vtb_snn_mem___024root* vlSelf);

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_settle(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_settle\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtb_snn_mem___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("tb_snn_mem.sv", 4, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtb_snn_mem___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__stl(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___dump_triggers__stl\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

void Vtb_snn_mem___024root___act_sequent__TOP__0(Vtb_snn_mem___024root* vlSelf);

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_stl(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_stl\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vtb_snn_mem___024root___act_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vtb_snn_mem___024root___eval_triggers__stl(Vtb_snn_mem___024root* vlSelf);

VL_ATTR_COLD bool Vtb_snn_mem___024root___eval_phase__stl(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___eval_phase__stl\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_snn_mem___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vtb_snn_mem___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__act(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___dump_triggers__act\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge tb_snn_mem.clk)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(negedge tb_snn_mem.rstn)\n");
    }
    if ((4ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_snn_mem___024root___dump_triggers__nba(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___dump_triggers__nba\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge tb_snn_mem.clk)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(negedge tb_snn_mem.rstn)\n");
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_snn_mem___024root___ctor_var_reset(Vtb_snn_mem___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_snn_mem___024root___ctor_var_reset\n"); );
    Vtb_snn_mem__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelf->tb_snn_mem__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb_snn_mem__DOT__rstn = VL_RAND_RESET_I(1);
    vlSelf->tb_snn_mem__DOT__event_vec_reg = VL_RAND_RESET_Q(48);
    VL_RAND_RESET_W(96, vlSelf->tb_snn_mem__DOT__spikes_vec);
    vlSelf->tb_snn_mem__DOT__T = 0;
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__events_mem[__Vi0] = VL_RAND_RESET_Q(48);
    }
    for (int __Vi0 = 0; __Vi0 < 4608; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__weights_mem[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__vth_mem[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->tb_snn_mem__DOT____Vlvbound_hd49f5adf__0 = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT____Vlvbound_hf699eec9__0 = VL_RAND_RESET_I(16);
    for (int __Vi0 = 0; __Vi0 < 4608; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__weights_rom[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__vth_rom[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__V_q14[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__refrac[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__active[__Vi0] = VL_RAND_RESET_I(1);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__leak32[__Vi0] = VL_RAND_RESET_I(32);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__acc32[__Vi0] = VL_RAND_RESET_I(32);
    }
    for (int __Vi0 = 0; __Vi0 < 96; ++__Vi0) {
        vlSelf->tb_snn_mem__DOT__dut__DOT__V_next[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w16_local = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT__dut__DOT__unnamedblk3__DOT__unnamedblk4__DOT__unnamedblk5__DOT__w32_local = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__sum32_local = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT__unnamedblk6__DOT__unnamedblk7__DOT__bias = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT__unnamedblk9__DOT__n = 0;
    vlSelf->tb_snn_mem__DOT__dut__DOT__unnamedblk8__DOT__n = 0;
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_hbe3549b5__0 = VL_RAND_RESET_I(1);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h71df13b1__0 = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h3fcb0b31__0 = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h4b083022__0 = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_he4874af4__0 = VL_RAND_RESET_I(1);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h4e2408c2__0 = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__0 = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h5a921e48__1 = VL_RAND_RESET_I(32);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_he5d2718e__0 = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__0 = VL_RAND_RESET_I(1);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__0 = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__0 = 0;
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h7b2f4ca3__1 = VL_RAND_RESET_I(1);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h790d1774__1 = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_hc40ee057__1 = 0;
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h4c05f569__0 = VL_RAND_RESET_I(16);
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_hef173a4e__0 = 0;
    vlSelf->tb_snn_mem__DOT__dut__DOT____Vlvbound_h4ba361bc__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb_snn_mem__DOT__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb_snn_mem__DOT__rstn__0 = VL_RAND_RESET_I(1);
}
