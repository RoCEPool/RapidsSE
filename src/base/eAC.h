/*============================================================================*/
/* eAC.h - Api Call kind                                                      */
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

extern Stz* aAC[];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
