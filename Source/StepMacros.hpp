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
    i8 _iTscEntry = 0; \
    \
    if (gpAttachMap->m_pStepTracker) { \
      if (! pStepAccum) { \
        gpAttachMap->m_pStepTracker->vGrabWrite(); \
        if (! pStepAccum) \
          pStepAccum = gpAttachMap->m_pStepTracker->pAllocAccum(aStepNm); \
        gpAttachMap->m_pStepTracker->vDropWrite(); \
      } \
      _iTscEntry = Synch::iTicks(); \
    } \
    \
    if (C_TRACE_STEPS && mpTask->m_iTracing) \
      mpTask->vTraceStep(aStepNm);

/*----------------------------------------------------------------------------*/
/* step exit macro                                                            */
/*----------------------------------------------------------------------------*/
  #define EXIT() \
    if (pStepAccum) { \
      i8 _iTscExit = Synch::iTicks(); \
      i8 _iTscDiff = _iTscExit - _iTscEntry; \
      pStepAccum->m_nInvoked++; \
      pStepAccum->m_nSumCycles += _iTscDiff; \
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
