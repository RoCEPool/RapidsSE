/*============================================================================*/
/* eQK.cpp -- Queue Kind                                                      */
/*============================================================================*/
#include "BASE.h"
#include "eQK.h"

Stz* aQK[QK_ENUMS]
{
  /*  0 */ "QK_NONE"_Stz,
  /*  1 */ "QK_BIDING_OPERAND_SET"_Stz,
  /*  2 */ "QK_BIDING_PAGEIN_SET"_Stz,
  /*  3 */ "QK_EXECUTING_SET"_Stz,
  /*  4 */ "QK_READY_QUEUE"_Stz,
  /*  5 */ "QK_READY_QUEUE_LP"_Stz,
  /*  6 */ "QK_RETIRED_SET"_Stz,
  /*  7 */ "QK_SLEEPING_QUEUE"_Stz,
  /*  8 */ "QK_WAITING_SET"_Stz,
//  /*  9 */ "QK_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
