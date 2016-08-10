/*============================================================================*/
/* Tasking.cpp - tasking functions                                            */
/*============================================================================*/

#include "Tasking.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Tasking::vCheckAssumptions(void)
{
  ASSERT(sizeof(Queue)    < C_SIZE_SMALL_OBJECT);
  ASSERT(sizeof(Task)     < C_SIZE_MEDIUM_OBJECT);
  ASSERT(sizeof(TaskList) < C_SIZE_SMALL_OBJECT);
  ASSERT(sizeof(TaskSet)  < C_SIZE_SMALL_OBJECT);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/