/*============================================================================*/
/* ApiMacros.hpp -- Api Macros                                                */
/*============================================================================*/
#pragma once


// API trace levels
#define TL_ENTRY  C_BIT_0 // 1
#define TL_ERROR  C_BIT_1 // 2
#define TL_EXIT   C_BIT_2 // 4
#define TL_INSERT C_BIT_3 // 8
#define TL_POOL   C_BIT_4 // 16


////////////////////////////////////////////////////////////////////////////////
//                                 The Level Macros                           //
////////////////////////////////////////////////////////////////////////////////

#define API_SYSTEM_LEVEL() \
  Session* pSession = nullptr; \
  Job* pJob = nullptr;

#define API_SESSION_LEVEL() \
  Job* pJob = nullptr;

#define API_JOB_LEVEL() \
  Job* pJob = _pJob(aJob);

#define API_LOC(iLocation) \
  ApiState::vMarkApiLoc(pSession,iLocation);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* begin an API function                                                      */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_BEGIN() \
  if (pSession) \
    ApiState::vMarkApiLoc(pSession,200);

#else
#define API_BEGIN()

#endif

/*----------------------------------------------------------------------------*/
/* catch an error in an API function                                          */
/*----------------------------------------------------------------------------*/
// not using braces { }, as that would hide the variables

#ifdef WITH_DEBUG_VERSION
#define API_CATCH(pError) \
  \
  eEC eErrorCode = pError ? pError->eErrorCode() : EC_SE_UNKNOWN_ERROR; \
  \
  Engine::vErrorHandler(pError); \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace("<!%s: %p(%lld) %s(%lld)\n"_Stz,  \
            aApiCall, pSession, \
            pSession ? pSession->iPartition() : -1, \
            aEC[eErrorCode],(i8)eErrorCode); \
  ApiState::vMarkError(pSession,eErrorCode); \
  ApiState::vMarkApiLoc(pSession,888); \
  ActivityMap::vMarkApiError(eApiCall,(i8)eErrorCode); \
  return eErrorCode;

#else
#define API_CATCH(pError) \
  \
  eEC eErrorCode = pError ? pError->eErrorCode() : EC_SE_UNKNOWN_ERROR; \
  \
  Engine::vErrorHandler(pError); \
  return eErrorCode;

#endif

/*----------------------------------------------------------------------------*/
/* enter an API function                                                      */
/*----------------------------------------------------------------------------*/
// not using braces { }, as that would hide the variables

#ifdef WITH_DEBUG_VERSION
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
      API_RETURN_EC(EC_INVALID_INTERFACE); \
    } \
    \
    ApiState::vMarkCall(pSession,eApiCall); \
    ApiState::vMarkApiLoc(pSession,100); \
    \
    if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
      vTrace(">%s: %p(%lld)\n"_Stz,  \
             aApiCall, pSession , \
             pSession ? pSession->iPartition() : -1); \
  }

#else
#define API_ENTRY(eApiCall)

#endif

/*----------------------------------------------------------------------------*/
/* API function is exiting prior to API_RETURN                                */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_EXITING() \
{ \
  if ( (iTrace & TL_EXIT) == TL_EXIT) \
    vTrace("<%s: %p(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1); \
  \
  if (pSession) { \
    ApiState::vMarkReturn(pSession); \
    ApiState::vMarkApiLoc(pSession,990); \
  } \
  \
  ActivityMap::vMarkApiExit(eApiCall); \
}

#else
#define API_EXITING()

#endif

/*----------------------------------------------------------------------------*/
/* validate not null argument                                                 */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_NOTNULL(p1) \
{ \
  if (! p1) { \
    API_TRACE_ERROR((char*)#p1, EC_INVALID_PARAMETER); \
    return EC_INVALID_PARAMETER; \
  } \
}

#else
#define API_NOTNULL(p1)

#endif

/*----------------------------------------------------------------------------*/
/* trace API parameter                                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_PARAM(p1) \
{ \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("%s: %p(%lld) %s\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1, p1); \
}
#else
#define API_PARAM(p1)

#endif

/*----------------------------------------------------------------------------*/
/* API function is returning                                                  */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_RETURN() \
{ \
  if ( (iTrace & TL_EXIT) == TL_EXIT) \
    vTrace("<%s: %p(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1); \
  \
  if (pSession) { \
    ApiState::vMarkReturn(pSession); \
    ApiState::vMarkApiLoc(pSession,999); \
  } \
  \
  ActivityMap::vMarkApiExit(eApiCall); \
  return EC_NONE; \
}

#else
#define API_RETURN() return EC_NONE;

#endif

/*----------------------------------------------------------------------------*/
/* return error code from an API function                                     */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_RETURN_EC(eErrorCode) \
{ \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace("<!%s: %p(%lld) %s(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1, \
           aEC[eErrorCode],(i8)eErrorCode); \
  ApiState::vMarkError(pSession,eErrorCode); \
  ApiState::vMarkApiLoc(pSession,880); \
  ActivityMap::vMarkApiError(eApiCall,(i8)eErrorCode); \
  return eErrorCode; \
}

#else
#define API_RETURN_EC(eErrorCode) return eErrorCode

#endif

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_TRACE_ERROR(aMarker, eErrorCode) \
{ \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace("!%s(%s): %p(%lld) %s(%lld)\n"_Stz,  \
           aApiCall, aMarker, pSession, \
           pSession ? pSession->iPartition() : -1, \
           aEC[eErrorCode], (i8)eErrorCode); \
}
#else
#define API_TRACE_ERROR(aMarker, eErrorCode)

#endif

/*----------------------------------------------------------------------------*/
/* verify buffer pool boundaries                                              */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_VALIDATE_POOL() \
{ \
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
#define API_VALIDATE_POOL()

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
