/*============================================================================*/
/* Addressing.hpp -- Functions for Manipulating addresses                     */
/*============================================================================*/
#pragma once

namespace Correctness
{
  i8 nDifference(void* aAddress0, void* aAddress1);
  
  void vCheckAddress(void* aAddress);
}


/*----------------------------------------------------------------------------*/
/*-- Template Functions ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* add bytes to given address                                                 */
/*----------------------------------------------------------------------------*/
template <class CAST>
inline static CAST aAdd(void* aAddress, i8 nBytes)
{
  char* aAddr0 = reinterpret_cast<char*>(aAddress);
  
  char* aAddr1 = aAddr0 + nBytes;

  return reinterpret_cast<CAST>(aAddr1);
}

/*----------------------------------------------------------------------------*/
/* add bytes to given address                                                 */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST ahAdd(Header* aHeader, i8 nBytes)
{
  char* aAddr0 = reinterpret_cast<char*>(aHeader);
  
  char* aAddr1 = aAddr0 + nBytes;
  
  CAST aOtherHeader = reinterpret_cast<CAST>(aAddr1);
  
  ASSERT(aOtherHeader->bValidMagic());
  
  return aOtherHeader;
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
  
  ASSERT(pObject->bValidMagic());
  
  return pOtherObject;
}

/*----------------------------------------------------------------------------*/
/* subtract bytes from given address                                          */
/*----------------------------------------------------------------------------*/
template <class CAST>
inline static CAST aSubtract(void* aAddress, i8 nBytes)
{
  char* aAddr0 = reinterpret_cast<char*>(aAddress);
  
  char* aAddr1 = aAddr0 - nBytes;

  return reinterpret_cast<CAST>(aAddr1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
