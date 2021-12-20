/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_EVENTLOG,
  /* 2 */ TI_JOBNETDISSECT,
  /* 3 */ TI_JOBNETWORK,
  /* 4 */ TI_JOBOPERANDS,
  /* 5 */ TI_JOBOPERATORS,
  /* 6 */ TI_JOBREPORT,
  /* 7 */ TI_MEMORYLOG,
  /* 8 */ TI_OPERANDLOG,
  /* 9 */ TI_PROMPTOUT,
  /*10 */ TI_RESOURCELOG,
  /*11 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_EVENTLOG"_Stz,
  /* 2 */ "TI_JOBNETDISSECT"_Stz,
  /* 3 */ "TI_JOBNETWORK"_Stz,
  /* 4 */ "TI_JOBOPERANDS"_Stz,
  /* 5 */ "TI_JOBOPERATORS"_Stz,
  /* 6 */ "TI_JOBREPORT"_Stz,
  /* 7 */ "TI_MEMORYLOG"_Stz,
  /* 8 */ "TI_OPERANDLOG"_Stz,
  /* 9 */ "TI_PROMPTOUT"_Stz,
  /*10 */ "TI_RESOURCELOG"_Stz,
  /*11 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
