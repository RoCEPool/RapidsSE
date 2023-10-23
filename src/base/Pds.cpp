/*============================================================================*/
/* Pds.cpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/

#include <climits>
#include "BASE.h"

#include "Pds.hpp"
#include "MiniHeap_t.hpp"
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Pds* BASE::_pPds(void* aAddress)
{
  Pds* pPds = reinterpret_cast<Pds*>(aAddress);

  RASSERT_TYPE(pPds,CN_PDS);

  return pPds;
}

/*----------------------------------------------------------------------------*/
/* allocate a new pds buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* Pds::operator new(size_t)
{
  Pds* pPds =
      static_cast<Pds*>(Heaps::pAllocate(CN_PDS));

  return pPds;
}

/*----------------------------------------------------------------------------*/
/* free a pds buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void Pds::operator delete(void* aAddress)
{
  Pds* pPds = static_cast<Pds*>(aAddress);

  Heaps::vFree(pPds);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pds::Pds(i8 nBytes, CompareFunc fpCompare)
    : Object(CN_PDS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(CacheLine::bOnCacheLineBoundary(this));

  // Assert that there is enough bytes for the default key (i8)
  RASSERT(nBytes >= C_MIN_KEY_LEN);

  m_nBlockSize = sizeof(SkipListNode) + nBytes;
  m_nPaddingPerBlk = 0;
//  m_iSyntheticKey = 0;

  // Ensure block size is a multiple of 16 bytes
  i8 iMod = m_nBlockSize % 16;
  if(iMod != 0) {
    m_nPaddingPerBlk = 16 - iMod;
    m_nBlockSize += m_nPaddingPerBlk;
  }

  if(fpCompare) {
    m_fpCompare = fpCompare;
  } else {
    m_fpCompare = iIntCompare;
  }

  m_iFreeListEntries = 0;

  // Add dummy root nodes to the Free and In Use skip lists
  // Note: Dummy root blocks have garbage values in their Item section
  // Could this cause a problem?
  m_uFreeListRootOffset = uGetOffsetFromBase((void*)aAllotSkipListBlock());
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aAllotSkipListBlock());

  // Reset the number of entries in both the lists
  m_iFreeListEntries = 0;
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
  }

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pds::~Pds(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove key from in use skip list and move to free list                     */
/*----------------------------------------------------------------------------*/
bool Pds::bDelete(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);


  // TODO: Add an assert to check that all the entries are deleted where
  // the entry to delete has a non-zero value in it's respective level.
  SkipListBlock* pDeleteEntry = nullptr;

  // Iterate the Skip List from the top down
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    // Get the dummy root at this level
    SkipListBlock* pPrev = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);
    SkipListBlock* pCurr = pNextSLBlock(pPrev, i_Level);

    // Iterate through all elements at this level while searching for the key
    while(pCurr != nullptr) {
      i8 iCompareRes = m_fpCompare(pCurr->aItem(), pKey);

      if(iCompareRes == 0) {
        // Found the key, save its address and update the chain pointers/offsets
        pDeleteEntry = pCurr;

        SkipListBlock* pNextBlock = pNextSLBlock(pCurr, i_Level);
        bool bResult = bSetNextSLBlock(pPrev, pNextBlock, i_Level);
        RASSERT(bResult == 1);

        m_iNumEntries[i_Level]--;
        RASSERT(m_iNumEntries[i_Level] >= 0);

        // No need to continue iterating at this level
        break;
      } else if(iCompareRes > 0)
        // We passed the key, move to a lower level
        break;
      pPrev = pCurr;
      pCurr = pNextSLBlock(pCurr, i_Level);
    }
  }

  if(pDeleteEntry){
    // Assert we are not deleting the root block
    u8 uOffsetToDelete = uGetOffsetFromBase(pDeleteEntry);
    RASSERT(uOffsetToDelete != m_uInUseRootOffset);

    vMoveToFreeList(pDeleteEntry);
//    vValidate();
    return true;
  }

//  vValidate();
  return false;
}

/*----------------------------------------------------------------------------*/
/* print the skip list of free entries                                        */
/*----------------------------------------------------------------------------*/
void Pds::vDumpFreeList(void) {
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  vTrace("Free List\n"_Stz);
  vTrace("=================================================="
         "===================================================\n"_Stz);
  if((SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset) == nullptr) {
    vTrace("Invalid offset given\n"_Stz);
    return;
  }

  vTrace("| %5s | %6s | %s\n"_Stz, "Level", "# Ents", "Items");

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);

  SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset);

  vTrace("| %5lld | %6lld | "_Stz, 0, m_iFreeListEntries);

  for(i8 iNumEntries = 0; iNumEntries < m_iFreeListEntries + 1; iNumEntries++) {
    vTrace("%lld-->"_Stz, pCursor->iKey());

    pCursor = pNextSLBlock(pCursor, 0);
    if(pCursor == nullptr) {
      break;
    }
  }
  vTrace("END\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);


  vTrace("=================================================="
         "===================================================\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print the skip list of in use entries                                      */
/*----------------------------------------------------------------------------*/
void Pds::vDumpInUseList(void) {
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

//  struct xTest {
//    i8 iKey_1;
//    i8 iKey_2;
//  }typedef DoubleKey;

  vTrace("In Use List\n"_Stz);
  vTrace("=================================================="
         "===================================================\n"_Stz);
  if((SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset) == nullptr) {
    vTrace("Invalid offset given\n"_Stz);
    return;
  }

  vTrace("| %5s | %6s | %s\n"_Stz, "Level", "# Ents", "Items");

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);

    vTrace("| %5lld | %6lld | "_Stz, i_Level, m_iNumEntries[i_Level]);

    for(i8 iNumEntries = 0; iNumEntries < m_iNumEntries[i_Level] + 1; iNumEntries++) {
//      DoubleKey oDKey;
//      oDKey = *(DoubleKey*)(pCursor->aItemKey());
//      vTrace("%lld(%lld)-->"_Stz, oDKey.iKey_1, oDKey.iKey_2);
      vTrace("%lld-->"_Stz, pCursor->iKey());

      pCursor = pNextSLBlock(pCursor, i_Level);
      if(pCursor == nullptr) {
        break;
      }
    }
    vTrace("END\n"_Stz);
    vTrace("--------------------------------------------------"
           "--------------------------------------------------\n"_Stz);
  }

  vTrace("=================================================="
         "===================================================\n\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* Insert Key                                                                 */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pInsert(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // try to allocate a block as mini-Heap may be full
  SkipListBlock* pNewSLBlock = aAllotSkipListBlock();
  if(pNewSLBlock == nullptr) {
    RASSERT(0);
    return nullptr;
  }

//  u2 uNewOffset = uGetOffsetFromBase(pNewSLBlock);

  SkipListBlock* xPriorVec[C_MAX_LVL_LEVELS];

  // Get the prior vector at all levels for this key, i.e
  // the elements at each level of the skip list after which
  // we want to insert this key.
  bool bResult = bPriorVecToKey(pKey, xPriorVec);

  if( ! bResult) {
    // Got duplicate key, error
    vMoveToFreeList(pNewSLBlock);
    vTrace("Pds Skip List attempting to insert a duplicate key\nAborting insert...\n"_Stz);
//    vValidate();
    return nullptr;
  }

  // Iterate this vector for each level, and we may or may not
  // insert the key
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {

    i8 iRMod = rand() % (i_Level+1);
    if(iRMod != 0) {
      break;
    }

    SkipListBlock* pCursor = xPriorVec[i_Level];

    // Insert element, update offset values
    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);

    // Assert that we are not inserting a block before the dummy root block
    RASSERT(uGetOffsetFromBase(pNext) != m_uInUseRootOffset);

    bool bResult = bSetNextSLBlock(pCursor, pNewSLBlock, i_Level);
    RASSERT(bResult);

    bResult = bSetNextSLBlock(pNewSLBlock, pNext, i_Level);
    RASSERT(bResult);

    m_iNumEntries[i_Level]++;
  }

//  vValidate();
  return pNewSLBlock;
}

/*----------------------------------------------------------------------------*/
/* find key                                                                   */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pFind(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  u2 uOffset = m_uInUseRootOffset;
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);
    while(pNext != nullptr) {
      i8 iCompareRes = m_fpCompare(pNext->aItem(), pKey);

      if(iCompareRes == 0) {
        //Ensure that we are not returning the dummy root block
        if(uGetOffsetFromBase(pNext) == m_uInUseRootOffset)
          return nullptr;

        return pNext;
      } else if(iCompareRes > 0) {
        // pNext > pKey
        uOffset = uGetOffsetFromBase(pCursor);
        break;
      }

      pCursor = pNext;
      pNext = pNextSLBlock(pNext, i_Level);
    }
  }

  return nullptr;
}
/*----------------------------------------------------------------------------*/
/* find last block less than the given key                                    */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pFindLessThan(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  u2 uOffset = m_uInUseRootOffset;
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);
    if(pNext == nullptr)
      RASSERT(0);
    while(pNext != nullptr) {
      i8 iCompareRes = m_fpCompare(pNext->aItem(), pKey);

      if(iCompareRes == 0) {
        // Ensure we are not returning the dummy root block
        if(uGetOffsetFromBase(pNext) == m_uInUseRootOffset)
          return nullptr;
        return pNext;
      } else if(iCompareRes > 0) {
        // pNext > pKey
        if(i_Level == 0) {
          // Ensure we are not returning the dummy root block
          if(uGetOffsetFromBase(pCursor) == m_uInUseRootOffset)
            return nullptr;
          return pCursor;
        }
        uOffset = uGetOffsetFromBase(pCursor);
        break;
      }

      pCursor = pNext;
      pNext = pNextSLBlock(pNext, i_Level);

      if(pNext == nullptr && i_Level == 0) {
        // Ensure we are not returning the dummy root block
        if(uGetOffsetFromBase(pCursor) == m_uInUseRootOffset)
          return nullptr;
        return pCursor;
      }

    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find first block greater than given key                                    */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pFindGreaterThan(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  u2 uOffset = m_uInUseRootOffset;
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);
    while(pNext != nullptr) {
      i8 iCompareRes = m_fpCompare(pNext->aItem(), pKey);

      if(iCompareRes == 0) {
        // Ensure we are not returning the dummy root block
        if(uGetOffsetFromBase(pNext) == m_uInUseRootOffset)
          return nullptr;
        return pNext;
      } else if(iCompareRes > 0) {
        // pNext > pKey
        if(i_Level == 0) {
          // Ensure we are not returning the dummy root block
          if(uGetOffsetFromBase(pNext) == m_uInUseRootOffset)
            return nullptr;
          return pNext;
        }
        uOffset = uGetOffsetFromBase(pCursor);
        break;
      }

      pCursor = pNext;
      pNext = pNextSLBlock(pNext, i_Level);

      if(pNext == nullptr && i_Level == 0) {
        // Ensure we are not returning the dummy root block
        if(uGetOffsetFromBase(pCursor) == m_uInUseRootOffset)
          return nullptr;
        return pCursor;
      }

    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Get the first block after the root block                                   */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pFirstBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  if(m_iNumEntries[0] == 0)
    return nullptr;

  SkipListBlock* pRootBlock = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);

  SkipListBlock* pFirstBlock = pNextSLBlock(pRootBlock, 0);

  RASSERT(pFirstBlock != nullptr);
  RASSERT(pFirstBlock != pRootBlock);

  return pFirstBlock;
}

/*----------------------------------------------------------------------------*/
/* Number of free bytes                                                       */
/*----------------------------------------------------------------------------*/
i8 Pds::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  return m_xWsp.nFreeBytes();
}
/*----------------------------------------------------------------------------*/
/* Number of elements in this Skip List                                       */
/*----------------------------------------------------------------------------*/
i8 Pds::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  return m_iNumEntries[0];
}

/*----------------------------------------------------------------------------*/
/* Number of elements in this Skip List                                       */
/*----------------------------------------------------------------------------*/
i8 Pds::iNumEntries(i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  RASSERT(iLevel >= 0 && iLevel < C_MAX_LVL_LEVELS);
  return m_iNumEntries[iLevel];
}

/*----------------------------------------------------------------------------*/
/* Validate this Skip List                                                    */
/*----------------------------------------------------------------------------*/
void Pds::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Validate free list
  u2 uOffset = m_uInUseRootOffset;
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {

    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);
    i8 iCount = 0;

    while(pNext != nullptr) {
      iCount++;
      pNext = pNextSLBlock(pNext, i_Level);
    }
    RASSERT(iCount == m_iNumEntries[i_Level]);
  }

  // Validate free list
  uOffset = m_uFreeListRootOffset;

  SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

  SkipListBlock* pNext = pNextSLBlock(pCursor, 0);
  i8 iCount = 0;

  while(pNext != nullptr) {
    iCount++;
    pNext = pNextSLBlock(pNext, 0);
  }
    RASSERT(iCount == m_iFreeListEntries);
}

/*----------------------------------------------------------------------------*/
/* Verify the integrity of this Skip List                                     */
/*----------------------------------------------------------------------------*/
bool Pds::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* allot a skip list block in its mini-heap buffer                            */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::aAllotSkipListBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListBlock* pAllocBlock = nullptr;

  // Check the free list if it has a block that we can re-use
  if(m_iFreeListEntries > 0) {
    // Get the dummy root block
    SkipListBlock* pFreeListDummyRoot = (SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset);

    // Get the next block (It should exist)
    SkipListBlock* pNextFree = pNextSLBlock(pFreeListDummyRoot, 0);
    RASSERT(pNextFree != nullptr);

    pAllocBlock = pNextFree;

    SkipListBlock* pNextNext = pNextSLBlock(pAllocBlock, 0);

    // Set the dummy root to point to next->next
    // next->next can be null, which is ok
    bool bResult = bSetNextSLBlock(pFreeListDummyRoot, pNextNext, 0);
    RASSERT(bResult == 1);

    m_iFreeListEntries--;
    RASSERT(m_iFreeListEntries >= 0);
  } else {
    pAllocBlock = m_xWsp.aAllotHead(m_nBlockSize);
  }

  if (! pAllocBlock) {
//    vTrace("Unable to allocate new entry in PDS SkipList\n"_Stz);
    return nullptr;
  }

//  pAllocBlock->vSetKey(iKey);

//  vTrace("Block addr: %p\n"_Stz, pAllocBlock);
  return pAllocBlock;
}

/*----------------------------------------------------------------------------*/
/* calculate address from base address of this object + offset                */
/*----------------------------------------------------------------------------*/
void* Pds::pGetAddrFromOffset(u2 uOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  return aAdd<void*>(this,uOffset*16);
}

/*----------------------------------------------------------------------------*/
/* calculate offset from base address of this object                          */
/*----------------------------------------------------------------------------*/
u2 Pds::uGetOffsetFromBase(void* aAddress)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  if(aAddress == nullptr)
    return (u2) 0;

  char* aAddr0 = reinterpret_cast<char*>(this);
  char* aAddr1 = reinterpret_cast<char*>(aAddress);

  RASSERT((aAddr1 - aAddr0)%16 == 0);
  i8 iOffset = (aAddr1 - aAddr0)/16; // offset is in multiples of 2 words
  RASSERT((iOffset >= 0) && (iOffset < USHRT_MAX));
  // TODO: Assert offset is in the range [0, LastUsableOffset];
  return (u2) iOffset;
}

/*----------------------------------------------------------------------------*/
/* move given block to the free list                                          */
/*----------------------------------------------------------------------------*/
void Pds::vMoveToFreeList(SkipListBlock* pSLBlock)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Clear the offset values for all the levels for this block
  for(i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    pSLBlock->m_xNode.vNext(i_Level, 0);
  }

//  MemLib::vZero(pSLBlock,m_nBlockSize,  CN_NONE);

  SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset);

  SkipListBlock* pNext = pNextSLBlock(pCursor, 0);

  bool bResult = bSetNextSLBlock(pSLBlock, pNext, 0);
  RASSERT(bResult);

  bResult = bSetNextSLBlock(pCursor, pSLBlock, 0);
  RASSERT(bResult);

  m_iFreeListEntries++;
}

/*----------------------------------------------------------------------------*/
/* Set next SkipListBlock node offset, given next block address               */
/*----------------------------------------------------------------------------*/
bool Pds::bSetNextSLBlock(SkipListBlock* pStartBlk, SkipListBlock* pEndBlock, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  u2 uOffset = uGetOffsetFromBase(pEndBlock);
  pStartBlk->m_xNode.vNext(iLevel, uOffset);
  return true;
}

/*----------------------------------------------------------------------------*/
/* Set next SkipListBlock node offset, given next block offset                */
/*----------------------------------------------------------------------------*/
bool Pds::bSetNextSLBlock(SkipListBlock* pStartBlk, u2 uOffset, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  // TODO: Assert offset is in the range [0, LastUsableOffset];
  pStartBlk->m_xNode.vNext(iLevel, uOffset);
  return true;
}

/*----------------------------------------------------------------------------*/
/* Get next SkipListBlock node pointer                                        */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pNextSLBlock(SkipListBlock* pSLBlock, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  if(pSLBlock == nullptr)
    return nullptr;

  u2 uNext = pSLBlock->m_xNode.uNext(iLevel);

  if(uNext == 0)
    return nullptr;

  return (SkipListBlock*) pGetAddrFromOffset(uNext);
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for insert key, such that next of PriorVec[0] is      */
/* specifically the point to insert at                                        */
/*----------------------------------------------------------------------------*/
bool Pds::bPriorVecToKey(void* pKey, SkipListBlock* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListBlock* pCurr = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);

  for (i8 i_Level = C_MAX_LVL_LEVELS - 1; i_Level >= 0; i_Level--) {

    if(m_iNumEntries[i_Level] == 0) {
      aPriorVec[i_Level] = pCurr;
      continue;
    }

    SkipListBlock* pNext = pNextSLBlock(pCurr, i_Level);

    i8 iCompareRes = 0;
    while(pNext != nullptr) {
      iCompareRes = m_fpCompare(pNext->aItem(), pKey);
      if(iCompareRes == 0)
        return false;
      else if(iCompareRes > 0)
        break;
      pCurr = pNext;
      pNext = pNextSLBlock(pCurr, i_Level);
    }

    aPriorVec[i_Level] = pCurr;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* compare two items                                                          */
/*----------------------------------------------------------------------------*/
i8 Pds::iIntCompare(void* aItem_l, void* aItem_r)
{
  _static_
  i8 iKey_l = *(i8*) aItem_l;
  i8 iKey_r = *(i8*) aItem_r;

  // do the node names match?
  if (iKey_l == iKey_r)
    return 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (iKey_l >= iKey_r)
    return 1;
  else
    return -1;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Pds::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS);

  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
