/*============================================================================*/
/* Alignment.cpp -- Alignment namespace functions                             */
/*============================================================================*/

#include "Alignment.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Alignment::vCheckAssumptions(void)
{
  ALWAYS(sizeof(SingleWord) == 8);
  ALWAYS(sizeof(SingleWord) == C_BYTES_PER_WORD);
  ALWAYS(sizeof(DoubleWord) == 2 * C_BYTES_PER_WORD);
  ALWAYS(sizeof(QuadWord)   == 4 * C_BYTES_PER_WORD);
  ALWAYS(sizeof(CacheLine)  == C_BYTES_PER_CACHE_LINE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
