/*============================================================================*/
/* CompactBuffers.cpp - CompactBuffers namespace functions                    */
/*============================================================================*/

#include "SortBuffers.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void CompactBuffers::vCheckAssumptions(void)
{
  ALWAYS(sizeof(HeadLine) == sizeof(SingleWord));
  ALWAYS(sizeof(TailLine) == sizeof(SpaceUnit));

  ALWAYS(sizeof(Lrw) == 16);
  ALWAYS(sizeof(LrwCdx) == 8);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
