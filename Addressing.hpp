/*============================================================================*/
/* Addressing.hpp -- Functions for Manipulating addresses                     */
/*============================================================================*/
#pragma once

namespace Base
{
  i8 nSub(void* aAddress0, void* aAddress1);
  
  void vCheckAddress(void* aAddress);
}

/*----------------------------------------------------------------------------*/
/* add bytes to given address                                                 */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST aAdd(void* aAddress, i8 nBytes)
{
  char* aAddr0 = reinterpret_cast<char*>(aAddress);
  
  char* aAddr1 = aAddr0 + nBytes;

  return reinterpret_cast<CAST>(aAddr1);
}

/*----------------------------------------------------------------------------*/
/* add bytes to given address                                                 */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST pAdd(Object* pObject, i8 nBytes)
{
  char* aAddr0 = reinterpret_cast<char*>(pObject);
  
  char* aAddr1 = aAddr0 + nBytes;
  
  CAST pOtherObject = reinterpret_cast<CAST>(aAddr1);
  
  return pOtherObject;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
