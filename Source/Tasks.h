/*============================================================================*/
/* Tasks.h -- Tasks namespace                                                 */
/*============================================================================*/
#pragma once

#include "Steps.h"

enum eQK // queue kinds
{
  /*  0 */ QK_NONE=0,
  /*  1 */ QK_BIDING_SET,
  /*  2 */ QK_EXECUTING_SET,
  /*  3 */ QK_READY_QUEUE,
  /*  4 */ QK_READY_QUEUE_LP,
  /*  5 */ QK_SLEEPING_QUEUE,
  /*  6 */ QK_WAITING_SET,
};

static Stz* aQK[]
{
  /*   0 */ "NONE"_Stz,           // QK_NONE
  /*   1 */ "BidingSet"_Stz,      // QK_BIDING_SET
  /*   2 */ "ExecutingSet"_Stz,   // QK_EXECUTING_SET
  /*   3 */ "ReadyQueue"_Stz,     // QK_READY_QUEUE
  /*   4 */ "ReadyQueueLP"_Stz,   // QK_READY_QUEUE_LP
  /*   5 */ "SleepingQueue"_Stz,  // QK_SLEEPING_QUEUE
  /*   6 */ "WaitingSet"_Stz,     // QK_WAITING_SET
};

//------------------------------------------------------------------------------

namespace Tasks
{
  void vCheckAssumptions(void);
}

using namespace Tasks;

//------------------------------------------------------------------------------


#include "Task.hpp"
#include "TaskMacros.hpp"
#include "TaskList.hpp"
#include "TaskQueue.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
