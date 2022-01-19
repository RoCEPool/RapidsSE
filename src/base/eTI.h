/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_DEREF,
  /* 2 */ TI_EVENTLOG,
  /* 3 */ TI_JOBMETRICS,
  /* 4 */ TI_JOBNETDISSECT,
  /* 5 */ TI_JOBNETWORK,
  /* 6 */ TI_JOBOPERANDS,
  /* 7 */ TI_JOBOPERATORS,
  /* 8 */ TI_JOBREPORT,
  /* 9 */ TI_MEMORYLOG,
  /*10 */ TI_OPERANDLOG,
  /*11 */ TI_PROMPTOUT,
  /*12 */ TI_RESOURCELOG,
  /*13 */ TI_SYSLOCKS,
  /*14 */ TI_SYSMETRICS,
  /*15 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_DEREF"_Stz,
  /* 2 */ "TI_EVENTLOG"_Stz,
  /* 3 */ "TI_JOBMETRICS"_Stz,
  /* 4 */ "TI_JOBNETDISSECT"_Stz,
  /* 5 */ "TI_JOBNETWORK"_Stz,
  /* 6 */ "TI_JOBOPERANDS"_Stz,
  /* 7 */ "TI_JOBOPERATORS"_Stz,
  /* 8 */ "TI_JOBREPORT"_Stz,
  /* 9 */ "TI_MEMORYLOG"_Stz,
  /*10 */ "TI_OPERANDLOG"_Stz,
  /*11 */ "TI_PROMPTOUT"_Stz,
  /*12 */ "TI_RESOURCELOG"_Stz,
  /*13 */ "TI_SYSLOCKS"_Stz,
  /*14 */ "TI_SYSMETRICS"_Stz,
  /*15 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
