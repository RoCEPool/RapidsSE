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
    struct {
      Heap*  m_pJumbo;
      Heap*  m_pLarge;
      Heap*  m_pMedium;
      Heap*  m_pSmall;
    } Transient;
  
  public:
    void vInit(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
