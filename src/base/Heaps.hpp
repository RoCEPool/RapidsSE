/*============================================================================*/
/* Heaps.hpp -- Set of Heaps in Pool                                          */
/*============================================================================*/
#pragma once

#include "eHT.h"


namespace BASE
{
  class Heap;
  
  class Heaps : public Object
  {
  public:
  
    i8 m_bNoFree;

    b8 m_bTraceAlloc;
    b8 m_bTraceFree;

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

    // must be last
    Footer m_xFooter;

  public:
    Heaps(i8 bNoFree);
  
    static void vSubSystem(TokenVec* pTokenVec);


    static Object* pAllocate(eCN eClass, bool bPersistent=false);
    static void vDumpHeaps(void);
    static void vFree(Object* pObject, bool bPersistent=false);
    static void vFree_NonObject(PoolBuffer* pBuffer);
    static void vSummaryReport(void);
    static void vSearchFree(void);

    Heap* pEachHeap(eHT& reLastHeap);
    Heap* pGetHeap(eHT eHeap);
    void vInit(void);
    void vSetTraceAlloc(bool bTraceAlloc);
    void vSetTraceFree(bool bTraceFree);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
