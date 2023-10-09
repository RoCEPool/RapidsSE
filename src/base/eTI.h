/*============================================================================*/
/* eTI.h -- Internal Table                                                    */
/*============================================================================*/
#pragma once

enum eTI // Internal Table
{
  /* 0 */ TI_NONE=0,
  /* 1 */ TI_DEREFMETRICS,
  /* 2 */ TI_EVENTLOG,
  /* 3 */ TI_JOBMETRICS,
  /* 4 */ TI_JOBNETDISSECT,
  /* 5 */ TI_JOBNETWORK,
  /* 6 */ TI_JOBOPERANDS,
  /* 7 */ TI_JOBOPERATORS,
  /* 8 */ TI_JOBREPORT,
  /* 9 */ TI_MEMORYLOG,
  /*10 */ TI_OPERANDLOG,
  /*11 */ TI_PAGEINREQUEST,
  /*11 */ TI_PROMPTOUT,
  /*12 */ TI_RESOURCELOG,
  /*13 */ TI_SYSLOCKS,
  /*14 */ TI_SYSMETRICS,
  /*15 */ TI_ENUMS
};

extern Stz* aTI[TI_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
