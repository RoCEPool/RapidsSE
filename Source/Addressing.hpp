/*============================================================================*/
/* Addressing.hpp -- functions for manipulating addresses                     */
/*============================================================================*/
#pragma once

#include "Types.h"

namespace Types
{
  char* pcAdd(void* pAddress, i8 nBytes);
  void* pvAdd(void* pAddress, i8 nBytes);

  i8    iSub(void* pAddress0, void* pAddress1);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
