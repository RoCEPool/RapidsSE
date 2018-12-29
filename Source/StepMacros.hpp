/*============================================================================*/
/* StepMacros.hpp - step macros                                               */
/*============================================================================*/
#pragma once


/*----------------------------------------------------------------------------*/
/*-- when __ENTRY_EXIT is defined --------------------------------------------*/
/*----------------------------------------------------------------------------*/

#ifdef __ENTRY_EXIT

/*----------------------------------------------------------------------------*/
/* step entry macro                                                           */
/*----------------------------------------------------------------------------*/
#define ENTRY(aStepNm) \
  \
  static StepAccum* _pStepAccum = nullptr; \
  i8 _iEnterCycles = 0; \
  i8 _iEnterMisses = 0; \
  \
  if (gpAttachMap->m_pStepTracker) { \
    if (! _pStepAccum) { \
      StepTracker* pStepTracker = gpAttachMap->m_pStepTracker; \
      pStepTracker->vGrabWrite(); \
      if (! _pStepAccum) \
        _pStepAccum = pStepTracker->pAllocAccum(aStepNm); \
      pStepTracker->vDropWrite(); \
    } \
    _pStepAccum->vEnter(_iEnterCycles,_iEnterMisses); \
  } \
  \
  if (C_TRACE_STEPS && mpTask->m_iTracing) \
    mpTask->vTraceStep(aStepNm);

/*----------------------------------------------------------------------------*/
/* step exit macro                                                            */
/*----------------------------------------------------------------------------*/
#define EXIT() \
  \
  if (_pStepAccum) \
    _pStepAccum->vExit(_iEnterCycles,_iEnterMisses);


/*----------------------------------------------------------------------------*/
/*-- when __ENTRY_EXIT is not defined ----------------------------------------*/
/*----------------------------------------------------------------------------*/

#else

#define ENTRY(aStepNm)

#define EXIT()

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
