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
      Heap* m_pJumbo;
      Heap* m_pLarge;
      Heap* m_pMedium;
      Heap* m_pSmall;
    } Transient;
  
    struct {
      Heap* m_pJumbo;
      Heap* m_pMedium;
      Heap* m_pLarge;
      Heap* m_pSmall;
    } Persistent;

  public:
    static Object* pAllocate(eCN eClass, bool bPersistent=false);
    static void vFree(Object* pObject, bool bPersistent=false);
    static void vFree_NonObject(Buffer* pBuffer);

    void vInit(void);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
