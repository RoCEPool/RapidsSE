/*============================================================================*/
/* HeapMem.hpp -- Set of Heaps in Pool                                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Heap;
  
  class HeapMem : public Object
  {
  public:
  
    struct {
      Heap* m_pTiny;
      Heap* m_pSmall;
      Heap* m_pMedium;
      Heap* m_pLarge;
    } Transient;
  
    struct {
      Heap* m_pTiny;
      Heap* m_pSmall;
      Heap* m_pMedium;
      Heap* m_pLarge;
    } Virtual;
  
    struct {
      Heap* m_pTiny;
      Heap* m_pSmall;
      Heap* m_pMedium;
      Heap* m_pLarge;
    } Persistent;

  public:
    HeapMem(void);
  
    static void vSubSystem(void* aSubSysArg);


    static Object* pAllocate(eCN eClass, bool bPersistent=false);
    static void vFree(Object* pObject, bool bPersistent=false);
    static void vFree_NonObject(PoolBuffer* pBuffer);
    static void vSummaryReport(void);
    static void vSearchFree(void);
  
    Heap* pEachHeap(eHT& reLastHeap);
    Heap* pGetHeap(eHT eHeap);
    void vInit(void);
    void vMonitorPortrait(s48K& sPortrait);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
