/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_EVENTS,
  /* 2 */ TI_MEMORYLOG,
  /* 3 */ TI_OPERANDLOG,
  /* 4 */ TI_RESOURCELOG,
  /* 5 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_EVENTS"_Stz,
  /* 2 */ "TI_MEMORYLOG"_Stz,
  /* 3 */ "TI_OPERANDLOG"_Stz,
  /* 4 */ "TI_RESOURCELOG"_Stz,
  /* 5 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
