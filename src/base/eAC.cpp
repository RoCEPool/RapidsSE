/*============================================================================*/
/* eAC.cpp - Api Call kind                                                    */
/*============================================================================*/
#include "BASE.h"
#include "eAC.h"

Stz* aAC[AC_ENUMS]
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

//  /* 26 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
