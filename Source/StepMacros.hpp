/*============================================================================*/
/* StepMacros.hpp - step macros                                               */
/*============================================================================*/
#pragma once


////////////////////////////////////////////////////////////////////////////////
//                         WITH_ENTRY_EXIT defined                            //
////////////////////////////////////////////////////////////////////////////////
#ifdef WITH_ENTRY_EXIT

/*----------------------------------------------------------------------------*/
/* step entry macro                                                           */
/*----------------------------------------------------------------------------*/
#define ENTRY(aStepNm) \
  \
  static StepAccum* _pStepAccum = nullptr; \
  i8 _iEnterCycles = 0; \
  i8 _iEnterMisses = 0; \
  \
  if (AM::pStepTiming()) { \
    if (! _pStepAccum) { \
      StepTiming* pStepTiming = AM::pStepTiming(); \
      pStepTiming->vGrabWrite(); \
      if (! _pStepAccum) \
        _pStepAccum = pStepTiming->pAllocAccum(aStepNm); \
      pStepTiming->vDropWrite(); \
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
//                         WITH_ENTRY_EXIT not defined                        //
////////////////////////////////////////////////////////////////////////////////
#else

#define ENTRY(aStepNm) EndDefine
#define EXIT() EndDefine

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
