/*============================================================================*/
/* Alignment.cpp -- Alignment namespace functions                                     */
/*============================================================================*/

#include "Alignment.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Alignment::vCheckAssumptions(void)
{
  RASSERT(sizeof(Word)       == 8);
  RASSERT(sizeof(Word)       == C_BYTES_PER_WORD);
  RASSERT(sizeof(DoubleWord) == 2 * C_BYTES_PER_WORD);
  RASSERT(sizeof(QuadWord)   == 4 * C_BYTES_PER_WORD);
  RASSERT(sizeof(CacheLine)  == C_BYTES_PER_CACHE_LINE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
