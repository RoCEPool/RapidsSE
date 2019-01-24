/*============================================================================*/
/* ApiMacros.hpp -- Api Macros                                                */
/*============================================================================*/
#pragma once



#define API_SYSTEM_LEVEL() \
  Session* pSession = nullptr; \
  Job* pJob = nullptr;

#define API_SESSION_LEVEL() \
  Job* pJob = nullptr;

#define API_JOB_LEVEL() \
  Job* pJob = _pJob(aJob);

#define API_LOC(iLocation) \
  ApiState::vMarkLocation(pSession,iLocation);

#ifdef _DEBUG

/*----------------------------------------------------------------------------*/
/* begin API function                                                         */
/*----------------------------------------------------------------------------*/
#define API_BEGIN() \
  if (pSession) \
    ApiState::vMarkLocation(pSession,200);

/*----------------------------------------------------------------------------*/
/* enter API function                                                         */
/*----------------------------------------------------------------------------*/
#define API_ENTRY(_eApiCall) \
  \
  eAC  eApiCall = _eApiCall; \
  Stz* aApiCall = aAC[eApiCall]; \
  \
  ActivityMap::vMarkApiEntry(eApiCall); \
  Api::vIncStatistics(eApiCall);\
  \
  if (pSession) { \
    \
    eEC eError = Api::eValidateSession(iTrace,pSession,"entry"_Stz); \
    if (eError != EC_NONE) { \
      TRACE_ERROR("entry", EC_INVALID_INTERFACE); \
      return eError; \
    } \
    \
    ApiState::vMarkCall(pSession,eApiCall); \
    ApiState::vMarkLocation(pSession,100); \
    \
    if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
      vTrace(">%s: %p(%lld)\n"_Stz,  \
             aApiCall, pSession , \
             pSession ? pSession->iPartition() : -1); \
  } \
  if (pJob) { \
  } \

// didnt use braces { }, as that would hide the variables

/*----------------------------------------------------------------------------*/
/* error in API function                                                      */
/*----------------------------------------------------------------------------*/
#define API_RETURN_EC(eErrorCode) \
{ \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace("!%s: %p(%lld) %s(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1, \
           aEC[eErrorCode],(i8)eErrorCode); \
  ApiState::vMarkError(pSession,eErrorCode); \
  ApiState::vMarkLocation(pSession,880); \
  ActivityMap::vMarkApiError(eApiCall,(i8)eErrorCode); \
  return eErrorCode; \
}

/*----------------------------------------------------------------------------*/
/* error in API function                                                      */
/*----------------------------------------------------------------------------*/
#define API_CATCH(pError) \
  \
  eEC eErrorCode = pError ? pError->eErrorCode() : EC_SE_UNKNOWN_ERROR; \
  \
  Engine::vErrorHandler(pError); \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace("!%s: %p(%lld) %s(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1, \
           aEC[eErrorCode],(i8)eErrorCode); \
  ApiState::vMarkError(pSession,eErrorCode); \
  ApiState::vMarkLocation(pSession,888); \
  ActivityMap::vMarkApiError(eApiCall,(i8)eErrorCode); \
  return eErrorCode;
// didnt use braces { }, as that would hide the variables

/*----------------------------------------------------------------------------*/
/* exit API function                                                          */
/*----------------------------------------------------------------------------*/
#define API_EXITING() \
{  \
  if ( (iTrace & TL_EXIT) == TL_EXIT) \
    vTrace(">%s: %p(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1); \
  \
  if (pSession) { \
    ApiState::vMarkReturn(pSession); \
    ApiState::vMarkLocation(pSession,990); \
  } \
  \
  ActivityMap::vMarkApiExit(eApiCall); \
}

/*----------------------------------------------------------------------------*/
/* exit API function                                                          */
/*----------------------------------------------------------------------------*/
#define API_RETURN() \
{  \
  if ( (iTrace & TL_EXIT) == TL_EXIT) \
    vTrace(">%s: %p(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1); \
  \
  if (pSession) { \
    ApiState::vMarkReturn(pSession); \
    ApiState::vMarkLocation(pSession,999); \
  } \
  \
  ActivityMap::vMarkApiExit(eApiCall); \
  return EC_NONE; \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define API_PARAM_P1(p1) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, p1); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define API_PARAM_P2(p1, p2) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, \
        p1 ? p1 : "null"_Stz , p2 ? p2 : "null"_Stz); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define API_PARAM_P3(p1, p2, p3) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, \
        p1 ? p1 : "null"_Stz, p2 ? p2 : "null"_Stz , p3 ? p3 : "null"_Stz); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define API_PARAM_P4(p1, p2, p3, p4) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, p1, p2, p3, p4); \
}

/*----------------------------------------------------------------------------*/
/* validate reference pointer                                                 */
/*----------------------------------------------------------------------------*/
#define API_NOTNULL(p1) \
{ \
  if (! p1) { \
    TRACE_ERROR((char*)#p1, EC_INVALID_PARAMETER); \
    return EC_INVALID_PARAMETER; \
  } \
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ERROR(aMarker, eErrorCode) \
{  \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace(">%s(%s): %p(%lld) %s(%lld)\n"_Stz,  \
           aApiCall, aMarker, pSession, \
           pSession ? pSession->iPartition() : -1, aEC[eErrorCode], \
           (i8)eErrorCode); \
}


/*----------------------------------------------------------------------------*/
/* verify buffer pool boundaries                                              */
/*----------------------------------------------------------------------------*/
#define VALIDATE_POOL() \
{  \
  bool bPoolValid; \
  if ( (iTrace & TL_POOL) == TL_POOL) { \
    bPoolValid = gpBufferPool->bVerifyBoundaries(true); \
  } \
  else { \
    bPoolValid = gpBufferPool->bVerifyBoundaries(false); \
  } \
  if ( ! bPoolValid )  { \
    vTrace("Pool base and limit are not valid!!!"    \
           "JAVA MAY HAVE RE_MAPPED MEMORY SEGMENTS!!!\n"_Stz);  \
    return EC_VALIDATION_FAILURE; \
  } \
}

#else

#define API_CATCH(pError) \
  \
  eEC eErrorCode = pError ? pError->eErrorCode() : EC_SE_UNKNOWN_ERROR; \
  \
  Engine::vErrorHandler(pError); \
  return eErrorCode;

#define API_BEGIN()
#define API_ENTRY(__eApiCall)
#define API_RETURN_EC(eErrorCode) return eErrorCode
#define API_ERROR()
#define API_EXIT()
#define API_EXITING()
#define API_NOTNULL(p1)

#define API_PARAM_P1(p1)
#define API_PARAM_P2(p1, p2)
#define API_PARAM_P3(p1, p2, p3)
#define API_PARAM_P4(p1, p2, p3, p4)

#define API_RETURN() return EC_NONE;

#define TRACE_ERROR(aMarker, eErrorCode)
#define VALIDATE_POOL()

#endif


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
