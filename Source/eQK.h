/*============================================================================*/
/* eQK.h -- queue kinds                                                       */
/*============================================================================*/
#pragma once

enum eQK // queue kinds
{
  /*  0 */ QK_NONE=0,
  /*  1 */ QK_BIDING_SET,
  /*  2 */ QK_EXECUTING_SET,
  /*  3 */ QK_PAUSED_SET,
  /*  4 */ QK_READY_QUEUE,
  /*  5 */ QK_READY_QUEUE_LP,
  /*  6 */ QK_SLEEPING_QUEUE,
  /*  7 */ QK_WAITING_SET,
};

static Stz* aQK[]
{
  /*  0 */ "NONE"_Stz,           // QK_NONE
  /*  1 */ "BidingSet"_Stz,      // QK_BIDING_SET
  /*  2 */ "ExecutingSet"_Stz,   // QK_EXECUTING_SET
  /*  3 */ "PausedSet"_Stz,      // QK_PAUSED_SET
  /*  4 */ "ReadyQueue"_Stz,     // QK_READY_QUEUE
  /*  5 */ "ReadyQueueLP"_Stz,   // QK_READY_QUEUE_LP
  /*  6 */ "SleepingQueue"_Stz,  // QK_SLEEPING_QUEUE
  /*  7 */ "WaitingSet"_Stz,     // QK_WAITING_SET
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
