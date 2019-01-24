/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/
#pragma once

// API trace levels
#define TL_ENTRY  1
#define TL_ERROR  2
#define TL_EXIT   4
#define TL_INSERT 8
#define TL_POOL   16

enum eAC // api call kind
{
  /*  0 */ AC_NONE=0,
  
  /*  1 */ AC_BeginInsert,
  /*  2 */ AC_BeginSubInserter,
  /*  3 */ AC_CheckVersion,
  /*  4 */ AC_CopyBuffer,
  /*  5 */ AC_DeleteSession,
  /*  6 */ AC_EndInsert,
  /*  7 */ AC_EndJobs,
  /*  8 */ AC_EndSubInserter,
  /*  9 */ AC_ExecuteCommand,
  /* 10 */ AC_FetchNextBuffer,
  /* 11 */ AC_GetLoadResult,
  /* 12 */ AC_GetRecordBuffer,
  /* 13 */ AC_GetRow,
  /* 14 */ AC_GetRowCount,
  /* 15 */ AC_GetTableColumnRow,
  /* 16 */ AC_GetTableColumnRowCount,
  /* 17 */ AC_InsertRow,
  /* 18 */ AC_IsInvalidSession,
  /* 19 */ AC_NewSession,
  /* 20 */ AC_OpenPool,
  /* 21 */ AC_ScanNextRow,
  /* 22 */ AC_TranslateError,

  /* 23 */ AC_ENUMS
};

static Stz* aAC[]
{
  /*  0 */ "NONE"_Stz,

  /*  1 */ "RapidsSE_BeginInsert"_Stz,
  /*  2 */ "RapidsSE_BeginSubInserter"_Stz,
  /*  3 */ "RapidsSE_CheckVersion"_Stz,
  /*  4 */ "RapidsSE_CopyBuffer"_Stz,
  /*  5 */ "RapidsSE_DeleteSession"_Stz,
  /*  6 */ "RapidsSE_EndInsert"_Stz,
  /*  7 */ "RapidsSE_EndJobs"_Stz,
  /*  8 */ "RapidsSE_EndSubInserter"_Stz,
  /*  9 */ "RapidsSE_ExecuteCommand"_Stz,
  /* 10 */ "RapidsSE_FetchNextBuffer"_Stz,
  /* 11 */ "RapidsSE_GetLoadResult"_Stz,
  /* 12 */ "RapidsSE_GetRecordBuffer"_Stz,
  /* 13 */ "RapidsSE_GetRow"_Stz,
  /* 14 */ "RapidsSE_GetRowCount"_Stz,
  /* 15 */ "RapidsSE_GetTableColumnRow"_Stz,
  /* 16 */ "RapidsSE_GetTableColumnRowCount"_Stz,
  /* 17 */ "RapidsSE_InsertRow"_Stz,
  /* 18 */ "RapidsSE_IsInvalidSession"_Stz,
  /* 19 */ "RapidsSE_NewSession"_Stz,
  /* 20 */ "RapidsSE_OpenPool"_Stz,
  /* 21 */ "RapidsSE_ScanNextRow"_Stz,
  /* 22 */ "RapidsSE_TranslateError"_Stz,

  /* 23 */ "ENUMS"_Stz};

////////////////////////////////////////////////////////////////////////////////

// System Level Calls
extern "C" eEC  RapidsSE_CheckVersion     (i8 iTrace,
                                           Stz* sVersion);
extern "C" eEC  RapidsSE_DeleteSession    (i8 iTrace,
                                           Session** ppSession);
extern "C" bool RapidsSE_IsInvalidSession (i8 iTrace,
                                           int eError);
extern "C" eEC  RapidsSE_NewSession       (i8 iTrace,
                                           Stz* sCatalog, Stz* sSchema, i8 iPartition,
                                           void** aaSession);
extern "C" eEC  RapidsSE_OpenPool         (i8 iTrace,
                                           Stz* sRegion, void** aaAttachMap);

////////////////////////////////////////////////////////////////////////////////

// Session Level Calls
extern "C" eEC  RapidsSE_BeginInsert     (i8 iTrace, Session* pSession,
                                          Stz* sTable, i8 nSubInserters, void** aaJob);
extern "C" eEC  RapidsSE_EndJobs         (i8 iTrace, Session* pSession);
extern "C" eEC  RapidsSE_ExecuteCommand  (i8 iTrace, Session* pSession,
                                          Stz* sCatalog, Stz* sSchema, Stz* sCommand,
                                          void** aaJob);
extern "C" eEC  RapidsSE_InsertRow       (i8 iTrace, Session* pSession,
                                          Stz* pTable, Stz* sRecord);
extern "C" eEC  RapidsSE_TranslateError  (i8 iTrace, Session* pSession,
                                          int iErrorCode, void** aaError);

////////////////////////////////////////////////////////////////////////////////

// Job Level Calls
extern "C" eEC  RapidsSE_BeginSubInserter(i8 iTrace, Session* pSession, void* aJob,
                                          void** aaJob);
extern "C" eEC  RapidsSE_CopyBuffer      (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_EndInsert       (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_EndSubInserter  (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_FetchNextBuffer (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaFetchBuffer, i8* aiExepectedRows);
extern "C" eEC  RapidsSE_GetLoadResult   (i8 iTrace, Session* pSession, void* aJob,
                                          i8* aiValidRows, i8* aiInvalidRows, i8* aiSkippedRows);
extern "C" eEC  RapidsSE_GetRecordBuffer (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaBuffer);
extern "C" eEC  RapidsSE_GetRow          (i8 iTrace, Session* pSession, void* aJob,
                                          i8 iRow, void** aaRow);
extern "C" eEC  RapidsSE_GetRowCount     (i8 iTrace, Session* pSession, void* aJob,
                                          i8 *aiRowCount);
extern "C" eEC  RapidsSE_GetTableColumnRow(i8 iTrace, Session* pSession, void* aJob,
                                           i8 i_Col, void** aaRow);
extern "C" eEC  RapidsSE_GetTableColumnRowCount(i8 iTrace, Session* pSession, void* aJob,
                                                i8* aRowCount);
extern "C" eEC  RapidsSE_ScanNextRow     (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaEntryBuffer, i8* aiExpectedRows,
                                          void** aaEntry, void** aaRow);

////////////////////////////////////////////////////////////////////////////////


void vApiCall(Session* pSession, eAC eApiCall);
void vApiError(Session* pSession, eEC eError);
void vApiJob(Session* pSession, Job* aJob);
void vApiLocation(Session* pSession, i8 iLocation);
void vApiReturn(Session* pSession);


////////////////////////////////////////////////////////////////////////////////

namespace Interface
{
  class Api
  {
  public:
   static void vDeleteJobs(Session* pSession);
   static Job* pExecuteCommand(Session* pSession, Stz* sCommand,
                               bool bPostJob, bool bFromPrompt);
   static void vIncStatistics(eAC eApiCall);
   static void vMonitorPortrait(s48K& sPortrait);
   static bool bSpawnJob(Session* pSession, Job* aJob, eSS eSeed);
   static void vSpawnJobAndWait(Session* pSession, Job* aJob);
  };
}


#define API_SYSTEM_LEVEL() \
  Session* pSession = nullptr; \
  Job* pJob = nullptr;

#define API_SESSION_LEVEL() \
  Job* pJob = nullptr;

#define API_JOB_LEVEL() \
  Job* pJob = _pJob(aJob);

#define API_LOC(iLocation) \
  vApiLocation(pSession,iLocation);

#ifdef _DEBUG

/*----------------------------------------------------------------------------*/
/* begin API function                                                         */
/*----------------------------------------------------------------------------*/
#define API_BEGIN() \
  if (pSession) \
    vApiLocation(pSession,200);

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
    eEC eResult = eValidateSession(iTrace,pSession,"entry"_Stz); \
    if (eResult != EC_NONE) { \
      TRACE_ERROR("entry", EC_INVALID_INTERFACE); \
      return eResult; \
    } \
    \
    vApiCall(pSession,eApiCall); \
    vApiLocation(pSession,100); \
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
  vApiError(pSession,eErrorCode); \
  vApiLocation(pSession,880); \
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
  RapidsSE::vErrorHandler(pError); \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace("!%s: %p(%lld) %s(%lld)\n"_Stz,  \
           aApiCall, pSession, \
           pSession ? pSession->iPartition() : -1, \
           aEC[eErrorCode],(i8)eErrorCode); \
  vApiError(pSession,eErrorCode); \
  vApiLocation(pSession,888); \
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
    vApiReturn(pSession); \
    vApiLocation(pSession,990); \
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
    vApiReturn(pSession); \
    vApiLocation(pSession,999); \
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
  RapidsSE::vErrorHandler(pError); \
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
#define TRACE_EXIT(aMarker)
#define VALIDATE_NOINDEX(aMarker)
#define VALIDATE_POOL()

#endif

/*----------------------------------------------------------------------------*/
/* trace API call entry                                                       */
/*----------------------------------------------------------------------------*/
inline void vTraceEntry(i8 iTrace, Stz* aFormat, ...)
{
  // trace entry ?
  if ( (iTrace & TL_ENTRY) != TL_ENTRY)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  Debugging::vPrint(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* trace API call exit                                                        */
/*----------------------------------------------------------------------------*/
inline void vTraceExit(i8 iTrace, Stz* aFormat, ...)
{
  // trace entry ?
  if ( (iTrace & TL_EXIT) != TL_EXIT)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  Debugging::vPrint(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
inline void vTraceError(i8 iTrace, Stz* aFormat, ...)
{
  // trace entry ?
  if ( (iTrace & TL_ERROR) != TL_ERROR)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  Debugging::vPrint(aFormat,xArgs);
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
inline eEC eValidateSession(i8 iTrace, Session* pSession, Stz* aApiCall)
{
  
  // pSession cannot be null
  if (pSession == nullptr) {
    TRACE_ERROR("va", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be a pool address
  if (!gpBufferPool->bValidAddress(pSession)) {
    TRACE_ERROR("vb", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be an SESSION class
  if (pSession->eClass() != CN_SESSION) {
    TRACE_ERROR("vc", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
