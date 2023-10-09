/*============================================================================*/
/* eTS.cpp -- Task State                                                      */
/*============================================================================*/
#include "BASE.h"
#include "eTS.h"
  
Stz* aTS[TS_ENUMS]
{
  /* 0 */ "TS_NONE"_Stz,
  /* 1 */ "TS_BIDING_OPERAND"_Stz,
  /* 2 */ "TS_BIDING_PAGEIN"_Stz,
  /* 3 */ "TS_COMPLETED"_Stz,
  /* 4 */ "TS_DELETED"_Stz,
  /* 5 */ "TS_DESISTED"_Stz,
  /* 6 */ "TS_EXECUTING"_Stz,
  /* 7 */ "TS_HIBERNATING"_Stz,
  /* 8 */ "TS_READY"_Stz,
  /* 9 */ "TS_REAPABLE"_Stz,
  /*10 */ "TS_RETIRED"_Stz,
  /*11 */ "TS_SLEEPING"_Stz,
  /*12 */ "TS_WAITING"_Stz,
//  /*13 */ "TS_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
