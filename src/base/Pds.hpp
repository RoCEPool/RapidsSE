/*============================================================================*/
/* Pds.hpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/
#pragma once

#include "SkipListNode.hpp"
#include "SkipListBlock.hpp"

namespace BASE
{
  typedef i8 (*CompareFunc)(void* pKey_l, void* pKey_r);

  class Pds : public Object,
              public InternalHeap_t<SkipListBlock>
  {
    static const i8 C_MIN_KEY_LEN = sizeof(i8);

  public:
    i8 m_nBlockSize; // Size of SLN + Item in bytes
    i8 m_nPaddingPerBlk; // Size of padding for each Skip List Block
    u2 m_uFreeListRootOffset;
    i8 m_iFreeListEntries; // Number of free entries available (Does not include dummy root node)

    CompareFunc m_fpCompare;  // compare function

    u2 m_uInUseRootOffset;
    i8 m_iNumEntries[C_MAX_LVL_LEVELS];

//    i8 m_iSyntheticKey; // Synthetic key used when we only append to the Skip List

    // Save the last usable offset for asserts

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Pds(i8 nBytes, CompareFunc fpCompare);
    ~Pds(void) noexcept(false);



    // Note: Only the public functions grab a lock
    // public:
//    SkipListBlock*  pAppendWithSynthKey(void);
    bool            bDelete(void* pKey);
    void            vDumpFreeList(void);
    void            vDumpInUseList(void);
    SkipListBlock*  pInsert(void* pKey);
    SkipListBlock*  pFind(void* pKey);
    SkipListBlock*  pFindLessThan(void* pKey);
    SkipListBlock*  pFindGreaterThan(void* pKey);
    SkipListBlock*  pFirstBlock(void);
    i8              iNumEntries(i8 iLevel);
    void            vValidate(void);
    bool            bVerify(void);

    // private:
    SkipListBlock*  aAllotSkipListBlock(void);
    void*           pGetAddrFromOffset(u2 uOffset);
    u2              uGetOffsetFromBase(void* aAddress);
    void            vMoveToFreeList(SkipListBlock* pSLBlock);
    SkipListBlock*  pNextSLBlock(SkipListBlock* pSLBlock , i8 iLevel);
    bool            bPriorVecToKey(void* pKey, SkipListBlock* aPriorVec[]);
    bool            bSetNextSLBlock(SkipListBlock* pStartBlk, SkipListBlock* pEndBlock, i8 iLevel);
    bool            bSetNextSLBlock(SkipListBlock* pStartBlk, u2 uOffset, i8 iLevel);

    static i8     iIntCompare(void* aItem_l, void* aItem_r);

  public:
    Footer* aFooter(void);
  };

  Pds* _pPds(void* aAddr);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
