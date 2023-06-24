/*============================================================================*/
/* Pds.cpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/

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
Pds::Pds(i8 nBytes)
  : Object(CN_PDS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(CacheLine::bOnCacheLineBoundary(this));

  RASSERT(nBytes >= C_MIN_KEY_LEN);

  m_nBlockSize = sizeof(SkipListNode) + nBytes;

  m_uFreeListRootOffset = uGetOffsetFromBase((void*)aAllotSkipListBlock(0));
  m_iFreeListEntries = 0;
  // add dummy node to in use skip list
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aAllotSkipListBlock(0));

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
/* allot a skip list block in this buffer                                     */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::aAllotSkipListBlock(i8 iKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListBlock* pReturnSet = nullptr;
  SkipListBlock* pFreeListHead = (SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset);
  SkipListBlock* pNextFree = pNextSLSet(pFreeListHead, 0);

  if(pNextFree != nullptr) {
    pReturnSet = pNextFree;
    SkipListBlock* pNextNext = pNextSLSet(pNextFree, 0);
    if(pNextNext != nullptr) {
      pFreeListHead->m_xNode.vNext(0, uGetOffsetFromBase(pNextNext));
      m_iFreeListEntries--;
      RASSERT(m_iFreeListEntries >= 0);
    }
    else {
      pFreeListHead->m_xNode.vNext(0, 0);
      m_iFreeListEntries--;
      RASSERT(m_iFreeListEntries == 0);
    }
  } else {
    pReturnSet = aAllotItem(CN_SKIPLISTBLOCK, m_nBlockSize);
  }

  if (! pReturnSet)
    return nullptr;

  pReturnSet->vSetKey(iKey);

  return pReturnSet;
}

/*----------------------------------------------------------------------------*/
/* remove key from in use skip list and move to free list                     */
/*----------------------------------------------------------------------------*/
void Pds::vDelete(i8 iKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Add an assert to check that all the entries are deleted where
  // the entry to delete has a non-zero value in it's respective level.
  SkipListBlock* pDeleteEntry = nullptr;
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);

    SkipListBlock* pNext = pNextSLSet(pCursor, i_Level);
    while(pNext != nullptr) {
      i8 iNextKey = pNext->iKey();
      if(iNextKey== iKey) {
        pCursor->m_xNode.vNext(i_Level, pNext->m_xNode.uNext(i_Level));
        pDeleteEntry = pNext;
        m_iNumEntries[i_Level]--;
        RASSERT(m_iNumEntries[i_Level] >= 0);
        break;
      } else if(iNextKey > iKey)
        break;
      pCursor = pNext;
      pNext = pNextSLSet(pNext, i_Level);
    }
  }

  if(pDeleteEntry){
    vMoveToFreeList(pDeleteEntry);
  }

}

/*----------------------------------------------------------------------------*/
/* print the skip list                                                        */
/*----------------------------------------------------------------------------*/
void Pds::vDump(u2 uOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  vTrace("=================================================="
         "===================================================\n"_Stz);
  if((SkipListBlock*) pGetAddrFromOffset(uOffset) == nullptr) {
    vTrace("Invalid offset given\n"_Stz);
    return;
  }

  vTrace("| %5s | %6s | %s\n"_Stz, "Level", "# Ents", "Items");

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

    vTrace("| %5lld | %6lld | "_Stz, i_Level, m_iNumEntries[i_Level]);

    for(i8 iNumEntries = 0; iNumEntries < m_iNumEntries[i_Level] + 1; iNumEntries++) {
      vTrace("%lld-->"_Stz, pCursor->iKey());

      pCursor = pNextSLSet(pCursor, i_Level);
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
/* print the skip list of free entries                                        */
/*----------------------------------------------------------------------------*/
void Pds::vDumpFree(void) {
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  vTrace("Free List\n"_Stz);
  vDump(m_uFreeListRootOffset);
}

/*----------------------------------------------------------------------------*/
/* print the skip list of in use entries                                      */
/*----------------------------------------------------------------------------*/
void Pds::vDumpInUse(void) {
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  vTrace("In Use List\n"_Stz);
  vDump(m_uInUseRootOffset);
}

/*----------------------------------------------------------------------------*/
/* find key                                                                   */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pFind(i8 iKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  u2 uOffset = m_uInUseRootOffset;
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

    SkipListBlock* pNext = pNextSLSet(pCursor, i_Level);
    while(pNext != nullptr) {
      i8 iNextKey = pNext->iKey();
      if(iNextKey == iKey) {
        return pNext;
      } else if(iNextKey > iKey) {
        uOffset = uGetOffsetFromBase(pCursor);
        break;
      }
      pCursor = pNext;
      pNext = pNextSLSet(pNext, i_Level);
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* calculate address from base address of this object + offset                */
/*----------------------------------------------------------------------------*/
void* Pds::pGetAddrFromOffset(u2 uOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  return aAdd<void*>(this,uOffset);
}

/*----------------------------------------------------------------------------*/
/* calculate offset from base address of this object                          */
/*----------------------------------------------------------------------------*/
u2 Pds::uGetOffsetFromBase(void* aAddress)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  char* aAddr0 = reinterpret_cast<char*>(this);
  char* aAddr1 = reinterpret_cast<char*>(aAddress);

  i8 iBytes = aAddr1 - aAddr0;
  ASSERT((iBytes >= 0) && (iBytes < USHRT_MAX));
  return (u2) iBytes;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
void Pds::vInsert(i8 iKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Check free list first before allocation
//  if(m_pFreeListRoot != nullptr)
  SkipListBlock* pNewSLSet = aAllotSkipListBlock(iKey);
  u2 uNewOffset = uGetOffsetFromBase(pNewSLSet);

  SkipListBlock* xPriorVec[C_MAX_LVL_LEVELS];

  vPriorVecToKey(iKey, xPriorVec);

  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {

    i8 iRMod = rand() % (i_Level+1);
    if(iRMod != 0) {
      break;
    }

    SkipListBlock* pCursor = xPriorVec[i_Level];

    u2 uTmpOffset = pCursor->m_xNode.uNext(i_Level);
    pCursor->m_xNode.vNext(i_Level, uNewOffset);
    pNewSLSet->m_xNode.vNext(i_Level, uTmpOffset);

    m_iNumEntries[i_Level]++;
  }
}

/*----------------------------------------------------------------------------*/
/* allot a skip list block in this buffer                                     */
/*----------------------------------------------------------------------------*/
void Pds::vMoveToFreeList(SkipListBlock* pSLSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  pSLSet->vSetKey(0);
  for(i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    pSLSet->m_xNode.vNext(i_Level, 0);
  }
  SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset);

  SkipListBlock* pNext = pNextSLSet(pCursor, 0);

  if(pNext != nullptr) {
    pSLSet->m_xNode.vNext(0, uGetOffsetFromBase(pNext));
  } else {
    pSLSet->m_xNode.vNext(0, 0);
  }

  pCursor->m_xNode.vNext(0, uGetOffsetFromBase(pSLSet));
  m_iFreeListEntries++;
}

/*----------------------------------------------------------------------------*/
/* Get next SkipListBlock node pointer                                        */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pNextSLSet(SkipListBlock* pSLSet , i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  if(pSLSet == nullptr)
    return nullptr;

  u2 uNext = pSLSet->m_xNode.uNext(iLevel);

  if(uNext == 0)
    return nullptr;

  return (SkipListBlock*) pGetAddrFromOffset(uNext);
}

/*----------------------------------------------------------------------------*/
/* compute prior vector for insert key, such that next of PriorVec[0] is      */
/* specifically the point to insert at                                        */
/*----------------------------------------------------------------------------*/
void Pds::vPriorVecToKey(i8 iKey, SkipListBlock* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListBlock* pCurr = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);

  for (i8 i_Level = C_MAX_LVL_LEVELS - 1; i_Level >= 0; i_Level--) {

    if(m_iNumEntries[i_Level] == 0) {
      aPriorVec[i_Level] = pCurr;
      continue;
    }

    SkipListBlock* pNext = pNextSLSet(pCurr, i_Level);

    i8 iNextKey = 0;
    if(pNext != nullptr) {
      iNextKey = pNext->iKey();
    }

    while(iNextKey != 0 && iNextKey < iKey) {
      pCurr = pNext;
      pNext = pNextSLSet(pCurr, i_Level);
      if(pNext != nullptr) {
        iNextKey = pNext->iKey();
      } else {
        iNextKey = 0;
      }
    }

    aPriorVec[i_Level] = pCurr;
  }

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
