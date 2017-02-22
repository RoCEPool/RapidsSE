/*============================================================================*/
/* Addressing.hpp -- Functions for Manipulating addresses                     */
/*============================================================================*/
#pragma once

#include "Platform.h"

namespace Platform
{
  i8 nSub(void* pAddress0, void* pAddress1);
}

/*----------------------------------------------------------------------------*/
/* pAdd: add bytes to given address                                           */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST pAdd(void* pAddress, i8 nBytes)
{
  char* pAddr0 = reinterpret_cast<char*>(pAddress);
  
  char* pAddr1 = pAddr0 + nBytes;

  return reinterpret_cast<CAST>(pAddr1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
