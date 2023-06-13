/*============================================================================*/
/* Pds.cpp --                 */
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
Pds::Pds(void)
  : Object(CN_PDS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(CacheLine::bOnCacheLineBoundary(this));

  m_pFreeListRoot = nullptr;
  // add dummy node to in use skip list
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aAllotSkipListSet(0, 32));

  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
  }


  vInsert(1);
  vInsert(12);
  vInsert(14);

  vDump();

//  SkipListSet* pSkipListSet = (SkipListSet*)uGetAddrFromOffset(m_uUsedRootOffset);
//  vTrace("%lld\n"_Stz, pSkipListSet->m_iItem);
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
/* allot a skip list set in this buffer                                       */
/*----------------------------------------------------------------------------*/
SkipListSet* Pds::aAllotSkipListSet(i16 iKey, i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListSet* aSet = aAllotItem(CN_SKIPLISTSET, nSize);
  if (! aSet)
    return nullptr;

  aSet->vSetKey(iKey);

  return aSet;
}

/*----------------------------------------------------------------------------*/
/* calculate address from base address of this object + offset                */
/*----------------------------------------------------------------------------*/
void Pds::vDump(void)
{
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    SkipListSet* pCursor = (SkipListSet*) pGetAddrFromOffset(m_uInUseRootOffset);

    vTrace("Level %lld, No. of entries: %lld\n"_Stz, i_Level, m_iNumEntries[i_Level]);

    for(i8 iNumEntries = 0; iNumEntries < m_iNumEntries[i_Level] + 1; iNumEntries++) {
      vTrace("%hu-->"_Stz, pCursor->m_iItem);

      pCursor = pNextSLSet(pCursor, i_Level);
      if(pCursor == nullptr) {
        break;
      }
    }
    vTrace("END\n\n"_Stz);
  }
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
//void Pds::vDelete(i16 iVal)
//{
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  //bVerifyItems();
//
//  Ent* aEnt = *aaEnt;
//
//  Ent* xPriorVec[C_MAX_LVL_LEVELS];
//
//  vPosVecPriorToEntry(aEnt, aEnt->aKey(), xPriorVec);
//
//  vCutEntry(aEnt,xPriorVec);
//
//  i8 nEntSize = aEnt->nSizeBytes();
//
//#ifdef __ENT_HEADER
//  vFreeItem(aEnt,nEntSize);
//  //vGiveSpace(nEntSize);
//
//#else
//  //if (aEnt->bPayload()) {
//  aEnt->rxRef().vFree(true);
//  aEnt->rxRef().vFreeSize(nEntSize);
//
//  if (! m_aFreeSpare) {
//    m_aFreeSpare = aEnt;
//    m_nFreeSize  = nEntSize;
//  }
//  vDecItems();
//  //}
//  //else {
//  //  vFreeItem(aEnt,nEntSize);
//  //}
//
//#endif
//
//  m_nDelete++;
//
//#ifdef EB_TRACE_INSERT
//  SP_Ref spRef;
//  SP_Key spKey;
//  vTraceWhere();
//  vTrace("-- delete d%lld %s %s\n"_Stz,
//         m_nDelete,spRef.sz(aEnt->wRef()),spKey.sz(aEnt->aKey(),m_pIndexDef));
//#endif
//
//  // entry no longer exists, set the pointer to be null
//  *aaEnt = nullptr;
//
//  //bVerifyItems();
//
//}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
void Pds::vInsert(i16 iKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Check free list first before allocation
//  if(m_pFreeListRoot != nullptr)
  SkipListSet* pNewSLSet = aAllotSkipListSet(iKey, 32);
  u2 uNewOffset = uGetOffsetFromBase(pNewSLSet);

  SkipListSet* xPriorVec[C_MAX_LVL_LEVELS];

  vPriorVecToKey(iKey, xPriorVec);


  for (i8 i_Level = C_MAX_LVL_LEVELS - 1; i_Level >= 0; i_Level--) {

    i8 iRMod = rand() % (i_Level+1);
    if(iRMod != 0) {
      continue;
    }

    SkipListSet* pCursor = xPriorVec[i_Level];

    u2 uTmpOffset = pCursor->m_xNode.uNext(i_Level);
    pCursor->m_xNode.vNext(i_Level, uNewOffset);
    pNewSLSet->m_xNode.vNext(i_Level, uTmpOffset);

    m_iNumEntries[i_Level]++;
  }
}

/*----------------------------------------------------------------------------*/
/* Get next SkipListSet node pointer                                          */
/*----------------------------------------------------------------------------*/
SkipListSet* Pds::pNextSLSet(SkipListSet* pSLSet ,i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  u2 uNext = pSLSet->m_xNode.uNext(iLevel);

  if(uNext == 0)
    return nullptr;

  return (SkipListSet*) pGetAddrFromOffset(uNext);
}
/*----------------------------------------------------------------------------*/
/* compute prior vector for insert key, such that next of PriorVec[0] is      */
/* specifically the point to insert at                                        */
/*----------------------------------------------------------------------------*/
void Pds::vPriorVecToKey(i8 iKey, SkipListSet* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListSet* pCurr = (SkipListSet*) pGetAddrFromOffset(m_uInUseRootOffset);

  for (i8 i_Level = C_MAX_LVL_LEVELS - 1; i_Level >= 0; i_Level--) {

    if(m_iNumEntries[i_Level] == 0) {
      aPriorVec[i_Level] = pCurr;
      continue;
    }

    SkipListSet* pNext = pNextSLSet(pCurr, i_Level);

    i16 iNextKey = 0;
    if(pNext != nullptr) {
      iNextKey = pNext->m_iItem;
    }

    while(iNextKey != 0 && iNextKey < iKey) {
      pCurr = pNext;
      pNext = pNextSLSet(pCurr, i_Level);
      if(pNext != nullptr) {
        iNextKey = pNext->m_iItem;
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
