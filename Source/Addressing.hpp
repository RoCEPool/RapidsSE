/*============================================================================*/
/* Addressing.hpp -- Functions for Manipulating addresses                     */
/*============================================================================*/
#pragma once

#include "Magic.h"

namespace Magic
{
  char* _pc(void* pAddress);
  
  char* pcAdd(void* pAddress, i8 nBytes);
  void* pvAdd(void* pAddress, i8 nBytes);

  i8    iSub(void* pAddress0, void* pAddress1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
