/*============================================================================*/
/* Api.h - Engine Api                                                       */
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
   static eEC  eValidateSession(i8 iTrace, Session* pSession, Stz* aApiCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
