/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_EVENTLOG,
  /* 2 */ TI_JOBOPERANDS,
  /* 3 */ TI_JOBOPERATORS,
  /* 4 */ TI_JOBREPORT,
  /* 5 */ TI_MEMORYLOG,
  /* 6 */ TI_OPERANDLOG,
  /* 7 */ TI_PROMPTOUT,
  /* 8 */ TI_RESOURCELOG,
  /* 9 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_EVENTLOG"_Stz,
  /* 2 */ "TI_JOBOPERANDS"_Stz,
  /* 3 */ "TI_JOBOPERATORS"_Stz,
  /* 4 */ "TI_JOBREPORT"_Stz,
  /* 5 */ "TI_MEMORYLOG"_Stz,
  /* 6 */ "TI_OPERANDLOG"_Stz,
  /* 7 */ "TI_PROMPTOUT"_Stz,
  /* 8 */ "TI_RESOURCELOG"_Stz,
  /* 9 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
