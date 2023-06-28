/*============================================================================*/
/* Pds.hpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/
#pragma once

#include "SkipListNode.hpp"
#include "SkipListBlock.hpp"

namespace BASE
{
  class Pds : public Object,
              public MiniHeap_t<SkipListBlock>
  {
    static const i8 C_MIN_KEY_LEN = sizeof(i8);

  public:
    i8 m_nBlockSize; // Size of SLN + Item in bytes
    i8 m_nPadding; // Size of free space
    u2 m_uFreeListRootOffset;
    i8 m_iFreeListEntries; // Number of free entries available (Does not include dummy root node)

    u2 m_uInUseRootOffset;
    i8 m_iNumEntries[C_MAX_LVL_LEVELS];

    // Save the last usable offset for asserts

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Pds(i8 nBytes);
    ~Pds(void) noexcept(false);

    SkipListBlock* aAllotSkipListBlock(i8 iKey);

    bool bDelete(i8 iKey);
    void vDump(u2 uOffset);
    void vDumpFreeList(void);
    void vDumpInUseList(void);

    SkipListBlock* pFind(i8 iKey);

    void* pGetAddrFromOffset(u2 uOffset);
    u2 uGetOffsetFromBase(void* aAddress);

    bool bInsert(i8 iKey);

    void vMoveToFreeList(SkipListBlock* pSLSet);

    SkipListBlock* pNextSLBlock(SkipListBlock* pSLBlock , i8 iLevel);

    void vPriorVecToKey(i8 iKey, SkipListBlock* aPriorVec[]);

    bool bSetNextSLBlock(SkipListBlock* pStartBlk, SkipListBlock* pEndBlock, i8 iLevel);
    bool bSetNextSLBlock(SkipListBlock* pStartBlk, u2 uOffset, i8 iLevel);

  public:
    Footer* aFooter(void);
  };

  Pds* _pPds(void* aAddr);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
