/*============================================================================*/
/* Heaps.hpp -- Set of Heaps in Pool                                          */
/*============================================================================*/
#pragma once

#include "System.h"

namespace Pool
{
  class Heaps
  {
  public:
    Heap*  m_pTransientJumbo;
    Heap*  m_pTransientLarge;
    Heap*  m_pTransientMedium;
    Heap*  m_pTransientSmall;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
