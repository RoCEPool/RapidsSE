/*============================================================================*/
/* Api.h - Engine Api                                                       */
/*============================================================================*/
#pragma once

enum eAC // api call kind
{
  /*  0 */ AC_NONE=0,
  
  /*    */ AC_BeginFetch,
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
  /*    */ AC_NextRow,
  /* 20 */ AC_OpenPool,
  /*    */ AC_TranslateError,

  /* 23 */ AC_ENUMS
};

static Stz* aAC[]
{
  /*  0 */ "NONE"_Stz,

  /*    */ "RapidsSE_BeginFetch"_Stz,
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
  /*    */ "RapidsSE_NextRow"_Stz,
  /* 20 */ "RapidsSE_OpenPool"_Stz,
 
  /*    */ "RapidsSE_TranslateError"_Stz,

  /* 23 */ "ENUMS"_Stz};

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
