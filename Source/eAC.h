/*============================================================================*/
/* eAC.hpp - Api Call kind                                                    */
/*============================================================================*/
#pragma once

enum eAC // api call kind
{
  /*  0 */ AC_NONE=0,
  
  /*  1 */ AC_BeginFetch,
  /*  2 */ AC_BeginInsert,
  /*  3 */ AC_BeginSubInserter,
  /*  4 */ AC_CheckVersion,
  /*  5 */ AC_DeleteSession,
  /*  6 */ AC_EndInsert,
  /*  7 */ AC_EndJobs,
  /*  8 */ AC_EndSubInserter,
  /*  9 */ AC_ExecuteCommand,
  /* 10 */ AC_FetchNextBuffer,
  /* 11 */ AC_FetchTable,
  /* 12 */ AC_GetFirstBuffer,
  /* 13 */ AC_GetLoadResult,
  /* 14 */ AC_GetNextBuffer,
  /* 15 */ AC_GetRecordBuffer,
  /* 16 */ AC_GetRow,
  /* 17 */ AC_GetRowCount,
  /* 18 */ AC_GetTableColumnRow,
  /* 19 */ AC_GetTableColumnRowCount,
  /* 20 */ AC_InsertRow,
  /* 21 */ AC_IsInvalidSession,
  /* 22 */ AC_NewSession,
  /* 23 */ AC_NextRow,
  /* 24 */ AC_OpenPool,
  /* 25 */ AC_TranslateError,

  /* 26 */ AC_ENUMS
};

static Stz* aAC[]
{
  /*  0 */ "NONE"_Stz,

  /*  1 */ "RapidsSE_BeginFetch"_Stz,
  /*  2 */ "RapidsSE_BeginInsert"_Stz,
  /*  3 */ "RapidsSE_BeginSubInserter"_Stz,
  /*  4 */ "RapidsSE_CheckVersion"_Stz,
  /*  5 */ "RapidsSE_DeleteSession"_Stz,
  /*  6 */ "RapidsSE_EndInsert"_Stz,
  /*  7 */ "RapidsSE_EndJobs"_Stz,
  /*  8 */ "RapidsSE_EndSubInserter"_Stz,
  /*  9 */ "RapidsSE_ExecuteCommand"_Stz,
  /* 10 */ "RapidsSE_FetchNextBuffer"_Stz,
  /* 11 */ "RapidsSE_FetchTable"_Stz,
  /* 12 */ "RapidsSE_GetFirstBuffer"_Stz,
  /* 13 */ "RapidsSE_GetLoadResult"_Stz,
  /* 14 */ "RapidsSE_GetNextBuffer"_Stz,
  /* 15 */ "RapidsSE_GetRecordBuffer"_Stz,
  /* 16 */ "RapidsSE_GetRow"_Stz,
  /* 17 */ "RapidsSE_GetRowCount"_Stz,
  /* 18 */ "RapidsSE_GetTableColumnRow"_Stz,
  /* 19 */ "RapidsSE_GetTableColumnRowCount"_Stz,
  /* 20 */ "RapidsSE_InsertRow"_Stz,
  /* 21 */ "RapidsSE_IsInvalidSession"_Stz,
  /* 22 */ "RapidsSE_NewSession"_Stz,
  /* 23 */ "RapidsSE_NextRow"_Stz,
  /* 24 */ "RapidsSE_OpenPool"_Stz,

  /* 25 */ "RapidsSE_TranslateError"_Stz,

  /* 26 */ "ENUMS"_Stz
  
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
