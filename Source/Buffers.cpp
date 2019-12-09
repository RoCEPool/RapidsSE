/*============================================================================*/
/* Buffers.cpp - Buffers namespace functions                                  */
/*============================================================================*/

#include "Buffers.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Buffers::vCheckAssumptions(void)
{
  RASSERT(sizeof(HeadLine) == sizeof(SingleWord));
  RASSERT(sizeof(TailLine) == sizeof(SpaceUnit));
  RASSERT(sizeof(FreeItem) <= sizeof(Ent));
  RASSERT(sizeof(NEO)      == C_NEO_SIZE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
