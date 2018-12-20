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
  
  /*  1 */ AC_CheckLibraryVersion,
  /*  2 */ AC_EndInsertJobs,
  /*  3 */ AC_EndJobs,
  /*  4 */ AC_ExecCmdStr,
  /*  5 */ AC_ExecuteCommand,
  /*  6 */ AC_FetchNextBuffer,
  /*  7 */ AC_GetErrorMessage,
  /*  8 */ AC_GetRow,
  /*  9 */ AC_GetRowCount,
  /* 10 */ AC_GetRowCountTableColumn,
  /* 11 */ AC_GetRowTableColumn,
  /* 12 */ AC_InsertRow,
  /* 13 */ AC_InsertRows,
  /* 14 */ AC_LoadResult,
  /* 15 */ AC_PoolOpen,
  /* 16 */ AC_ScanNextRow,
  /* 17 */ AC_SessionDelete,
  /* 18 */ AC_SessionInvalid,
  /* 19 */ AC_SessionNew,
  
  /* 19 */ AC_ENUMS
};

static Stz* aAC[]
{
  /*  0 */ "NONE"_Stz,

  /*  1 */ "RapidsSE_CheckLibraryVersion"_Stz,
  /*  2 */ "RapidsSE_EndInsertJobs"_Stz,
  /*  3 */ "RapidsSE_EndJobs"_Stz,
  /*  4 */ "RapidsSE_pExecCmdStr"_Stz,
  /*  5 */ "RapidsSE_ExecuteCommand"_Stz,
  /*  6 */ "RapidsSE_FetchNextBuffer"_Stz,
  /*  7 */ "RapidsSE_GetErrorMessage"_Stz,
  /*  8 */ "RapidsSE_GetRow"_Stz,
  /*  9 */ "RapidsSE_GetRowCount"_Stz,
  /* 10 */ "RapidsSE_GetRowCountTableColumn"_Stz,
  /* 11 */ "RapidsSE_GetRowTableColumn"_Stz,
  /* 12 */ "RapidsSE_InsertRow"_Stz,
  /* 13 */ "RapidsSE_LoadResult"_Stz,
  /* 14 */ "RapidsSE_PoolOpen"_Stz,
  /* 15 */ "RapidsSE_SessionDelete"_Stz,
  /* 16 */ "RapidsSE_SessionInvalid"_Stz,
  /* 17 */ "RapidsSE_SessionNew"_Stz
  /* 18 */ "ENUMS"_Stz};

extern "C" eEC  RapidsSE_CheckLibraryVersion(Stz* pVersion, i8 iTraceLevel);
extern "C" eEC  RapidsSE_EndInsertJobs(Session* pSession, void* pJobPtr, i8 iTraceLevel);
extern "C" eEC  RapidsSE_EndJobs(Session* pSession, i8 iTraceLevel);
extern "C" Job* RapidsSE_pExecCmdStr(Stz* aCommand, Session* pSession, bool bPostJob, bool bFromPrompt);
extern "C" eEC  RapidsSE_ExecuteCommand(Session* pSession, Stz* aCommand, Stz* sCatalogName, Stz* sSchemaName,
                                        void** ppJob, i8 iTraceLevel);
extern "C" eEC  RapidsSE_FetchNextBuffer(Session* pSession, void* pJob, void** ppFetchBuffer, i8* piExepectedRows, i8 iTraceLevel);
extern "C" eEC  RapidsSE_GetErrorMessage(Session* pSession, int iErrorCode, void** ppError, i8 iTraceLevel);
extern "C" eEC  RapidsSE_GetRow(Session* pSession, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel);
extern "C" eEC  RapidsSE_GetRowCount(Session* pSession, void* pJobPtr, i8 *pRowCount, i8 iTraceLevel);
extern "C" eEC  RapidsSE_GetRowTableColumn(Session* pSession, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel);
extern "C" eEC  RapidsSE_GetRowCountTableColumn(Session* pSession, void* pJobPtr, i8* pRowCount, i8 iTraceLevel);
extern "C" eEC  RapidsSE_InsertRow(Session* pSession, Stz* pTable, Stz* sRow, i8 iTraceLevel);
extern "C" eEC  RapidsSE_InsertRows(Session* pSession, void* pJob, Stz* sRow, i8 iTraceLevel);
extern "C" eEC  RapidsSE_LoadResult(Session* pSession, void* pJob, i8* pValidRows, i8* pInvalidRows, i8* piSkippedRows, i8 iTraceLevel);
extern "C" eEC  RapidsSE_PoolOpen(Stz* pInstanceName, void** ppInstance, i8 iTraceLevel);
extern "C" eEC  RapidsSE_ScanNextRow(Session* pSession, void* pJob, void** ppEntryBuffer, void** ppEntry, void** ppRow, i8* piExepectedRows, i8 iTraceLevel);
extern "C" eEC  RapidsSE_SessionDelete(Session* pSession, i8 iTraceLevel);
extern "C" bool RapidsSE_SessionInvalid(int eError, i8 iTraceLevel);
extern "C" eEC  RapidsSE_SessionNew(void** ppSession, Stz* sCatalogName, Stz* sSchemaName, i8 iPartition, i8 iTraceLevel);

bool RapidsSE_bSpawnJob(Job* pJob, eSS eSeed, Session* pSession);

void vApiCall(Session* pSession, eAC eApiCall);
void vApiError(Session* pSession, eEC eError);
void vApiJob(Session* pSession, Job* pJob);
void vApiLocation(Session* pSession, i8 iLocation);
void vApiReturn(Session* pSession);
void vDeleteJobs(Session* pSession);
void vSpawnJobAndWait(Session* pSession, Job* pJob);

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P0() \
  {  \
    if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
      vTrace("<%s: %p(%lld)\n"_Stz,  \
        aApiCall, pSession , pSession ? pSession->iPartition() : -1); \
  }

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P1(p1) \
  {  \
    if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
      vTrace("<%s: %p(%lld) %s\n"_Stz,  \
        aApiCall, pSession, pSession ? pSession->iPartition() : -1, p1); \
  }

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P2(p1, p2) \
{  \
  if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, \
        p1 ? p1 : "null"_Stz , p2 ? p2 : "null"_Stz); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P3(p1, p2, p3) \
{  \
  if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, \
        p1 ? p1 : "null"_Stz, p2 ? p2 : "null"_Stz , p3 ? p3 : "null"_Stz); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P4(p1, p2, p3, p4) \
{  \
  if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s %s\n"_Stz,  \
      aApiCall, pSession, pSession ? pSession->iPartition() : -1, p1, p2, p3, p4); \
}

/*----------------------------------------------------------------------------*/
/* trace API insert                                                           */
/*----------------------------------------------------------------------------*/
#define TRACE_INSERT(aMarker, sRow) \
{  \
  if ( (iTraceLevel & TL_INSERT) == TL_INSERT) \
    vTrace("%s(%s): %p(%lld) inserting row %s\n"_Stz,  \
      aApiCall, aMarker, pSession, pSession ? pSession->iPartition() : -1, sRow); \
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ERROR(aMarker, eErrorCode) \
  {  \
    if ( (iTraceLevel & TL_ERROR) == TL_ERROR) \
      vTrace(">%s(%s): %p(%lld) %s(%lld)\n"_Stz,  \
        aApiCall, aMarker, pSession, pSession ? pSession->iPartition() : -1, aEC[eErrorCode], (i8)eErrorCode); \
  }

/*----------------------------------------------------------------------------*/
/* trace API exit                                                             */
/*----------------------------------------------------------------------------*/
#define TRACE_EXIT(aMarker) \
  {  \
    if ( (iTraceLevel & TL_EXIT) == TL_EXIT) \
      vTrace(">%s(%s): %p(%lld)\n"_Stz,  \
        aApiCall, aMarker, pSession, pSession ? pSession->iPartition() : -1); \
  }

/*----------------------------------------------------------------------------*/
/* validate job                                                               */
/*----------------------------------------------------------------------------*/
#define VALIDATE_JOB(aMarker) \
  {  \
    if ( ! pJobPtr ) { \
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
    eResult = eValidateSession(pSession, aApiCall, iTraceLevel); \
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
    if ( (iTraceLevel & TL_POOL) == TL_POOL) { \
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

/*----------------------------------------------------------------------------*/
/* trace API call entry                                                       */
/*----------------------------------------------------------------------------*/
inline void vTraceEntry(i8 iTraceLevel, Stz* aFormat, ...)
{
  // trace entry ?
  if ( (iTraceLevel & TL_ENTRY) != TL_ENTRY)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* trace API call exit                                                        */
/*----------------------------------------------------------------------------*/
inline void vTraceExit(i8 iTraceLevel, Stz* aFormat, ...)
{
  // trace entry ?
  if ( (iTraceLevel & TL_EXIT) != TL_EXIT)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
inline void vTraceError(i8 iTraceLevel, Stz* aFormat, ...)
{
  // trace entry ?
  if ( (iTraceLevel & TL_ERROR) != TL_ERROR)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
inline eEC eValidateSession(Session* pSession, Stz* aApiCall, i8 iTraceLevel) {
  
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
