/*============================================================================*/
/* Datums.cpp -- Datums namespace functions                                   */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Datums::vCheckAssumptions(void)
{
  RASSERT(8 == sizeof(Word));
  RASSERT(sizeof(Dtm) == sizeof(Word));
  
  RASSERT(sizeof(Bcd) == sizeof(Word));
  RASSERT(sizeof(Bol) == sizeof(Word));
  RASSERT(sizeof(Dte) == sizeof(Word));
  RASSERT(sizeof(Flt) == sizeof(Word));
  RASSERT(sizeof(Int) == sizeof(Word));
  RASSERT(sizeof(Ivl) == sizeof(Word));
  RASSERT(sizeof(Tme) == sizeof(Word));
  RASSERT(sizeof(Tsp) == sizeof(Word));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
