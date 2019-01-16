/*============================================================================*/
/* Tasks.h -- Tasks namespace                                                 */
/*============================================================================*/
#pragma once

#include "Steps.h"

enum eQR // queue round robin
{
  /* -5 */ QR_NONE=-5,
  /* -4 */ QR_EXECUTING_QUEUE,
  /* -3 */ QR_WAITING_QUEUE,
  /* -2 */ QR_SLEEPING_QUEUE,
  /* -1 */ QR_READY_QUEUE,
};

enum eQK // queue kinds
{
  /*  0 */ QK_NONE=0,
  /*  1 */ QK_EXECUTING_SET,
  /*  2 */ QK_READY_QUEUE,
  /*  3 */ QK_READY_SLOT_QUEUE,
  /*  4 */ QK_READY_SEND_QUEUE,
  /*  5 */ QK_SLEEPING_QUEUE,
  /*  6 */ QK_WAITING_SET,
};

static Stz* aQK[]
{
  /*   0 */ "NONE"_Stz,           // QK_NONE
  /*   1 */ "ExecutingSet"_Stz,   // QK_EXECUTING_SET
  /*   2 */ "ReadyQueue"_Stz,     // QK_READY_QUEUE
  /*   3 */ "ReadySlotQueue"_Stz, // QK_READY_SLOT_QUEUE
  /*   4 */ "ReadySendQueue"_Stz, // QK_READY_SEND_QUEUE
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

#include "Compiler.hpp"

#include "Socket.hpp"

#include "Task.hpp"
#include "TaskMacros.hpp"
#include "TaskList.hpp"
#include "TaskQueue.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
