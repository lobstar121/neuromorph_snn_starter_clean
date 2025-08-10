// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_snn_mem__pch.h"

//============================================================
// Constructors

Vtb_snn_mem::Vtb_snn_mem(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_snn_mem__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_snn_mem::Vtb_snn_mem(const char* _vcname__)
    : Vtb_snn_mem(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_snn_mem::~Vtb_snn_mem() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_snn_mem___024root___eval_debug_assertions(Vtb_snn_mem___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_snn_mem___024root___eval_static(Vtb_snn_mem___024root* vlSelf);
void Vtb_snn_mem___024root___eval_initial(Vtb_snn_mem___024root* vlSelf);
void Vtb_snn_mem___024root___eval_settle(Vtb_snn_mem___024root* vlSelf);
void Vtb_snn_mem___024root___eval(Vtb_snn_mem___024root* vlSelf);

void Vtb_snn_mem::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_snn_mem::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_snn_mem___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_snn_mem___024root___eval_static(&(vlSymsp->TOP));
        Vtb_snn_mem___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_snn_mem___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_snn_mem___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_snn_mem::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_snn_mem::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_snn_mem::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_snn_mem___024root___eval_final(Vtb_snn_mem___024root* vlSelf);

VL_ATTR_COLD void Vtb_snn_mem::final() {
    Vtb_snn_mem___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_snn_mem::hierName() const { return vlSymsp->name(); }
const char* Vtb_snn_mem::modelName() const { return "Vtb_snn_mem"; }
unsigned Vtb_snn_mem::threads() const { return 1; }
void Vtb_snn_mem::prepareClone() const { contextp()->prepareClone(); }
void Vtb_snn_mem::atClone() const {
    contextp()->threadPoolpOnClone();
}
