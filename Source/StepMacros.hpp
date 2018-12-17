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
        gpAttachMap->m_pStepTracker->vGrabWrite(); \
        if (! pStepAccum) \
          pStepAccum = gpAttachMap->m_pStepTracker->pAllocAccum(aStepNm); \
        gpAttachMap->m_pStepTracker->vDropWrite(); \
      } \
      _iEnterCycles = Synch::iTicks(); \
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
      i8 _iExitCycles = Synch::iTicks(); \
      i8 _iDiffCycles = _iExitCycles - _iEnterCycles; \
      pStepAccum->m_nInvoked++; \
      pStepAccum->m_nSumCycles += _iDiffCycles; \
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
