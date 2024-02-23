/*============================================================================*/
/* Pds.cpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/

#include <climits>
#include "BASE.h"

#include "Pds.hpp"
#include "MiniHeap_t.hpp"
#include "eCX.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of cell array of row                                           */
/*----------------------------------------------------------------------------*/
extern "C" void* aExtern_PdsRowBufferCellArray(void* pPdsRowBuffer, i8 iRow)
{
  return Pds::aGetCellArray((Pds*)pPdsRowBuffer, iRow);
}

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
Pds::Pds(RowPix* pRowPix, eBT eBufferType)
    : DataBuffer(CN_PDS, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(CacheLine::bOnCacheLineBoundary(this));

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
  m_eBufferType = eBufferType;

  // Init temp variables -------------------------------
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_nRowStringBytes = 0;

  // Set compare function ------------------------------
  m_fpCompare = PdsUtils::iIntCompare;

  // Initialise the workspace --------------------------
  m_iFreeListEntries = 0;
  m_xWsp.vInit(m_nBlockSize);

  // Add dummy root nodes to the Free and In Use skip lists
  // Note: Dummy root blocks are zeroed since we don't know what the key type is

  SkipListBlock* aFreeListRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aFreeListRootOffset,m_nBlockSize,CN_NONE);
  m_uFreeListRootOffset = uGetOffsetFromBase((void*)aFreeListRootOffset);

  SkipListBlock* aInUseRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aInUseRootOffset,m_nBlockSize,CN_NONE);
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aInUseRootOffset);


  // Reset the number of entries to zero since we don't count the dummy blocks
  m_iFreeListEntries = 0;
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
    // Init the tail vector to the dummy root
    m_auTailOffsets[i_Level] = m_uInUseRootOffset;
  }

  // Init RowPix object -------------------------------
  vInitPix(pRowPix);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pds::Pds(StreamDef* pStreamDef, bool bAddKeyHash, eBT eBufferType)
    : DataBuffer(CN_PDS, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(CacheLine::bOnCacheLineBoundary(this));

  i8 iRowSizeProper = Row::nRowProper(pStreamDef->nColumns() + (bAddKeyHash ? 1 : 0));

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
  m_eBufferType = eBufferType;

  // Init temp variables -------------------------------
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_nRowStringBytes = 0;

  // Set compare function ------------------------------
  m_fpCompare = PdsUtils::iIntCompare;

  // Initialise the workspace --------------------------
  m_iFreeListEntries = 0;
	m_xWsp.vInit(m_nBlockSize);

  // Add dummy root nodes to the Free and In Use skip lists
  // Note: Dummy root blocks are zeroed since we don't know what the key type is

  SkipListBlock* aFreeListRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aFreeListRootOffset,m_nBlockSize,CN_NONE);
  m_uFreeListRootOffset = uGetOffsetFromBase((void*)aFreeListRootOffset);
  // Assert the offset isn't some large garbage value
//  ASSERT((m_uFreeListRootOffset != 0) && (m_uFreeListRootOffset < 1000));

  SkipListBlock* aInUseRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aInUseRootOffset,m_nBlockSize,CN_NONE);
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aInUseRootOffset);
//  ASSERT((m_uInUseRootOffset != 0) && (m_uInUseRootOffset < 1000));


  // Reset the number of entries to zero since we don't count the dummy blocks
  m_iFreeListEntries = 0;
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
    // Init the tail vector to the dummy root
    m_auTailOffsets[i_Level] = m_uInUseRootOffset;
  }

  // Init RowPix object -------------------------------
  vInitPix(pStreamDef, bAddKeyHash);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pds::Pds(i8 nBytes, CompareFunc fpCompare, bool bAddToHashMap)
    : DataBuffer(bAddToHashMap, CN_PDS, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(CacheLine::bOnCacheLineBoundary(this));

  // Assert that there is enough bytes for the default key (i8)
  ASSERT(nBytes >= C_MIN_KEY_LEN);

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
  m_eBufferType = BT_BUFFERID;

  // Init temp variables -------------------------------
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_nRowStringBytes = 0;


  // Set compare function ------------------------------
  if(fpCompare) {
    m_fpCompare = fpCompare;
  } else {
    m_fpCompare = PdsUtils::iIntCompare;
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
//  ASSERT((m_uFreeListRootOffset != 0) && (m_uFreeListRootOffset < 1000));

  SkipListBlock* aInUseRootOffset = aAllotSkipListBlock();
  MemLib::vZero(aInUseRootOffset,m_nBlockSize,CN_NONE);
  m_uInUseRootOffset = uGetOffsetFromBase((void*)aInUseRootOffset);
//  ASSERT((m_uInUseRootOffset != 0) && (m_uInUseRootOffset < 1000));


  // Reset the number of entries to zero since we don't count the dummy blocks
  m_iFreeListEntries = 0;
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    m_iNumEntries[i_Level] = 0;
    // Init the tail vector to the dummy root
    m_auTailOffsets[i_Level] = m_uInUseRootOffset;
  }

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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of cell array of row                                           */
/*----------------------------------------------------------------------------*/
void* Pds::aGetCellArray(Pds* pPdsRowBuffer, i8 iRow)
{
  _static_

  SkipListBlock* pSkipListBlock = pPdsRowBuffer->pFind(&iRow);
  if (pSkipListBlock == nullptr) {
    return nullptr;
  }

  // Calculate the address of the Row
  // <(i8)key><Row>
  Row* pRow = pSkipListBlock->aRow();

  return (void*)pRow->aCellArray();
}

void Pds::vRepair(Object* pObject)
{
  _static_
  Pds* pPds = _pPds(pObject);
  AM::vDeleteOperand(pPds,nullptr);
}

/*----------------------------------------------------------------------------*/
/* get the Pds buffer that contains the given address                         */
/*----------------------------------------------------------------------------*/
Pds* Pds::pToPds(void* aAddr)
{
  _static_

  DataBuffer* pDataBuffer = pToDataBuffer(aAddr);

  Pds* pPds =
      reinterpret_cast<Pds*>(pDataBuffer);

  return pPds;
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

  // Pass an integer key to Pds that we increment each time we append
  SkipListBlock* pSkipListBlock = pAppend((void*) &(m_iSynthRowKey));
  i8* pKey = (i8*)pSkipListBlock->aItem();
  *pKey = m_iSynthRowKey;

  m_iSynthRowKey++;

  // Calculate the address of the Row
  // <(i8)key><Row>
  Row* aRowCur = pSkipListBlock->aRow();

  // Initialise the row at the given addr
  new(aRowCur) Row();

  m_aRowCur = aRowCur;
  m_i_ColCrs = 0;
  m_nRowStringBytes = 0;
  m_aStzSet = nullptr;

  return aRowCur;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* Pds::pAllotRow(i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // allot a row
  Row* pRow = pAllotRow();

  if ( ! pRow )
    return nullptr;

  ASSERT(C_VARGS_BEGIN == iVArgsBegin);

  va_list xVarArgs;

  va_start(xVarArgs,iVArgsBegin);

  for (i8 i_Col = 0; i_Col < m_oRowPix.m_nColumns; i_Col++) {

    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;

    if (DM_VCH == eDatum) {
      // STRING_DEPOSIT
      Stz* szVal = va_arg(xVarArgs,Stz*);
      if (szVal) {
        bool bResult = bDepositColVal(szVal);
        if( ! bResult ) {
          vDiscardInvalidRow();
          return nullptr;
        }
      }
      else {
        vDepositColVal(C_DATUM_NULL);
      }
      // END STRING_DEPOSIT
    }
    else {
      i8 iVal = va_arg(xVarArgs,i8);
      vDepositColVal(iVal);
    }
  }

  i8 iVArgsEnd = va_arg(xVarArgs,i8);
  ASSERT(C_VARGS_END == iVArgsEnd);
  UNUSED(iVArgsEnd);

  va_end(xVarArgs);

  return pRow;
}

/*----------------------------------------------------------------------------*/
/* Allot a row and the string set space. Init the string set space and return addr */
/*----------------------------------------------------------------------------*/
//Row* Pds::pAllotRowFull(char** aaStzSet)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
//
//  *aaStzSet = nullptr;
//  // Do we have space for the row?
//  if (nFreeBytes() < m_nBlockSize )
//    return nullptr;
//
//  // Pass an integer key to Pds that we increment each time we insert
//  // Essentially append
//  SkipListBlock* pSkipListBlock = pInsert((void*) &(m_iSynthRowKey));
//  i8* pKey = (i8*)pSkipListBlock->aItem();
//  *pKey = m_iSynthRowKey;
//
//  m_iSynthRowKey++;
//
//  // Calculate the address of the Row
//  // <(i8)key><Row>
//  Row* aRowCur = pSkipListBlock->aRow();
//
//  // Initialise the row at the given addr
//  new(aRowCur) Row();
//
//  m_aRowCur = aRowCur;
//  m_i_ColCrs = 0;
//
//  // If this Row contains strings,
//  // allot a String Set from the tail of the Pds
//  if(iStzSetSize()) {
//    if (aAllotStzSet() == nullptr) {
//      vTrace("%s: Unable to copy rows as the Pds string area is full\n"_Stz, __func__ );
//      vTrace("Pds Rows: %lld\n"_Stz, (m_iSynthRowKey));
//      vDiscardInvalidRow();
//      return nullptr;
//    }
//    ASSERT(m_aStzSet);
//    *aaStzSet = (char*)m_aStzSet;
//    void* pCurStzSet = m_aStzSet;
//
//    ASSERT(m_oRowPix.m_bInitialised);
//
//    // do a new() for each string area inside the string set where the column is of type VCH
//    i8 nCols = m_oRowPix.m_nColumns;
//    for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
//      eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
//
//      if (DM_VCH == eDatum) {
//        i8 nSize = m_oRowPix.m_xaColPix[i_Col].m_uLength;
//        String_d* azStringDyn = new(pCurStzSet) String_d(nSize);
//        pCurStzSet = aAdd<void*>(pCurStzSet, azStringDyn->iTotalSize());
//      }
//    }
//  }
//
//  return aRowCur;
//}

/*----------------------------------------------------------------------------*/
/* Append Key to end of the skip list                                         */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pAppend(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // try to allocate a block as mini-Heap may be full
  SkipListBlock* pNewSLBlock = aAllotSkipListBlock();
  if(pNewSLBlock == nullptr) {
//    ASSERT(0);
    return nullptr;
  }

  u2 uNewOffset = uGetOffsetFromBase(pNewSLBlock);

  // Iterate this vector for each level, and we may or may not
  // insert the key based on a random number
//  u8 uSkipLevels = (RndLib::uInteger() % C_MAX_LVL_LEVELS) + 1;

  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {

//    i8 iRMod = rand() % (i_Level+1);
    i8 iRMod = (i8) (RndLib::uInteger() % (i_Level+1));
    if(iRMod != 0) {
      break;
    }

    u2 uTailOffset = m_auTailOffsets[i_Level];
    SkipListBlock* pCurrTail = (SkipListBlock*) pGetAddrFromOffset(uTailOffset);

    if( uTailOffset != m_uInUseRootOffset) {
      i8 iCompareRes = m_fpCompare(pCurrTail->aItem(), pKey);
      UNUSED(iCompareRes);

      // Assert the new key being added is greater than the last key at this level
      ASSERT(iCompareRes < 0);
      ASSERT(m_iNumEntries[i_Level] != 0);
    }

    // Assert next is null
    SkipListBlock* pNext = pNextSLBlock(pCurrTail, i_Level);
    UNUSED(pNext);
    ASSERT(pNext == nullptr);

    // Insert element, update offset values
    pCurrTail->m_xNode.vNext(i_Level, uNewOffset);
    pNewSLBlock->m_xNode.vNext(i_Level, 0);
    m_auTailOffsets[i_Level] = uNewOffset;

    m_iNumEntries[i_Level]++;
  }

//  vValidate();
  return pNewSLBlock;
}

/*----------------------------------------------------------------------------*/
/* get the buffer type                                                        */
/*----------------------------------------------------------------------------*/
eBT Pds::eBufferType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  return m_eBufferType;
}

/*----------------------------------------------------------------------------*/
/* set the buffer type                                                        */
/*----------------------------------------------------------------------------*/
void Pds::vBufferType(eBT eBufferType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  m_eBufferType = eBufferType;
}

/*----------------------------------------------------------------------------*/
/* Allot a row and copy given row into it                                     */
/*----------------------------------------------------------------------------*/
Row* Pds::pCopyRow(Row* pRow, bool bSrcHasPtnHash, bool bCopyPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  ASSERT(m_oRowPix.m_bInitialised);

  Row* pNewRow = pAllotRow();
  if (! pNewRow) {
//    vTrace("%s: Unable to copy row as the Pds is full\n"_Stz, __func__ );
    return nullptr;
  }

  i8 iVal;
  Stz* szSrcStz;

  i8 nCols = m_oRowPix.m_nColumns;
  i8 i_Col;
  for (i_Col = 0; i_Col < nCols; i_Col++) {

    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;

    if (DM_VCH == eDatum) {
      // vch datum

      if (C_DATUM_NULL == pRow->iColVal_int(i_Col)) {
        vDepositColVal(C_DATUM_NULL);
      }
      else {
        szSrcStz = pRow->szColVal_ofs(i_Col);
        bool bResult = bDepositColVal(szSrcStz);
        if( ! bResult ) {
          vDiscardInvalidRow();
          return nullptr;
        }
      }
    }
    else {
      // all others

      iVal = pRow->iColVal_int(i_Col);
      vDepositColVal(iVal);
    }
  }

  if (bSrcHasPtnHash && bCopyPtnHash) {
    iVal = pRow->iColVal_int(i_Col);
    vDepositColVal(iVal);
  }

  vReset();

  return pNewRow;
}

/*----------------------------------------------------------------------------*/
/* Allot a row and copy given row into it                                     */
/*----------------------------------------------------------------------------*/
//Row* Pds::pCopyRowArray(i8* pRowArray)
//{
//	_precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
//	ASSERT(m_oRowPix.m_bInitialised);
//
//	Row* pNewRow = pAllotRow();
//	if (! pNewRow) {
//		vTrace("%s: Unable to copy row as the Pds is full\n"_Stz, __func__ );
//		return nullptr;
//	}
//
//	i8 iVal;
//	Stz* szSrcStz;
//
//	i8 nCols = m_oRowPix.m_nColumns;
//	i8 i_Col;
//	for (i_Col = 0; i_Col < nCols; i_Col++) {
//
//		eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
//
//		if (DM_VCH == eDatum) {
//			// vch datum
//
//			if (C_DATUM_NULL == pRowArray[i_Col]) {
//				vDepositColVal(C_DATUM_NULL);
//			}
//			else {
//				ASSERT(m_aStzSet != nullptr);
//				szSrcStz = (Stz*) pRowArray[i_Col];
//				vDepositColVal(szSrcStz, m_oRowPix.m_xaColPix[i_Col].m_uLength);
//			}
//		}
//		else {
//			// all others
//
//			iVal = pRowArray[i_Col];
//			vDepositColVal(iVal);
//		}
//	}
//
//	vReset();
//
//	return pNewRow;
//}

/*----------------------------------------------------------------------------*/
/* Allot space for Stz of fixed size, return dst str addr                     */
/*----------------------------------------------------------------------------*/
//void* Pds::aAllotStzSet(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
//  ASSERT(iStzSetSize());
//  if (nFreeBytes() < iStzSetSize())
//    return nullptr;
//
//  void* aStzSet = (void*) m_xWsp.aAllotTail();
//
//  if(aStzSet == nullptr)
//    return nullptr;
//
//  ASSERT(DoubleWord::bOnDoubleWordBoundary(aStzSet));
//
//  m_iCurStzSetSpace = iStzSetSize();
//  m_aStzSet = aStzSet;
//  return aStzSet;
//}

/*----------------------------------------------------------------------------*/
/* delete this Pds buffer                                                     */
/*----------------------------------------------------------------------------*/
void Pds::vDecidedDelete(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

//  i8 nAcksWereRemaining;
//
//  if (m_oDecider.bDecAndDecide(nAcksWereRemaining)) {
//
//    m_bDeletable = true;

    delete this;
    // dont touch this pointer
//  }
}

/*----------------------------------------------------------------------------*/
/* remove key from in use skip list and move to free list                     */
/*----------------------------------------------------------------------------*/
bool Pds::bDelete(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // TODO: Add an assert to check that all the entries are deleted where

  SkipListBlock* pDeleteEntry = nullptr;
  SkipListBlock* xPriorVec[C_MAX_LVL_LEVELS];

  // Get the prior vector at all levels for this key, i.e
  // the elements at each level of the skip list after which
  // is the key we want to delete
  vPriorVecToKey(pKey, xPriorVec);

  // Iterate this vector for each level
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {

    SkipListBlock* pCursor = xPriorVec[i_Level];

    // Delete the next, if it matches the key
    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);

    if(pNext) {
      // Check if the next key is the same as the key we want to delete
      i8 iCompareRes = m_fpCompare(pNext->aItem(), pKey);
      if (iCompareRes == 0) {
        // Found the key, save its address and update the chain pointers/offsets
        pDeleteEntry = pNext;

        SkipListBlock* pNextNext = pNextSLBlock(pNext, i_Level);
        bool bResult = bSetNextSLBlock(pCursor, pNextNext, i_Level);
				UNUSED(bResult);
        ASSERT(bResult);

        m_iNumEntries[i_Level]--;
        ASSERT(m_iNumEntries[i_Level] >= 0);

        // Update tail offset array if we are removing an element at the end
        u2 uDeletedOffset = uGetOffsetFromBase(pDeleteEntry);
        if(m_auTailOffsets[i_Level] == uDeletedOffset) {
          // Assert the next next is null as this entry being deleted
          // should be the end of the list
          ASSERT(pNextNext == nullptr);
          m_auTailOffsets[i_Level] = uGetOffsetFromBase(pCursor);
        }
      }
    }
  }

  if(pDeleteEntry){
    vMoveToFreeList(pDeleteEntry);
    return true;
  }

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
  if(pHeadFreeList() == nullptr) {
    vTrace("Invalid offset given\n"_Stz);
    return;
  }

  vTrace("| %5s | %6s | %s\n"_Stz, "Level", "# Ents", "Items");

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);

  SkipListBlock* pHead = pHeadFreeList();
  SkipListBlock* pCursor = pNextSLBlock(pHead, 0);

  vTrace("| %5lld | %6lld | "_Stz, 0, m_iFreeListEntries);
  vTrace("Rt-->"_Stz);

  while(pCursor != nullptr) {
    vTrace("(%lld)-->"_Stz, pCursor->iKey());

    pCursor = pNextSLBlock(pCursor, 0);
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
  if(pHeadSL() == nullptr) {
    vTrace("Invalid offset given\n"_Stz);
    return;
  }

  vTrace("| %5s | %6s | %s\n"_Stz, "Level", "# Ents", "Items");

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {
    SkipListBlock* pHead = pHeadSL();
    SkipListBlock* pCursor = pNextSLBlock(pHead, i_Level);

    vTrace("| %5lld | %6lld | "_Stz, i_Level, m_iNumEntries[i_Level]);
    vTrace("Rt-->"_Stz);

    while(pCursor != nullptr) {
//      DoubleKey oDKey;
//      oDKey = *(DoubleKey*)(pCursor->aItemKey());
//      vTrace("%lld(%lld)-->"_Stz, oDKey.iKey_1, oDKey.iKey_2);
      vTrace("(%lld)-->"_Stz, pCursor->iKey());

      pCursor = pNextSLBlock(pCursor, i_Level);
    }
    vTrace("END\n"_Stz);
    vTrace("--------------------------------------------------"
           "--------------------------------------------------\n"_Stz);
  }

  vTrace("=================================================="
         "===================================================\n\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* Get dummy head of in use skip list                                         */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pHeadSL(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(m_uInUseRootOffset != 0);

  return (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);
}

/*----------------------------------------------------------------------------*/
/* Get dummy head of free list                                                */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pHeadFreeList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(m_uFreeListRootOffset != 0);

  return (SkipListBlock*) pGetAddrFromOffset(m_uFreeListRootOffset);
}

/*----------------------------------------------------------------------------*/
/* Insert Key                                                                 */
/*----------------------------------------------------------------------------*/
SkipListBlock* Pds::pInsert(void* pKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  // -------------- Sanity check ----------------
  // Assert the key is greater than the root dummy block key of zero
//  SkipListBlock* pRootCursor = (SkipListBlock*) pGetAddrFromOffset(m_uInUseRootOffset);
//  i8 iCompareRes = m_fpCompare(pRootCursor->aItem(), pKey);
//  ASSERT(iCompareRes == -1);
  // --------------------------------------------

  // try to allocate a block as mini-Heap may be full
  SkipListBlock* pNewSLBlock = aAllotSkipListBlock();
  if(pNewSLBlock == nullptr) {
//    ASSERT(0);
    return nullptr;
  }

  u2 uNewOffset = uGetOffsetFromBase(pNewSLBlock);

  SkipListBlock* xPriorVec[C_MAX_LVL_LEVELS];

  // Get the prior vector at all levels for this key, i.e
  // the elements at each level of the skip list after which
  // we want to insert this key.
  vPriorVecToKey(pKey, xPriorVec);

  // Iterate this vector for each level, and we may or may not
  // insert the key based on a random number
//  u8 uSkipLevels = (RndLib::uInteger() % C_MAX_LVL_LEVELS) + 1;

  for (i8 i_Level = 0; i_Level < C_MAX_LVL_LEVELS; i_Level++) {

//    i8 iRMod = rand() % (i_Level+1);
    i8 iRMod = (i8) (RndLib::uInteger() % (i_Level+1));
    if(iRMod != 0) {
      break;
    }

    SkipListBlock* pCursor = xPriorVec[i_Level];

    // Insert element, update offset values
    SkipListBlock* pNext = pNextSLBlock(pCursor, i_Level);

    if(pNext) {
      // Check if the next key is the same as the key we are inserting
      // Currently we do not allow duplicates
      i8 iCompareRes = m_fpCompare(pNext->aItem(), pKey);
      if (iCompareRes == 0) {
        ASSERT(0);
        // Got duplicate key, error
        vMoveToFreeList(pNewSLBlock);
        vTrace("Pds Skip List attempting to insert a duplicate key\nAborting insert...\n"_Stz);
        return nullptr;
      }
    } else {
      // Next is null, which means we are appending to the end of the skiplist at this level
      // Update the tail elements array
      m_auTailOffsets[i_Level] = uNewOffset;
    }

    pCursor->m_xNode.vNext(i_Level, uNewOffset);
    pNewSLBlock->m_xNode.vNext(i_Level, uGetOffsetFromBase(pNext));

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

  SkipListBlock* pPrev = pHeadSL();
  for( i8 i_Level = C_MAX_LVL_LEVELS -1; i_Level >= 0; i_Level--) {
    if(m_iNumEntries[i_Level] == 0)
      continue;

    SkipListBlock* pCursor = pNextSLBlock(pPrev, i_Level);
    while(pCursor != nullptr) {
      i8 iCompareRes = m_fpCompare(pCursor->aItem(), pKey);

      if(iCompareRes == 0) {
        return pCursor;
      } else if(iCompareRes > 0) {
        // pCursor > pKey
        // move down one level
        break;
      }

      pPrev = pCursor;
      pCursor = pNextSLBlock(pCursor, i_Level);
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

  SkipListBlock* pRootBlock = pHeadSL();

  SkipListBlock* pFirstBlock = pNextSLBlock(pRootBlock, 0);

  ASSERT(pFirstBlock != nullptr);
  ASSERT(pFirstBlock != pRootBlock);

  return pFirstBlock;
}

/*----------------------------------------------------------------------------*/
/* Get the first Row after the root block                                     */
/*----------------------------------------------------------------------------*/
Row* Pds::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  if(m_iNumEntries[0] == 0)
    return nullptr;

  SkipListBlock* pRootBlock = pHeadSL();

  SkipListBlock* pFirstBlock = pNextSLBlock(pRootBlock, 0);

  ASSERT(pFirstBlock != nullptr);
  ASSERT(pFirstBlock != pRootBlock);

  return pFirstBlock->aRow();
}

/*----------------------------------------------------------------------------*/
/* Number of free bytes                                                       */
/*----------------------------------------------------------------------------*/
i8 Pds::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  // Note, does not include free list entries
  return m_xWsp.nFreeBytes();
}

/*----------------------------------------------------------------------------*/
/* Init the RowPix structure                                                  */
/*----------------------------------------------------------------------------*/
void Pds::vInitPix(RowPix* pRowPix)
{
  ASSERT(pRowPix);

  m_oRowPix.vCopyRowPix(pRowPix);

//  i8 nCols = m_oRowPix.m_nColumns;
//  i8 iTotalStzArea = 0;
//
//  // Calculate total size of string area needed per row
//  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
//    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
//    if (DM_VCH == eDatum) {
//      i8 nBytes = m_oRowPix.m_xaColPix[i_Col].m_uLength;
//
//      if (nBytes) {
//        iTotalStzArea += String_d::iBytesToAlloc(nBytes);
//      }
//    }
//  }
//  // Set size of StzSet area
//  vStzSetSize(iTotalStzArea);

  i8 iRowSizeProper = Row::nRowProper(m_oRowPix.m_nColumns);
	UNUSED(iRowSizeProper);
  // Assert the space we are allocating for the row + key is less than or equal to the block size
  ASSERT((iRowSizeProper + (i8)sizeof(m_iSynthRowKey)) <= m_nBlockSize);

}

/*----------------------------------------------------------------------------*/
/* Init the RowPix structure from a streamdef                                 */
/*----------------------------------------------------------------------------*/
void Pds::vInitPix(StreamDef* pStreamDef, bool bAddKeyHash)
{

  pStreamDef->vToRowPix(&m_oRowPix, bAddKeyHash);

//  i8 nCols = m_oRowPix.m_nColumns;
//  i8 iTotalStzArea = 0;
//
//  // Calculate total size of string area needed per row
//  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
//    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
//    if (DM_VCH == eDatum) {
//      i8 nBytes = m_oRowPix.m_xaColPix[i_Col].m_uLength;
//
//      // TODO: Hack, till we get the proper length
//      if (nBytes) {
//        if(nBytes > 800) {
//          iTotalStzArea += String_d::iBytesToAlloc(800);
//          m_oRowPix.m_xaColPix[i_Col].m_uLength = 800;
//        }
//        else
//          iTotalStzArea += String_d::iBytesToAlloc(nBytes);
//      }
//    }
//  }
//  // Set size of StzSet area
//  vStzSetSize(iTotalStzArea);

  i8 iRowSizeProper = Row::nRowProper(m_oRowPix.m_nColumns);
	UNUSED(iRowSizeProper);

  // Assert the space we are allocating for the row + key is less than or equal to the block size
  ASSERT((iRowSizeProper + (i8)sizeof(m_iSynthRowKey)) <= m_nBlockSize);

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
  ASSERT(iLevel >= 0 && iLevel < C_MAX_LVL_LEVELS);
  return m_iNumEntries[iLevel];
}

/*----------------------------------------------------------------------------*/
/* Number of elements at iLevel in this Skip List                             */
/*----------------------------------------------------------------------------*/
Row* Pds::pNextRow(Row* pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  i8 iOffset = sizeof(SkipListNode) + sizeof(i8);

  // Get start addr of the SkipListBlock that this Row resides in
  void* aSLBlockAddr = aSubtract<void*>(pCurr, iOffset);
  ASSERT(DoubleWord::bOnDoubleWordBoundary(aSLBlockAddr));

  SkipListBlock* pSlBlock = (SkipListBlock*) aSLBlockAddr;

  // Get the next block
  SkipListBlock* pNext = pNextSLBlock(pSlBlock, 0);

  if( ! pNext )
    return nullptr;

  return pNext->aRow();
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void Pds::vPrintRows(File* pFilePtr, Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  UNUSED(pFilePtr);

  ASSERT(m_oRowPix.m_bInitialised);

  HV_Record hvRecord(&m_oRowPix);

  PdsCursor* pCursor = new PdsCursor(this);

  while( ! pCursor->bIsCurrNull()) {
    Row* aRowCur = pCursor->pCurrRow();

    hvRecord->vToCsvRecord(aRowCur);
    hvRecord->vPrintAsRow(szLabel);
    pCursor->vNext();
  }
  pCursor->m_pPds = nullptr;
  delete pCursor;
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
  m_nRowStringBytes = 0;
}

/*----------------------------------------------------------------------------*/
/* return address of RowPix                                                   */
/*----------------------------------------------------------------------------*/
RowPix* Pds::pRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);
  return &m_oRowPix;
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
    UNUSED(iCount);

    while(pNext != nullptr) {
      iCount++;
      pNext = pNextSLBlock(pNext, i_Level);
    }
    ASSERT(iCount == m_iNumEntries[i_Level]);
  }

  // Validate free list
  uOffset = m_uFreeListRootOffset;

  SkipListBlock* pCursor = (SkipListBlock*) pGetAddrFromOffset(uOffset);

  SkipListBlock* pNext = pNextSLBlock(pCursor, 0);
  i8 iCount = 0;
  UNUSED(iCount);

  while(pNext != nullptr) {
    iCount++;
    pNext = pNextSLBlock(pNext, 0);
  }
    ASSERT(iCount == m_iFreeListEntries);
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
    ASSERT(pNextFree != nullptr);

    pAllocBlock = pNextFree;

    SkipListBlock* pNextNext = pNextSLBlock(pAllocBlock, 0);

    // Set the dummy root to point to next->next
    // next->next can be null, which is ok
    bool bResult = bSetNextSLBlock(pFreeListDummyRoot, pNextNext, 0);
		UNUSED(bResult);
    ASSERT(bResult == 1);

    m_iFreeListEntries--;
    ASSERT(m_iFreeListEntries >= 0);
  } else {
    pAllocBlock = m_xWsp.aAllotTail();
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

  ASSERT((aAddr1 - aAddr0)%16 == 0);
  i8 iOffset = (aAddr1 - aAddr0)/16; // offset is in multiples of 2 words

  ASSERT((iOffset >= 0) && (iOffset <= iLastUsableOffset()));

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


  SkipListBlock* pCursor = pHeadFreeList();

  SkipListBlock* pNext = pNextSLBlock(pCursor, 0);

  bool bResult = bSetNextSLBlock(pSLBlock, pNext, 0);
	UNUSED(bResult);
  ASSERT(bResult);

  bResult = bSetNextSLBlock(pCursor, pSLBlock, 0);
  ASSERT(bResult);

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
/* compute prior vector for insert/delete key, such that next of PriorVec[0] is      */
/* specifically the point to insert at or delete                              */
/*----------------------------------------------------------------------------*/
void Pds::vPriorVecToKey(void* pKey, SkipListBlock* aPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  SkipListBlock* pCurr = pHeadSL();

  for (i8 i_Level = C_MAX_LVL_LEVELS - 1; i_Level >= 0; i_Level--) {

    if(m_iNumEntries[i_Level] == 0) {
      aPriorVec[i_Level] = pCurr;
      continue;
    }

    SkipListBlock* pNext = pNextSLBlock(pCurr, i_Level);

    // inner loop:
    //   move right along the nodes at this level
    //     until we find
    //       pNext >= pKey at current level
    while(pNext != nullptr) {
      i8 iCompareRes = m_fpCompare(pNext->aItem(), pKey);

      if(iCompareRes >= 0)
        break;

      pCurr = pNext;
      pNext = pNextSLBlock(pCurr, i_Level);
    }

    aPriorVec[i_Level] = pCurr;
  }
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

  ASSERT(m_i_ColCrs != C_UNKNOWN);
  m_i_ColCrs++;
}


/*--------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
void Pds::vDepositColVal(d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(m_i_ColCrs != C_UNKNOWN);
  m_aRowCur->vColVal_flt(m_i_ColCrs,dColVal);

  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
void Pds::vDepositColVal(i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(m_i_ColCrs != C_UNKNOWN);
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
/* deposit (area) + (stz) value into next cell                              */
/*--------------------------------------------------------------------------*/
bool Pds::bDepositColVal(Stz* szColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

	i8 iFreeBytes = nFreeBytes();
	i8 iSrcLen = szColVal->nLength();

  // Ensure we have enough space for 1 byte str + null terminator
  if(iFreeBytes < iSrcLen + 2)
    return false;

  char* azTgt = azDepositString_begin();
  if( ! azTgt)
    return false;

  i8 nLen = StrLib::nCopy(azTgt, iFreeBytes, szColVal->azChar(), iSrcLen);

  vDepositString_end(nLen + 1);
  return true;
}

/*--------------------------------------------------------------------------*/
/* begin deposit of string                                                  */
/*--------------------------------------------------------------------------*/
char* Pds::azDepositString_begin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  ASSERT(m_i_ColCrs != C_UNKNOWN);

//  String_d* szDeposited = new(m_aStzSet) String_d(nSize);

  char* aAddr = (char*)m_xWsp.ayLatterHead();

  if( ! aAddr)
    return nullptr;

  m_aStzSet = (char*) aAddr;

  // TODO: Arjun - Check type conversion with Ray
  i8 i_Ofs = - nDifference(aAddr, m_aRowCur->aColCell(m_i_ColCrs));

  m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

  vAdvanceCursor();


  return aAddr;
}

/*--------------------------------------------------------------------------*/
/* take bytes for deposited string                                          */
/*--------------------------------------------------------------------------*/
void Pds::vDepositString_end(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  m_xWsp.vTakeHead(nSize);
  m_nRowStringBytes += nSize;
}

/*--------------------------------------------------------------------------*/
/* discard current invalid/incomplete row                                   */
/*--------------------------------------------------------------------------*/
void Pds::vDiscardInvalidRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDS,_FOOTER_);

  m_iSynthRowKey--;

  bool bResult = bDelete(&m_iSynthRowKey);

  if (! bResult) {
    vTrace("Pds failed to discard invalid row with key: %lld"_Stz, m_iSynthRowKey);
    return;
  }

  m_aRowCur = nullptr;
  m_i_ColCrs = 0;

  m_xWsp.vGiveHead(m_nRowStringBytes);

  m_nRowStringBytes = 0;
  m_aStzSet = nullptr;
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
