/*============================================================================*/
/* TaskMacros.hpp - task macros                                               */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// variable argument throws

#define GET_THROW(_1,_2,NAME,...) NAME

#define THROW(...) GET_THROW(__VA_ARGS__, THROW2, THROW1)(__VA_ARGS__)

#define THROW1(p1) \
{ \
  s256 sFile((Stz*)__FILE__); \
  s256 sException; \
  sException.vSprint(p1); \
  Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
  pTask->vException(sFile,sException); \
  Debugging::vThrowTrap(); \
  throw (i8)__LINE__; \
}

#define THROW2(p1, p2) \
{ \
  s256 sFile((Stz*)__FILE__); \
  s256 sException; \
  sException.vSprint(p1,p2); \
  Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
  pTask->vException(sFile,sException); \
  Debugging::vThrowTrap(); \
  throw (i8)__LINE__; \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// throw invalid auxiliary stack

#define THROW_INVALID_ASTACK(primitive) \
{ \
  THROW("Invalid aux stack for: %s"_Stz,primitive); \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// throw invalid data stack

#define THROW_INVALID_DSTACK(primitive) \
{ \
  THROW("Invalid data stack in for: %s"_Stz,primitive); \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// throw invalid return stack

#define THROW_INVALID_RSTACK(primitive) \
{ \
  THROW("Invalid return stack for: %s"_Stz,primitive); \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// step entry and exit macros

#ifdef __ENTRY_EXIT
    
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

  #define EXIT() \
    if (pStepAccum) { \
      i8 _iTscExit = Synch::iTicks(); \
      i8 _iTscDiff = _iTscExit - _iTscEntry; \
      pStepAccum->m_nInvoked++; \
      pStepAccum->m_nCycles += _iTscDiff; \
    }

#else

  #define ENTRY(p1)

  #define EXIT()

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
