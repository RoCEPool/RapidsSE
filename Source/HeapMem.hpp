/*============================================================================*/
/* HeapMem.hpp -- Set of Heaps in Pool                                        */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Heap;
  
  class HeapMem
  {
  public:
    struct {
      Heap* m_pLarge;
      Heap* m_pMedium;
      Heap* m_pSmall;
      Heap* m_pTiny;
    } Transient;
  
    struct {
      Heap* m_pLarge;
      Heap* m_pMedium;
      Heap* m_pSmall;
      Heap* m_pTiny;
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
