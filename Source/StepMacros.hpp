/*============================================================================*/
/* StepMacros.hpp - step macros                                               */
/*============================================================================*/
#pragma once


#ifdef __ENTRY_EXIT
    
/*----------------------------------------------------------------------------*/
/* step entry macro                                                           */
/*----------------------------------------------------------------------------*/
  #define ENTRY(aStepNm) \
    \
    static StepAccum* pStepAccum = nullptr; \
    i8 _iEnterCycles = 0; \
    i8 _iEnterMisses = 0; \
    \
    if (gpAttachMap->m_pStepTracker) { \
      if (! pStepAccum) { \
        StepTracker* pStepTracker = gpAttachMap->m_pStepTracker; \
        pStepTracker->vGrabWrite(); \
        if (! pStepAccum) \
          pStepAccum = pStepTracker->pAllocAccum(aStepNm); \
        pStepTracker->vDropWrite(); \
      } \
      pStepAccum->vEnter(_iEnterCycles,_iEnterMisses); \
    } \
    \
    if (C_TRACE_STEPS && mpTask->m_iTracing) \
      mpTask->vTraceStep(aStepNm);

/*----------------------------------------------------------------------------*/
/* step exit macro                                                            */
/*----------------------------------------------------------------------------*/
  #define EXIT() \
    \
    if (pStepAccum) { \
      pStepAccum->vExit(_iEnterCycles,_iEnterMisses); \
    }

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#else

  #define ENTRY(p1)

  #define EXIT()

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
