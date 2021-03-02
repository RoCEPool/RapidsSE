/*============================================================================*/
/* StepMacros.hpp - step macros                                               */
/*============================================================================*/
#pragma once


////////////////////////////////////////////////////////////////////////////////
//                         WITH_STEP_TIMING defined                            //
////////////////////////////////////////////////////////////////////////////////
#ifdef WITH_STEP_TIMING

/*----------------------------------------------------------------------------*/
/* step entry macro                                                           */
/*----------------------------------------------------------------------------*/
#define ENTRY(aStepNm) \
  \
  static StepAccum* _pStepAccum = nullptr; \
  i8 _iEnterCycles = 0; \
  i8 _iEnterMisses = 0; \
  \
  if (AM::pSteps()) { \
    if (! _pStepAccum) { \
      Steps* pSteps = AM::pSteps(); \
      pSteps->vGrabWrite(); \
      if (! _pStepAccum) \
        _pStepAccum = pSteps->pAllocAccum(aStepNm); \
      pSteps->vDropWrite(); \
    } \
    _pStepAccum->vEnter(_iEnterCycles,_iEnterMisses); \
  } \
  \
  if (C_TRACE_STEPS && mpTask->m_iTracing) \
    mpTask->vTraceStep(aStepNm); \
EndDefine

/*----------------------------------------------------------------------------*/
/* step exit macro                                                            */
/*----------------------------------------------------------------------------*/
#define EXIT() \
  \
  if (_pStepAccum) \
    _pStepAccum->vExit(_iEnterCycles,_iEnterMisses); \
EndDefine


////////////////////////////////////////////////////////////////////////////////
//                         WITH_STEP_TIMING not defined                        //
////////////////////////////////////////////////////////////////////////////////
#else

#define ENTRY(aStepNm) EndDefine
#define EXIT() EndDefine

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
