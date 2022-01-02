/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_EVENTLOG,
  /* 2 */ TI_JOBMETRICS,
  /* 3 */ TI_JOBNETDISSECT,
  /* 4 */ TI_JOBNETWORK,
  /* 5 */ TI_JOBOPERANDS,
  /* 6 */ TI_JOBOPERATORS,
  /* 7 */ TI_JOBREPORT,
  /* 8 */ TI_MEMORYLOG,
  /* 9 */ TI_OPERANDLOG,
  /*10 */ TI_PROMPTOUT,
  /*11 */ TI_RESOURCELOG,
  /*12 */ TI_ENUMS
};

static Stz* aTI[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "TI_EVENTLOG"_Stz,
  /* 2 */ "TI_JOBMETRICS"_Stz,
  /* 3 */ "TI_JOBNETDISSECT"_Stz,
  /* 4 */ "TI_JOBNETWORK"_Stz,
  /* 5 */ "TI_JOBOPERANDS"_Stz,
  /* 6 */ "TI_JOBOPERATORS"_Stz,
  /* 7 */ "TI_JOBREPORT"_Stz,
  /* 8 */ "TI_MEMORYLOG"_Stz,
  /* 9 */ "TI_OPERANDLOG"_Stz,
  /*10 */ "TI_PROMPTOUT"_Stz,
  /*11 */ "TI_RESOURCELOG"_Stz,
  /*12 */ "TI_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
