/*============================================================================*/
/* HeapMem.hpp -- Set of Heaps in Pool                                        */
/*============================================================================*/
#pragma once

#include "System.h"

namespace Pool
{
  class HeapMem
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
    void vInit(void);
    void vMonitorPortrait(s48K& sPortrait);

    static Object* pAllocate(eCN eClass, bool bPersistent=false);
    static void vFree(Object* pObject, bool bPersistent=false);
    static void vFree_NonObject(PoolBuffer* pBuffer);
    static void vReport(void);
    static void vSearchFree(void);
    static void vValidateEnums(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
