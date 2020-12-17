/*============================================================================*/
/* EnterExit.h -- step macros                                                 */
/*============================================================================*/
#pragma once


////////////////////////////////////////////////////////////////////////////////
//                         WITH_ENTRY_EXIT defined                            //
////////////////////////////////////////////////////////////////////////////////
#ifdef WITH_ENTRY_EXIT

/*----------------------------------------------------------------------------*/
/* step entry macro                                                           */
/*----------------------------------------------------------------------------*/
#define ENTRY() \
  \
  i8 __iEnterCycles = 0; \
  i8 __iEnterMisses = 0; \
  static StepAccum* __pStepAccum = nullptr; \
  \
  if (StepTiming* pStepTiming = AM::pStepTiming()) { \
  \
    if (! __pStepAccum) { \
    \
      pStepTiming->vGrabWrite(); \
      if (! __pStepAccum) \
        __pStepAccum = pStepTiming->pAllocAccum((Stz*)__func__); \
        pStepTiming->vDropWrite(); \
      } \
    \
    __pStepAccum->vEnter(__iEnterCycles,__iEnterMisses); \
    \
  } \
  \
  if (C_TRACE_STEPS && m_pTask->m_iTraceMask) \
    m_pTask->vTraceStep((Stz*)__func__); \
  \
  vCheckBreakPt();
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
//                         WITH_ENTRY_EXIT not defined                        //
////////////////////////////////////////////////////////////////////////////////
#else

#define ENTRY(aStepNm) EndDefine
#define EXIT() EndDefine

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
