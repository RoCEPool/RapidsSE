/*============================================================================*/
/* Addressing.cpp -- Functions for Manipulating Addresses                     */
/*============================================================================*/

#include "Magic.h"

/*----------------------------------------------------------------------------*/
/* address as a char*                                                         */
/*----------------------------------------------------------------------------*/
char* Magic::_pc(void* pVoidAddress)
{
  char* pCharAddress = reinterpret_cast<char*>(pVoidAddress);
  
  return pCharAddress;
}

/*----------------------------------------------------------------------------*/
/* compute difference between two addresses                                   */
/*----------------------------------------------------------------------------*/
i8 Magic::iSub(void* pAddress0, void* pAddress1)
{
  char* pAddr0 = reinterpret_cast<char*>(pAddress0);
  char* pAddr1 = reinterpret_cast<char*>(pAddress1);
 
  i8 nBytes = pAddr0 - pAddr1;
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
