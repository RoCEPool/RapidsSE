/*============================================================================*/
/* Addressing.cpp -- functions for manipulating addresses                     */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* add bytes to address                                                       */
/*----------------------------------------------------------------------------*/
char* Types::pcAdd(void* pAddress, i8 nBytes)
{
  char* pAddr0 = reinterpret_cast<char*>(pAddress);
  
  char* pAddr1 = pAddr0 + nBytes;
  
  return pAddr1;
}

/*----------------------------------------------------------------------------*/
/* add bytes to address                                                       */
/*----------------------------------------------------------------------------*/
void* Types::pvAdd(void* pAddress, i8 nBytes)
{
  char* pAddr0 = reinterpret_cast<char*>(pAddress);
  
  char* pAddr1 = pAddr0 + nBytes;
  
  return pAddr1;
}
  
/*----------------------------------------------------------------------------*/
/* compute difference between two addresses                                   */
/*----------------------------------------------------------------------------*/
i8 Types::iSub(void* pAddress0, void* pAddress1)
{
  char* pAddr0 = reinterpret_cast<char*>(pAddress0);
  char* pAddr1 = reinterpret_cast<char*>(pAddress1);
 
  i8 nBytes = pAddr0 - pAddr1;
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/