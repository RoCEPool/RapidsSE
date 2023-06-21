/*============================================================================*/
/* Pds.hpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/
#pragma once

#include "SkipListNode.hpp"
#include "SkipListSet.hpp"

namespace BASE
{
  class Pds : public Object,
              public MiniHeap_t<SkipListSet>
  {
    static const i8 C_MIN_KEY_LEN = sizeof(i8);

  public:
    i8 m_nBlockSize; // Size of SLN + Item in bytes
    u2 m_uFreeListRootOffset;
    i8 m_iFreeListEntries;

    u2 m_uInUseRootOffset;
    i8 m_iNumEntries[C_MAX_LVL_LEVELS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Pds(i8 nBytes);
    ~Pds(void) noexcept(false);

    SkipListSet* aAllotSkipListSet(i8 iKey);

    void vDelete(i8 iKey);
    void vDump(u2 uOffset);
    void vDumpFree(void);
    void vDumpInUse(void);

    SkipListSet* pFind(i8 iKey);
    void* pGetAddrFromOffset(u2 uOffset);
    u2 uGetOffsetFromBase(void* aAddress);

    void vInsert(i8 iKey);

    void vMoveToFreeList(SkipListSet* pSLSet);
    SkipListSet* pNextSLSet(SkipListSet* pSLSet ,i8 iLevel);

    void vPriorVecToKey(i8 iKey, SkipListSet* aPriorVec[]);

  public:
    Footer* aFooter(void);
  };

  Pds* _pPds(void* aAddr);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
