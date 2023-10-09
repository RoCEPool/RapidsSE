/*============================================================================*/
/* eQK.h -- Queue Kind                                                        */
/*============================================================================*/
#pragma once

enum eQK // queue kind
{
  /*  0 */ QK_NONE=0,
  /*  1 */ QK_BIDING_OPERAND_SET,
  /*  2 */ QK_BIDING_PAGEIN_SET,
  /*  3 */ QK_EXECUTING_SET,
  /*  4 */ QK_READY_QUEUE,
  /*  5 */ QK_READY_QUEUE_LP,
  /*  6 */ QK_RETIRED_SET,
  /*  7 */ QK_SLEEPING_QUEUE,
  /*  8 */ QK_WAITING_SET,
  /*  9 */ QK_ENUMS
};

extern Stz* aQK[QK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
