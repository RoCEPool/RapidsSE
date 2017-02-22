/*============================================================================*/
/* Addressing.cpp -- Functions for Manipulating Addresses                     */
/*============================================================================*/

#include "Magic.h"

/*----------------------------------------------------------------------------*/
/* compute byte difference between two addresses                              */
/*----------------------------------------------------------------------------*/
i8 Platform::nSub(void* pAddress0, void* pAddress1)
{
  char* pAddr0 = reinterpret_cast<char*>(pAddress0);
  char* pAddr1 = reinterpret_cast<char*>(pAddress1);
 
  i8 nBytes = pAddr0 - pAddr1;
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
