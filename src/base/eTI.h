/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_EVENTLOG,
  /* 2 */ TI_JOBOPERATORS,
  /* 3 */ TI_JOBREPORT,
  /* 4 */ TI_MEMORYLOG,
  /* 5 */ TI_OPERANDLOG,
  /* 6 */ TI_PROMPTOUT,
  /* 7 */ TI_RESOURCELOG,
  /* 8 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_EVENTLOG"_Stz,
  /* 2 */ "TI_JOBOPERATORS"_Stz,
  /* 3 */ "TI_JOBREPORT"_Stz,
  /* 4 */ "TI_MEMORYLOG"_Stz,
  /* 5 */ "TI_OPERANDLOG"_Stz,
  /* 6 */ "TI_PROMPTOUT"_Stz,
  /* 7 */ "TI_RESOURCELOG"_Stz,
  /* 8 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
