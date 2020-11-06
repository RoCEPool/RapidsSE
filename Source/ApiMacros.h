/*============================================================================*/
/* ApiMacros.h -- Api Macros                                                  */
/*============================================================================*/
#pragma once


// API trace levels
const u8 API_TL_ENTRY  = C_BIT_0; // 1
const u8 API_TL_ERROR  = C_BIT_1; // 2
const u8 API_TL_EXIT   = C_BIT_2; // 4
const u8 API_TL_INSERT = C_BIT_3; // 8
const u8 API_TL_POOL   = C_BIT_4; // 16


////////////////////////////////////////////////////////////////////////////////
//                            The Level Macros                                //
////////////////////////////////////////////////////////////////////////////////

#define API_SYSTEM_LEVEL() \
  Session* pSession = nullptr; \
  Job* pJob = nullptr; \
EndDefine

#define API_SESSION_LEVEL() \
  Job* pJob = nullptr; \
EndDefine

#define API_JOB_LEVEL() \
  Job* pJob = _pJob(aJob); \
EndDefine

#define API_LOC(iLocation) \
  SessionArray::vMarkApiLoc(pSession,iLocation); \
EndDefine


////////////////////////////////////////////////////////////////////////////////
//                               API_XYZ Macros                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* mark the location at the beginning an API function                         */
/*----------------------------------------------------------------------------*/
#define API_BEGIN() \
  if (pSession) \
    SessionArray::vMarkApiLoc(pSession,200); \
EndDefine

/*----------------------------------------------------------------------------*/
/* catch an error in an API function                                          */
/*----------------------------------------------------------------------------*/
#define API_CATCH(pError) \
{ \
  eEC eErrorCode = pError ? pError->eErrorCode() : EC_SE_UNKNOWN_ERROR; \
  \
  Engine::vErrorHandler(pError); \
  \
  SessionArray::vMarkError(pSession,eErrorCode); \
  SessionArray::vMarkApiLoc(pSession,888); \
  AM::vLogSsn(SL_INF,pSession,AA_API_ERROR,"Api %s Error %lld"_Stz,eApiCall,eErrorCode); \
  \
  TRACE_API_ERROR("catch"_Stz,eErrorCode); \
  \
  return eErrorCode; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* catch an error in an API function                                          */
/*----------------------------------------------------------------------------*/
#define API_CATCH_NO_SESSION(pError) \
{ \
  eEC eErrorCode = pError ? pError->eErrorCode() : EC_SE_UNKNOWN_ERROR; \
  \
  Engine::vErrorHandler(pError); \
  \
  TRACE_API_ERROR("catch"_Stz,eErrorCode); \
  \
  return eErrorCode; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* enter an API function                                                      */
/*----------------------------------------------------------------------------*/
// not using braces { }, as that would hide the variables
#define API_ENTER(_eApiCall) \
  \
  eAC  eApiCall = _eApiCall; \
  \
  AM::vLogSsn(SL_INF,pSession,AA_API_ENTRY,\
              "Api %s entry"_Stz,aAC[eApiCall]); \
  Api::vIncStatistics(eApiCall);\
  \
  if (pSession) { \
    eEC eError = Api::eValidateSession(iTrace,pSession,eApiCall); \
    if (eError != EC_NONE) \
      API_RETURN_ERROR(EC_INVALID_INTERFACE); \
    \
    SessionArray::vMarkCall(pSession,eApiCall); \
    SessionArray::vMarkApiLoc(pSession,100); \
  } \
  \
  TRACE_API("enter"_Stz); \
EndDefine

/*----------------------------------------------------------------------------*/
/* API function is exiting prior to API_RETURN                                */
/*----------------------------------------------------------------------------*/
#define API_EXITING() \
{ \
  if (pSession) { \
    SessionArray::vMarkReturn(pSession); \
    SessionArray::vMarkApiLoc(pSession,990); \
  } \
  \
  AM::vLogSsn(SL_INF,pSession,AA_API_EXIT,"Api %s exit"_Stz,eApiCall); \
  \
  TRACE_API("exiting"_Stz); \
} \
EndDefine

#ifdef WITH_DEBUG_VERSION
  #define WITH_TRACE_API \
EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* validate not null argument                                                 */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_NOTNULL(p1) \
{ \
  if (! p1) { \
    TRACE_API_ERROR((char*)#p1, EC_INVALID_PARAMETER); \
    return EC_INVALID_PARAMETER; \
  } \
} \
EndDefine

#else
#define API_NOTNULL(p1) \
EndDefine

#endif

/*----------------------------------------------------------------------------*/
/* trace API parameter                                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG_VERSION
#define API_PARAM(p1) \
  TRACE_API_PARAM("param"_Stz,(p1? p1 : C_NULL)); \
EndDefine

#else
#define API_PARAM(p1) \
EndDefine

#endif

/*----------------------------------------------------------------------------*/
/* normal return from API function (no error)                                 */
/*----------------------------------------------------------------------------*/
#define API_RETURN() \
{ \
  if (pSession) { \
    SessionArray::vMarkReturn(pSession); \
    SessionArray::vMarkApiLoc(pSession,999); \
  } \
  \
  AM::vLogSsn(SL_INF,pSession,AA_API_EXIT,"Api %s exit"_Stz,eApiCall); \
  \
  TRACE_API("return"_Stz); \
  \
  return EC_NONE; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* return from an API function with an error code                             */
/*----------------------------------------------------------------------------*/
#define API_RETURN_ERROR(eErrorCode) \
{ \
  SessionArray::vMarkError(pSession,eErrorCode); \
  SessionArray::vMarkApiLoc(pSession,880); \
  AM::vLogSsn(SL_INF,pSession,AA_API_ERROR,"Api %s Error %lld"_Stz,eApiCall,eErrorCode); \
  \
  TRACE_API_ERROR("return error"_Stz,eErrorCode); \
  \
  return eErrorCode; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* return from an API function with an error code                             */
/*----------------------------------------------------------------------------*/
#define API_RETURN_ERROR_NO_SESSION(eErrorCode) \
{ \
  TRACE_API_ERROR("return error"_Stz,eErrorCode); \
  \
  return eErrorCode; \
} \
EndDefine

/*----------------------------------------------------------------------------*/
/* verify buffer pool boundaries                                              */
/*----------------------------------------------------------------------------*/
#define API_VALIDATE_POOL() \
{ \
  bool bPoolValid = gpBufferPool->bVerifyBoundaries(); \
  if (! bPoolValid)  { \
    vTrace("Pool base %p and limit %p are not valid\n"    \
           "JAVA MAY HAVE RE_MAPPED MEMORY SEGMENTS!!!\n"_Stz, \
           gpBufferPool->pBase(), gpBufferPool->pLimit() );  \
    return EC_VALIDATION_FAILURE; \
  } \
} \
EndDefine



////////////////////////////////////////////////////////////////////////////////
//                             TRACE_XYZ Macros                               //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* trace API                                                                  */
/*----------------------------------------------------------------------------*/
#ifdef WITH_TRACE_API
#define TRACE_API(aMarker) \
{ \
  if ( (iTrace & API_TL_ERROR) == API_TL_ERROR) \
    vTrace("!%s(%s): %p(%lld)\n"_Stz,  \
           aAC[eApiCall], aMarker, \
           pSession, pSession ? pSession->tPtnNo() : -1); \
} \
EndDefine

#else
#define TRACE_API(aMarker) \
EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
#ifdef WITH_TRACE_API
#define TRACE_API_ERROR(aMarker, eErrorCode) \
{ \
  if ( (iTrace & API_TL_ERROR) == API_TL_ERROR) \
    vTrace("!%s(%s): %p(%lld) %s(%lld)\n"_Stz,  \
           aAC[eApiCall], aMarker, \
           pSession, pSession ? pSession->tPtnNo() : -1, \
           aEC[eErrorCode], (i8)eErrorCode); \
} \
EndDefine

#else
#define TRACE_API_ERROR(aMarker, eErrorCode) \
EndDefine
#endif

/*----------------------------------------------------------------------------*/
/* trace API param                                                            */
/*----------------------------------------------------------------------------*/
#ifdef WITH_TRACE_API
#define TRACE_API_PARAM(aMarker, sParam) \
{ \
  if ( (iTrace & API_TL_ERROR) == API_TL_ERROR) \
    vTrace("!%s(%s): %p(%lld) %s\n"_Stz,  \
           aAC[eApiCall], aMarker, \
           pSession, pSession ? pSession->tPtnNo() : -1, \
           sParam ? sParam : "<null>"_Stz); \
} \
EndDefine

#else
#define TRACE_API_ERROR(aMarker, eErrorCode) \
EndDefine
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
