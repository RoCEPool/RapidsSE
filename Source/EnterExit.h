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
// this variant uses __func__ instead of szStepNm
#define ENTRY() \
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
        _pStepAccum = pStepTiming->pAllocAccum((Stz*)__func__); \
        pStepTiming->vDropWrite(); \
      } \
    _pStepAccum->vEnter(_iEnterCycles,_iEnterMisses); \
  } \
  \
  if (C_TRACE_STEPS && m_pTask->m_iTraceMask) \
    m_pTask->vTraceStep((Stz*)__func__); \
EndDefine

// this variant uses __func__ instead of szStepNm
//#define ENTRY(szStepNm) \
//  \
//  static StepAccum* _pStepAccum = nullptr; \
//  i8 _iEnterCycles = 0; \
//  i8 _iEnterMisses = 0; \
//  \
//  if (AM::pStepTiming()) { \
//    if (! _pStepAccum) { \
//      StepTiming* pStepTiming = AM::pStepTiming(); \
//      pStepTiming->vGrabWrite(); \
//      if (! _pStepAccum) \
//        _pStepAccum = pStepTiming->pAllocAccum((Stz*)__func__); \
//      pStepTiming->vDropWrite(); \
//    } \
//    _pStepAccum->vEnter(_iEnterCycles,_iEnterMisses); \
//  } \
//  \
//  if (C_TRACE_STEPS && m_pTask->m_iTraceMask) \
//    m_pTask->vTraceStep((Stz*)__func__); \
//EndDefine

// this is the original variant which uses szStepNm
//#define ENTRY(szStepNm) \
//  \
//  static StepAccum* _pStepAccum = nullptr; \
//  i8 _iEnterCycles = 0; \
//  i8 _iEnterMisses = 0; \
//  \
//  if (AM::pStepTiming()) { \
//    if (! _pStepAccum) { \
//      StepTiming* pStepTiming = AM::pStepTiming(); \
//      pStepTiming->vGrabWrite(); \
//      if (! _pStepAccum) \
//        _pStepAccum = pStepTiming->pAllocAccum(szStepNm); \
//      pStepTiming->vDropWrite(); \
//    } \
//    _pStepAccum->vEnter(_iEnterCycles,_iEnterMisses); \
//  } \
//  \
//  if (C_TRACE_STEPS && m_pTask->m_iTraceMask) \
//    m_pTask->vTraceStep(szStepNm); \
//EndDefine

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
