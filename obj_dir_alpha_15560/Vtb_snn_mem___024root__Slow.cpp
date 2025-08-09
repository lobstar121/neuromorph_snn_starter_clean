// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_snn_mem.h for the primary calling header

#include "Vtb_snn_mem__pch.h"
#include "Vtb_snn_mem__Syms.h"
#include "Vtb_snn_mem___024root.h"

void Vtb_snn_mem___024root___ctor_var_reset(Vtb_snn_mem___024root* vlSelf);

Vtb_snn_mem___024root::Vtb_snn_mem___024root(Vtb_snn_mem__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , __VdlySched{*symsp->_vm_contextp__}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vtb_snn_mem___024root___ctor_var_reset(this);
}

void Vtb_snn_mem___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vtb_snn_mem___024root::~Vtb_snn_mem___024root() {
}
