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
Pds::Pds(RowPix* pRowPix)
    : DataBuffer(CN_PDS, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(CacheLine::bOnCacheLineBoundary(this));

  i8 iRowSizeProper = Row::nRowProper(pRowPix->m_nColumns);

  // Calculate block size and padding ------------------
  m_nBlockSize = sizeof(SkipListNode) + iRowSizeProper + sizeof(m_iSynthRowKey);
  m_nPaddingPerBlk = 0;

  // Ensure block size is a multiple of 16 bytes
  i8 iMod = m_nBlockSize % 16;
  if(iMod != 0) {
    m_nPaddingPerBlk = 16 - iMod;
    m_nBlockSize += m_nPaddingPerBlk;
  }

  m_iSynthRowKey  = 0;

  // Init temp variables -------------------------------
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_iCurStzSetSpace = 0;

  // Set compare function ------------------------------
  m_fpCompare = iIntCompare;

  // Initialise the workspace --------------------------
  m_iFreeListEntries = 0;
  m_xWsp.vInit(m_nBlockSize);

  // Add dummy root nodes to the Free and In Use skip lists
  // Note: Dummy root blocks are zeroed since we don't know what the key type is

  SkipListBlock* aFreeListRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aFreeListRootOffset,m_nBlockSize,CN_NONE);
  m_uFreeListRootOffset = uGetOffsetFromBase((void*)aFreeListRootOffset);
  // Assert the offset isn't some large garbage value
  RASSERT((m_uFreeListRootOffset != 0) && (m_uFreeListRootOffset < 1000));

  SkipListBlock* aInUseRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aInUseRootOffset,m_nBlockSize,CN_NONE);
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aInUseRootOffset);
  RASSERT((m_uInUseRootOffset != 0) && (m_uInUseRootOffset < 1000));


  // Reset the number of entries to zero since we don't count the dummy blocks
  m_iFreeListEntries = 0;
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
  }

  // Init RowPix object -------------------------------
  vInitPix(pRowPix);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pds::Pds(i8 nBytes, CompareFunc fpCompare, RowPix* pRowPix)
    : DataBuffer(CN_PDS, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(CacheLine::bOnCacheLineBoundary(this));

  // Assert that there is enough bytes for the default key (i8)
  RASSERT(nBytes >= C_MIN_KEY_LEN);

  // Calculate block size and padding ------------------
  m_nBlockSize = sizeof(SkipListNode) + nBytes;
  m_nPaddingPerBlk = 0;

  // Ensure block size is a multiple of 16 bytes
  i8 iMod = m_nBlockSize % 16;
  if(iMod != 0) {
    m_nPaddingPerBlk = 16 - iMod;
    m_nBlockSize += m_nPaddingPerBlk;
  }

  m_iSynthRowKey  = 0;

  // Init temp variables -------------------------------
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_iCurStzSetSpace = 0;


  // Set compare function ------------------------------
  if(fpCompare) {
    m_fpCompare = fpCompare;
  } else {
    m_fpCompare = iIntCompare;
  }

  m_iFreeListEntries = 0;

  // Initialise the workspace --------------------------
  m_xWsp.vInit(m_nBlockSize);

  // Add dummy root nodes to the Free and In Use skip lists
  // Note: Dummy root blocks are zeroed since we don't know what the key type is

  SkipListBlock* aFreeListRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aFreeListRootOffset,m_nBlockSize,CN_NONE);
  m_uFreeListRootOffset = uGetOffsetFromBase((void*)aFreeListRootOffset);
  // Assert the offset isn't some large garbage value
  RASSERT((m_uFreeListRootOffset != 0) && (m_uFreeListRootOffset < 1000));

  SkipListBlock* aInUseRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aInUseRootOffset,m_nBlockSize,CN_NONE);
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aInUseRootOffset);
  RASSERT((m_uInUseRootOffset != 0) && (m_uInUseRootOffset < 1000));


  // Reset the number of entries to zero since we don't count the dummy blocks
  m_iFreeListEntries = 0;
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
  }

  // Init RowPix object -------------------------------
  if (pRowPix)
    vInitPix(pRowPix);
  else
    m_oRowPix.m_bInitialised = false;

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
/* Allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* Pds::pAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Do we have space for the row?
  if (nFreeBytes() < m_nBlockSize)
    return nullptr;

  // Pass an integer key to Pds that we increment each time we insert
  // Essentially append
  SkipListBlock* pSkipListBlock = pInsert((void*) &(++m_iSynthRowKey));
  i8* pKey = (i8*)pSkipListBlock->aItem();
  *pKey = m_iSynthRowKey;

  // Calculate the address of the Row
  // <(i8)key><Row>
  Row* aRowCur = aAdd<Row*>(pSkipListBlock->aItem(), sizeof(m_iSynthRowKey));

  // Initialise the row at the given addr
  new(aRowCur) Row();

  m_aRowCur = aRowCur;

  return aRowCur;
}

/*----------------------------------------------------------------------------*/
/* Allot a row and copy given row into it                                     */
/*----------------------------------------------------------------------------*/
Row* Pds::pCopyRow(Row* pRow, bool bOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  RASSERT(m_oRowPix.m_bInitialised);

  Row* pNewRow = pAllotRow();
  if (! pNewRow) {
    vTrace("%s: Unable to copy row as the Pds is full\n"_Stz, __func__ );
    return nullptr;
  }

  // If this Row contains strings,
  // allot a String Set from the tail of the Pds
  if(iStzSetSize()) {
    if (aAllotStzSet() == nullptr) {
      vTrace("%s: Unable to copy rows as the Pds string area is full\n"_Stz, __func__ );
      vTrace("Pds Rows: %lld\n"_Stz, (m_iSynthRowKey));
      return nullptr;
    }
  }

  i8 iVal;
  Stz* szSrcStz;

  i8 nCols = m_oRowPix.m_nColumns;
  i8 i_Col;
  m_i_ColCrs = 0;
  for (i_Col = 0; i_Col < nCols; i_Col++) {

    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;

    if (DM_VCH == eDatum) {
      // vch datum

      if (C_DATUM_NULL == pRow->iColVal_int(i_Col)) {
        vDepositColVal(C_DATUM_NULL);
      }
      else {
        RASSERT(m_aStzSet != nullptr);
        if (bOffset) {
          szSrcStz = pRow->szColVal_ofs(i_Col);
          vDepositColVal(szSrcStz, m_oRowPix.m_xaColPix[i_Col].m_uLength);
        } else {
          szSrcStz = (Stz*) pRow->iColVal_int(i_Col);
          vDepositColVal(szSrcStz, m_oRowPix.m_xaColPix[i_Col].m_uLength);
        }
      }
    }
    else {
      // all others

      iVal = pRow->iColVal_int(i_Col);
      vDepositColVal(iVal);
    }
  }

  vReset();

  return pNewRow;
}

/*----------------------------------------------------------------------------*/
/* Allot space for Stz of fixed size, return dst str addr                     */
/*----------------------------------------------------------------------------*/
void* Pds::aAllotStzSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  RASSERT(iStzSetSize());
  if (nFreeBytes() < iStzSetSize())
    return nullptr;

  void* aStzSet = (void*) m_xWsp.aAllotTail();

  if(aStzSet == nullptr)
    return nullptr;

  RASSERT(DoubleWord::bOnDoubleWordBoundary(aStzSet));

  m_iCurStzSetSpace = iStzSetSize();
  m_aStzSet = aStzSet;
  return aStzSet;
}

/*----------------------------------------------------------------------------*/
/* remove key from in use skip list and move to free list                     */
/*----------------------------------------------------------------------------*/
bool Pds::bDelete(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);


  // -------------- Sanity check ----------------
  // Assert the key is greater than the root dummy block key of zero
  SkipListBlock* pRootCursor = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);
  i8 iCompareRes = m_fpCompare(pRootCursor->aItem(), pKey);
  RASSERT(iCompareRes == -1);
  // --------------------------------------------

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

  // -------------- Sanity check ----------------
  // Assert the key is greater than the root dummy block key of zero
  SkipListBlock* pRootCursor = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);
  i8 iCompareRes = m_fpCompare(pRootCursor->aItem(), pKey);
  RASSERT(iCompareRes == -1);
  // --------------------------------------------

  // try to allocate a block as mini-Heap may be full
  SkipListBlock* pNewSLBlock = aAllotSkipListBlock();
  if(pNewSLBlock == nullptr) {
//    RASSERT(0);
    return nullptr;
  }

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
/* Init the RowPix structure                                                  */
/*----------------------------------------------------------------------------*/
void Pds::vInitPix(RowPix* pRowPix)
{
  RASSERT(pRowPix);

  m_oRowPix.vCopyRowPix(pRowPix);

  i8 nCols = m_oRowPix.m_nColumns;
  i8 iTotalStzArea = 0;

  // Calculate total size of string area needed per row
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
    if (DM_VCH == eDatum) {
      i8 nBytes = m_oRowPix.m_xaColPix[i_Col].m_uLength;

      if (nBytes) {
        iTotalStzArea += String_d::iBytesToAlloc(nBytes);
      }
    }
  }
  // Set size of StzSet area
  vStzSetSize(iTotalStzArea);

  i8 iRowSizeProper = Row::nRowProper(m_oRowPix.m_nColumns);

  // Assert the space we are allocating for the row + key is less than or equal to the block size
  RASSERT((iRowSizeProper + (i8)sizeof(m_iSynthRowKey)) <= m_nBlockSize);

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
/* Last usable address in this Pds                                            */
/*----------------------------------------------------------------------------*/
void* Pds::aLastUsableByteAddr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  return m_xWsp.aLastUsableByteAddr();
}

/*----------------------------------------------------------------------------*/
/* return the last usable offset if we want to store a block of size m_nBlockSize*/
/*----------------------------------------------------------------------------*/
i8 Pds::iLastUsableOffset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  void* pLastUsableAddr = aAdd<void*>(aLastUsableByteAddr(), (1 - m_nBlockSize));

  char* aAddr0 = reinterpret_cast<char*>(this);
  char* aAddr1 = reinterpret_cast<char*>(pLastUsableAddr);

  return aAddr1 - aAddr0;
}

/*----------------------------------------------------------------------------*/
/* Number of elements at iLevel in this Skip List                             */
/*----------------------------------------------------------------------------*/
i8 Pds::iNumEntries(i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  RASSERT(iLevel >= 0 && iLevel < C_MAX_LVL_LEVELS);
  return m_iNumEntries[iLevel];
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void Pds::vPrintRows(File* pFilePtr, Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  UNUSED(pFilePtr);

  RASSERT(m_oRowPix.m_bInitialised);

  HV_Record hvRecord(&m_oRowPix);

  PdsCursor* pCursor = new PdsCursor(this);

  i8 iKeySize = sizeof(m_iSynthRowKey);

  while( ! pCursor->bIsCurrNull()) {
    SkipListBlock* pSkipListBlock = pCursor->m_pCurrentBlock;
    Row* aRowCur = aAdd<Row*>(pSkipListBlock->aItem(), iKeySize);

    hvRecord->vToCsvRecord(aRowCur);
    hvRecord->vPrintAsRow(szLabel);
    pCursor->vNext();
  }
}

/*----------------------------------------------------------------------------*/
/* Reset tmp variables                                                        */
/*----------------------------------------------------------------------------*/
void Pds::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_iCurStzSetSpace = 0;
}

/*----------------------------------------------------------------------------*/
/* Set the size of the StringSet tail area                                    */
/*----------------------------------------------------------------------------*/
void Pds::vStzSetSize(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  m_xWsp.vTailSize(nBytes);
}

/*----------------------------------------------------------------------------*/
/* Get the size of the StringSet tail area                                    */
/*----------------------------------------------------------------------------*/
i8 Pds::iStzSetSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  return m_xWsp.iTailSize();
}

/*----------------------------------------------------------------------------*/
/* Validate this Skip List                                                    */
/*----------------------------------------------------------------------------*/
void Pds::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // Validate in use list

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
    pAllocBlock = m_xWsp.aAllotHead();
  }

  if (! pAllocBlock) {
//    vTrace("Unable to allocate new entry in PDS SkipList\n"_Stz);
    return nullptr;
  }

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

  RASSERT((iOffset >= 0) && (iOffset <= iLastUsableOffset()));

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

  // Key value of 0 is used for the dummy root blocks
  RASSERT(iKey_r != 0);

  // do the node keys match?
  if (iKey_l == iKey_r)
    return 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (iKey_l >= iKey_r)
    return 1;
  else
    return -1;
}

/*--------------------------------------------------------------------------*/
/* Row helper                                                               */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* advance cursor                                                           */
/*--------------------------------------------------------------------------*/
void Pds::vAdvanceCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(m_i_ColCrs != C_UNKNOWN);
  m_i_ColCrs++;
}


/*--------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
void Pds::vDepositColVal(d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(m_i_ColCrs != C_UNKNOWN);
  m_aRowCur->vColVal_flt(m_i_ColCrs,dColVal);

  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
void Pds::vDepositColVal(i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(m_i_ColCrs != C_UNKNOWN);
  m_aRowCur->vColVal_int(m_i_ColCrs,iColVal);

  vAdvanceCursor();
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
void Pds::vDepositColVal(i8 i_Col, i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  m_aRowCur->vColVal_int(i_Col,iColVal);
}

/*--------------------------------------------------------------------------*/
/* deposit an stz value into next cell                                      */
/*--------------------------------------------------------------------------*/
void Pds::vDepositColVal(Stz* szColVal, i8 iMaxStzLen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(m_iCurStzSetSpace > 0);

  String_d* azStringDyn = azDepositString_begin(iMaxStzLen);

  azStringDyn->vCopy(szColVal);
  i8 iTotalSize = azStringDyn->iTotalSize();
  RASSERT((m_iCurStzSetSpace - iTotalSize) >= 0);

  m_iCurStzSetSpace -= iTotalSize;

  // TODO: Arjun - Check with Ray: nLen or iMaxStzLen?
  vDepositString_end(iTotalSize);
}

/*--------------------------------------------------------------------------*/
/* begin deposit of string                                                  */
/*--------------------------------------------------------------------------*/
String_d* Pds::azDepositString_begin(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  RASSERT(m_i_ColCrs != C_UNKNOWN);

  String_d* szDeposited = new(m_aStzSet) String_d(nSize);

  // TODO: Arjun - Check type conversion with Ray
  i8 i_Ofs = nDifference(m_aRowCur->aColCell(m_i_ColCrs),szDeposited);

  m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

  vAdvanceCursor();


  return szDeposited;
}

/*--------------------------------------------------------------------------*/
/* take bytes for deposited string                                          */
/*--------------------------------------------------------------------------*/
void Pds::vDepositString_end(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  m_aStzSet = aAdd<void*>(m_aStzSet, nSize);
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
