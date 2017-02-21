/*============================================================================*/
/* Addressing.hpp -- Functions for Manipulating addresses                     */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  char* _pc(void* pAddress);
  
  void* pvAdd(void* pAddress, i8 nBytes);

  i8    iSub(void* pAddress0, void* pAddress1);
}

/*----------------------------------------------------------------------------*/
/* pAdd: add bytes to given address                                           */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST pAdd(void* pAddress, i8 nBytes)
{
  return static_cast<CAST>(pvAdd(pAddress,nBytes));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
