/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_EVENTLOG,
  /* 2 */ TI_MEMORYLOG,
  /* 3 */ TI_OPERANDLOG,
  /* 4 */ TI_PROMPTOUT,
  /* 5 */ TI_RESOURCELOG,
  /* 6 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_EVENTLOG"_Stz,
  /* 2 */ "TI_MEMORYLOG"_Stz,
  /* 3 */ "TI_OPERANDLOG"_Stz,
  /* 4 */ "TI_PROMPTOUT"_Stz,
  /* 5 */ "TI_RESOURCELOG"_Stz,
  /* 6 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
