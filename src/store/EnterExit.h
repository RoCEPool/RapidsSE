/*============================================================================*/
/* EnterExit.h -- step macros                                                 */
/*============================================================================*/
#pragma once


////////////////////////////////////////////////////////////////////////////////
//                         WITH_STEP_TIMING defined                            //
////////////////////////////////////////////////////////////////////////////////
#ifdef WITH_STEP_TIMING

/*----------------------------------------------------------------------------*/
/* step entry macro                                                           */
/*----------------------------------------------------------------------------*/
#define ENTRY() \
  \
  i8 __iEnterCycles = 0; \
  i8 __iEnterMisses = 0; \
  static StepAccum* __pStepAccum = nullptr; \
  \
  if (Steps* pSteps = AM::pSteps()) { \
  \
    if (! __pStepAccum) { \
    \
      pSteps->vGrabWrite(); \
      if (! __pStepAccum) \
        __pStepAccum = pSteps->pAllocAccum((Stz*)__func__); \
        pSteps->vDropWrite(); \
      } \
    \
    __pStepAccum->vEnter(__iEnterCycles,__iEnterMisses); \
    \
  } \
  \
  if (C_TRACE_STEPS && m_iTraceMask) \
    vTraceStep((Stz*)__func__); \
  \
  vCheckBreakPt(); \
  \
EndDefine

/*----------------------------------------------------------------------------*/
/* step exit macro                                                            */
/*----------------------------------------------------------------------------*/
#define EXIT() \
  \
  if (__pStepAccum) \
    __pStepAccum->vExit(__iEnterCycles,__iEnterMisses); \
  \
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
