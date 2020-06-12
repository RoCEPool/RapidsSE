/*============================================================================*/
/* CompactBuffers.cpp - CompactBuffers namespace functions                    */
/*============================================================================*/

#include "SortBuffers.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void CompactBuffers::vCheckAssumptions(void)
{
  RASSERT(sizeof(HeadLine) == sizeof(SingleWord));
  RASSERT(sizeof(TailLine) == sizeof(SpaceUnit));

  RASSERT(sizeof(Lrw) == 16);
  RASSERT(sizeof(LrwCdx) == 8);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
