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
  
  /*  1 */ AC_BeginInsertTable,
  /*  2 */ AC_BeginSubInserter,
  /*  3 */ AC_CheckLibraryVersion,
  /*  4 */ AC_CopyBuffer,
  /*  5 */ AC_EndInsertTable,
  /*  6 */ AC_EndJobs,
  /*  7 */ AC_EndSubInserter,
  /*  8 */ AC_ExecCmdStr,
  /*  9 */ AC_ExecuteCommand,
  /* 10 */ AC_FetchNextBuffer,
  /* 11 */ AC_GetRecordBuffer,
  /* 12 */ AC_GetErrorMessage,
  /* 13 */ AC_GetRow,
  /* 14 */ AC_GetRowCount,
  /* 15 */ AC_GetRowCountTableColumn,
  /* 16 */ AC_GetRowTableColumn,
  /* 17 */ AC_InsertRow,
  /* 18 */ AC_LoadResult,
  /* 19 */ AC_PoolOpen,
  /* 20 */ AC_ScanNextRow,
  /* 21 */ AC_SessionDelete,
  /* 22 */ AC_SessionInvalid,
  /* 23 */ AC_SessionNew,
  
  /* 24 */ AC_ENUMS
};

static Stz* aAC[]
{
  /*  0 */ "NONE"_Stz,

  /*  1 */ "RapidsSE_BeginInsertTable"_Stz,
  /*  2 */ "RapidsSE_BeginSubInserter"_Stz,
  /*  3 */ "RapidsSE_CheckLibraryVersion"_Stz,
  /*  4 */ "RapidsSE_CopyBuffer"_Stz,
  /*  5 */ "RapidsSE_EndInsertTable"_Stz,
  /*  6 */ "RapidsSE_EndJobs"_Stz,
  /*  7 */ "RapidsSE_EndSubInserter"_Stz,
  /*  8 */ "RapidsSE_ExecCmdStr"_Stz,
  /*  9 */ "RapidsSE_ExecuteCommand"_Stz,
  /* 10 */ "RapidsSE_FetchNextBuffer"_Stz,
  /* 11 */ "RapidsSE_GetRecordBuffer"_Stz,
  /* 12 */ "RapidsSE_GetErrorMessage"_Stz,
  /* 13 */ "RapidsSE_GetRow"_Stz,
  /* 14 */ "RapidsSE_GetRowCount"_Stz,
  /* 15 */ "RapidsSE_GetRowCountTableColumn"_Stz,
  /* 16 */ "RapidsSE_GetRowTableColumn"_Stz,
  /* 17 */ "RapidsSE_InsertRow"_Stz,
  /* 18 */ "RapidsSE_LoadResult"_Stz,
  /* 19 */ "RapidsSE_PoolOpen"_Stz,
  /* 20 */ "RapidsSE_ScanNextRow"_Stz,
  /* 21 */ "RapidsSE_SessionDelete"_Stz,
  /* 22 */ "RapidsSE_SessionInvalid"_Stz,
  /* 23 */ "RapidsSE_SessionNew"_Stz
  /* 24 */ "ENUMS"_Stz};

////////////////////////////////////////////////////////////////////////////////

// RapidSE level calls
extern "C" eEC  RapidsSE_CheckLibraryVersion(i8 iTrace,
                                             Stz* sVersion);
extern "C" eEC  RapidsSE_PoolOpen           (i8 iTrace,
                                             Stz* sRegion, void** aaAttachMap);
extern "C" bool RapidsSE_SessionInvalid     (i8 iTrace,
                                             int eError);
extern "C" eEC  RapidsSE_SessionNew         (i8 iTrace,
                                             void** aaSession,
                                             Stz* sCatalog, Stz* sSchema, i8 iPartition);

////////////////////////////////////////////////////////////////////////////////

// session level calls
extern "C" eEC  RapidsSE_BeginInsertTable(i8 iTrace, Session* pSession,
                                          Stz* sTable, void** aaJob, i8 nSubInserters);
extern "C" eEC  RapidsSE_EndJobs         (i8 iTrace, Session* pSession);
extern "C" eEC  RapidsSE_ExecuteCommand  (i8 iTrace, Session* pSession,
                                          Stz* sCommand, Stz* sCatalog, Stz* sSchema,
                                          void** aaJob);
extern "C" eEC  RapidsSE_GetErrorMessage (i8 iTrace, Session* pSession,
                                          int iErrorCode, void** aaError);
extern "C" eEC  RapidsSE_InsertRow       (i8 iTrace, Session* pSession,
                                          Stz* pTable, Stz* sRecord);
extern "C" eEC  RapidsSE_SessionDelete   (i8 iTrace, Session* pSession);

////////////////////////////////////////////////////////////////////////////////

// job level calls
extern "C" eEC  RapidsSE_BeginSubInserter(i8 iTrace, Session* pSession, void* aJob,
                                          void** aaJob);
extern "C" eEC  RapidsSE_CopyBuffer      (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_EndInsertTable  (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_EndSubInserter  (i8 iTrace, Session* pSession, void* aJob);
extern "C" eEC  RapidsSE_FetchNextBuffer (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaFetchBuffer, i8* aiExepectedRows);
extern "C" eEC  RapidsSE_GetRecordBuffer (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaBuffer);
extern "C" eEC  RapidsSE_GetRow          (i8 iTrace, Session* pSession, void* aJob,
                                          i8 iRow, void** aaRow);
extern "C" eEC  RapidsSE_GetRowCount     (i8 iTrace, Session* pSession, void* aJob,
                                          i8 *aRowCount);
extern "C" eEC  RapidsSE_GetRowTableColumn(i8 iTrace, Session* pSession, void* aJob,
                                           i8 iRow, void** aaRow);
extern "C" eEC  RapidsSE_GetRowCountTableColumn(i8 iTrace, Session* pSession, void* aJob,
                                                i8* aRowCount);
extern "C" eEC  RapidsSE_LoadResult      (i8 iTrace, Session* pSession, void* aJob,
                                          i8* aiValidRows, i8* aiInvalidRows, i8* aiSkippedRows);
extern "C" eEC  RapidsSE_ScanNextRow     (i8 iTrace, Session* pSession, void* aJob,
                                          void** aaEntryBuffer, void** aaEntry,
                                          void** aaRow, i8* aiExepectedRows);


Job* RapidsSE_pExecCmdStr(Stz* sCommand, Session* pSession, bool bPostJob, bool bFromPrompt);
bool RapidsSE_bSpawnJob(Job* aJob, eSS eSeed, Session* pSession);

void vApiCall(Session* pSession, eAC eApiCall);
void vApiError(Session* pSession, eEC eError);
void vApiJob(Session* pSession, Job* aJob);
void vApiLocation(Session* pSession, i8 iLocation);
void vApiReturn(Session* pSession);
void vDeleteJobs(Session* pSession);
void vSpawnJobAndWait(Session* pSession, Job* aJob);

void vApiStatistics(eAC eApiCall);
void RapidsSE_vMonitorPortrait(s48K& sPortrait);


#ifdef _DEBUG


/*----------------------------------------------------------------------------*/
/* enter API function                                                         */
/*----------------------------------------------------------------------------*/
#define API_ENTRY(_eApiCall) \
\
  eAC  eApiCall = _eApiCall; \
  Stz* aApiCall = aAC[eApiCall]; \
  ActivityMap::vMarkApiEntry(eApiCall); \
  vApiStatistics(eApiCall);\
  if (pSession) { \
    vApiCall(pSession,eApiCall); \
    vApiLocation(pSession,100); \
  }
// didnt use braces { }, as that would hide the variables

/*----------------------------------------------------------------------------*/
/* error in API function                                                      */
/*----------------------------------------------------------------------------*/
#define API_ERROR() \
{  \
  ActivityMap::vMarkApiError(_eApiCall); \
}

/*----------------------------------------------------------------------------*/
/* exit API function                                                          */
/*----------------------------------------------------------------------------*/
#define API_EXIT() \
{  \
  ActivityMap::vMarkApiEntry(_eApiCall); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P0() \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld)\n"_Stz,  \
      aApiCall, pSession , pSession ? pSession->iPartition() : -1); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P1(p1) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, p1); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P2(p1, p2) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, \
        p1 ? p1 : "null"_Stz , p2 ? p2 : "null"_Stz); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P3(p1, p2, p3) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, \
        p1 ? p1 : "null"_Stz, p2 ? p2 : "null"_Stz , p3 ? p3 : "null"_Stz); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P4(p1, p2, p3, p4) \
{  \
  if ( (iTrace & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, p1, p2, p3, p4); \
}

/*----------------------------------------------------------------------------*/
/* trace API insert                                                           */
/*----------------------------------------------------------------------------*/
#define TRACE_INSERT(aMarker, sRecord) \
{  \
  if ( (iTrace & TL_INSERT) == TL_INSERT) \
    vTrace("%s(%s): %p(%lld) inserting row %s\n"_Stz,  \
      aApiCall, aMarker, pSession, pSession ? pSession->iPartition() : -1, sRecord); \
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ERROR(aMarker, eErrorCode) \
{  \
  if ( (iTrace & TL_ERROR) == TL_ERROR) \
    vTrace(">%s(%s): %p(%lld) %s(%lld)\n"_Stz,  \
      aApiCall, aMarker, pSession, pSession ? pSession->iPartition() : -1, aEC[eErrorCode], (i8)eErrorCode); \
}

/*----------------------------------------------------------------------------*/
/* trace API exit                                                             */
/*----------------------------------------------------------------------------*/
#define TRACE_EXIT(aMarker) \
{  \
  if ( (iTrace & TL_EXIT) == TL_EXIT) \
    vTrace(">%s(%s): %p(%lld)\n"_Stz,  \
      aApiCall, aMarker, pSession, pSession ? pSession->iPartition() : -1); \
}

/*----------------------------------------------------------------------------*/
/* validate job                                                               */
/*----------------------------------------------------------------------------*/
#define VALIDATE_JOB(aMarker) \
{  \
  if ( ! aJob ) { \
    TRACE_ERROR(aMarker, EC_VALIDATION_FAILURE); \
    return EC_VALIDATION_FAILURE; \
  } \
}

/*----------------------------------------------------------------------------*/
/* validate index                                                             */
/*----------------------------------------------------------------------------*/
#define VALIDATE_INDEX(aMarker) \
{  \
  if ( ! pSession->pCurrentIndex() ) { \
    TRACE_ERROR(aMarker, EC_VALIDATION_FAILURE); \
    return EC_VALIDATION_FAILURE; \
  } \
}

/*----------------------------------------------------------------------------*/
/* validate no index                                                          */
/*----------------------------------------------------------------------------*/
#define VALIDATE_NOINDEX(aMarker) \
{  \
  if ( pSession->pCurrentIndex() ) { \
    TRACE_ERROR(aMarker, EC_VALIDATION_FAILURE); \
    return EC_VALIDATION_FAILURE; \
  } \
}

/*----------------------------------------------------------------------------*/
/* validate a parameter                                                       */
/*----------------------------------------------------------------------------*/
#define VALIDATE_PARAM(aMarker, pParameter) \
{  \
  if ( ! pParameter ) { \
    TRACE_ERROR(aMarker, EC_INVALID_PARAMETER); \
    return EC_INVALID_PARAMETER; \
   } \
}

/*----------------------------------------------------------------------------*/
/* validate reference pointer                                                 */
/*----------------------------------------------------------------------------*/
#define VALIDATE_REFPOINTER(aMarker, pRefPointer) \
{ \
  if ( ! pRefPointer) { \
    TRACE_ERROR(aMarker, EC_INVALID_PARAMETER); \
    return EC_INVALID_PARAMETER; \
  } \
}

/*----------------------------------------------------------------------------*/
/* validate SE session                                                        */
/*----------------------------------------------------------------------------*/
#define VALIDATE_SESSION(aMarker) \
{ \
  eEC eResult = eValidateSession(iTrace, pSession, aMarker); \
  if (eResult != EC_NONE) { \
      TRACE_ERROR(aMarker, EC_INVALID_INTERFACE); \
      return eResult; \
   } \
}

/*----------------------------------------------------------------------------*/
/* check result of a request that was sent                                    */
/*----------------------------------------------------------------------------*/
#define CHECK_RESULT(aMarker) \
{ \
  if (EC_NONE != eResult) { \
    TRACE_ERROR(aMarker,eResult); \
    return eResult; \
  } \
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

#define API_ENTRY(__eApiCall)
#define API_ERROR()
#define API_EXIT()

#define TRACE_ENTRY_P0()
#define TRACE_ENTRY_P1(p1)
#define TRACE_ENTRY_P2(p1, p2)
#define TRACE_ENTRY_P3(p1, p2, p3)
#define TRACE_ENTRY_P4(p1, p2, p3, p4)
#define TRACE_INSERT(aMarker, sRecord)
#define TRACE_ERROR(aMarker, eErrorCode)
#define TRACE_EXIT(aMarker)
#define VALIDATE_JOB(aMarker)
#define VALIDATE_INDEX(aMarker)
#define VALIDATE_NOINDEX(aMarker)
#define VALIDATE_PARAM(aMarker, pParameter)
#define VALIDATE_REFPOINTER(aMarker, pRefPointer)
#define VALIDATE_SESSION(aMarker)
#define CHECK_RESULT(aMarker)
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
