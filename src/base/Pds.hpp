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
  public:
    i8 m_nItemSize; // Size of item in bytes
    u2 m_uFreeListRootOffset;

    u2 m_uInUseRootOffset;
    i8 m_iNumEntries[C_MAX_LVL_LEVELS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Pds(void);
    ~Pds(void) noexcept(false);

    SkipListSet* aAllotSkipListSet(i16 iKey, i8 nSize);

    void vDelete(i16 iKey);
    void vDump(u2 uOffset);

    SkipListSet* pFind(i16 iKey);
    void* pGetAddrFromOffset(u2 uOffset);
    u2 uGetOffsetFromBase(void* aAddress);

    void vInsert(i16 iKey);

    void vMoveToFreeList(SkipListSet* pSLSet);
    SkipListSet* pNextSLSet(SkipListSet* pSLSet ,i8 iLevel);

    void vPriorVecToKey(i8 iKey, SkipListSet* aPriorVec[]);

    // Add search()
    //
  public:
    Footer* aFooter(void);
  };

  Pds* _pPds(void* aAddr);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
