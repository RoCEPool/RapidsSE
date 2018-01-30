/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* DBobjects::_pEntryBuffer_v2(void* aAddr)
{
  EntryBuffer_v2* pEntryBuffer = reinterpret_cast<EntryBuffer_v2*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v2::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v2::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2::EntryBuffer_v2(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability  = BD_PERMANENT;
  m_bIsLast      = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex       = nullptr;
  m_nBufferLevel = 0;
  m_nPartitionNo = -1;
  
  m_pEntryOffset2 = reinterpret_cast<Offset*>(aAllotSpace(sizeof(Offset) * C_OFFSET_2));
  m_pEntryOffset1 = reinterpret_cast<Offset*>(aAllotSpace(sizeof(Offset) * C_OFFSET_1));
  m_pEntryOffset0 = reinterpret_cast<i4*>(aAllotSpace(sizeof(i4) * C_OFFSET_0));
  
  m_nEntryOffset2 = 0;
  m_nEntryOffset1 = 0;
  
  m_pEntries = _pEntry(m_xWsp.aLatter());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2::~EntryBuffer_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}



/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer_v2::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v2::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vHigherBuffer(EntryBuffer_v2* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherBuffer);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vNextBuffer(EntryBuffer_v2* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v2* EntryBuffer_v2::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vPrevBuffer(EntryBuffer_v2* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vBufferLevel(i8 nBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nBufferLevel = nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v2::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer_v2::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vPartitionNo(i8 nPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nPartitionNo = nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v2::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pAllotEntryBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Entry* pEntry = reinterpret_cast<Entry*>(pAllotItem(nBytes));
  if ( ! pEntry )
    return nullptr;
  
  new(pEntry) Entry(CN_ENTRY,nBytes,0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v2::bAppendEntry_v2(EntWrk_v2& xRowEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ASSERT(nBufferLevel() == 0);
  
  i8 nOffsets =
    Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1,C_FLAT_FREQ) + 1;
  xRowEntry.vOffsets(nOffsets);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has enough space
  
  i8 nEntryBytes = Entry::nBytesRequired(xRowEntry);
  
  if ( ! bHasSpace(nEntryBytes) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id xRowId(*nBufferNo, nRowNo);
  
  xRowEntry.vId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate space in this buffer
  
  Entry* pBufferEntry = pAllotEntryBytes(nEntryBytes);
  ASSERT(pBufferEntry)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the buffer entry
  
  pBufferEntry->vSetEntry(xRowEntry);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set entry offsets
  
  i8 iEntryOffset = iToOffsetAbs(pBufferEntry);
  m_pEntryOffset0[nRowNo] = iEntryOffset;
  
  if (nOffsets >= 2) {
    Offset* pOffset1 = &m_pEntryOffset1[m_nEntryOffset1];
    pOffset1->m_iEntryNo = nRowNo;
    pOffset1->m_iOffset = iEntryOffset;
    m_nEntryOffset1++;
  }
  
  if (nOffsets == 3) {
    Offset* pOffset2 = &m_pEntryOffset2[m_nEntryOffset2];
    pOffset2->m_iEntryNo = nRowNo;
    pOffset2->m_iOffset = iEntryOffset;
    m_nEntryOffset2++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* return number of index buffers under this buffer                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // the location of the possible first entry set in constructor
  ASSERT(m_pEntries);
  
  // first possible first entry
  Entry* pFirst = m_pEntries;
  
  // check for valid entry
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  if (pFirst < pLatter)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v2::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  if (gpAttachMap->m_bIndexSpace) {
    // has space calculation based on free space
    if (nFree < nBytes) {
      //vTrace("EntryBuffer full with %lld bytes left\n"_Stz,nFree);
      return false;
    }
    else
      return true;
  }
  else {
    // has space calculation based on no of enteries
    if (m_nItems > gpAttachMap->m_nIndexEntries) {
      vTrace("EntryBuffer full with %lld items\n"_Stz,m_nItems);
      return false;
    }
    else
      return true;
  }
}


/*----------------------------------------------------------------------------*/
/* create a new next buffer                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v2::pNextEntry(Entry* pCurrent)
{
  // get the size of given entry
  i8 nBytes = pCurrent->nSizeEntry();
  
  // compute the address of possible next entry
  Entry* pNext = pAdd<Entry*>(pCurrent,nBytes);
  
  // check for valid entry
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  if (pNext < pLatter)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position at entry which is prior to key                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove given entry                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v2::vShow(void)
{
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString(((EntryBuffer_v2*)m_pHigherBuffer)->m_nBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("EntryBuffer %p (buffer %lld): %s index %s partition %lld level L%lld "
         "HigerBuffer %p (L%s) entries %lld prevBuffer %p nextBuffer %p \n"_Stz,
         this,
         *nBufferNo(),
         aBD[m_eDurability],
         m_pIndex->aIndexName(),
         m_nPartitionNo,
         m_nBufferLevel,
         m_pHigherBuffer,
         sHigherBuffer.aStr(),
         m_nItems,
         m_pPrevBuffer,
         m_pNextBuffer);
  
  vTrace("Entry Offsets 0:\n"_Stz);
  for (i8 iEntry = 0; iEntry < m_nItems; iEntry++)
    vTrace("  [%2lld]: %lld\n"_Stz,iEntry,m_pEntryOffset0[iEntry]);

  vTrace("Entry Offsets 1:\n"_Stz);
  for (i8 iEntry = 0; iEntry < m_nEntryOffset1; iEntry++)
    vTrace("  [%2lld]: %lld %lld\n"_Stz,
           iEntry,m_pEntryOffset1[iEntry].m_iEntryNo,
                  m_pEntryOffset1[iEntry].m_iOffset);
  
  vTrace("Entry Offsets 2:\n"_Stz);
  for (i8 iEntry = 0; iEntry < m_nEntryOffset2; iEntry++)
    vTrace("  [%2lld]: %lld %lld\n"_Stz,
           iEntry,m_pEntryOffset2[iEntry].m_iEntryNo,
                  m_pEntryOffset2[iEntry].m_iOffset);
  
  vTrace("\n"_Stz);
  vTrace("Entries:\n"_Stz);
  for (Entry* pEntry = pFirstEntry(); pEntry != nullptr; pEntry = pNextEntry(pEntry)) {
    vTrace("%8lld: "_Stz,nSub(pEntry,m_xWsp.aArea()));
    i8 iOffset = iToOffsetAbs(pEntry);
    pEntry->vShow_v2(iOffset,m_pIndex,true);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer plus a new buffer         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* verify the structure of this buffer                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compact this buffer                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy entries from fromBuffer starting at fromEntry to this buffer          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the given prior vector                                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fix the higher level buffers                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the prior vector of given entry                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given entry             */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v2::iToOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntryBuffer_v2::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Unit Test ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void DBobjects::vEntryBuffer_v2_UnitTest(Table* pTable)
{
  
//  for (i8 iInt = 0; iInt < 1000; iInt++) {
//    i8 nOffsets = Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1, C_FLAT_FREQ);
//    vTrace("nOffsets %lld\n"_Stz,nOffsets);
//  }
  
  Index* pHiddenIndex = pTable->pHiddenIndex();
  ColumnVec* pColVec = pTable->pColumnVec();
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
 
  EntryBuffer_v2* pEntryBuffer_v2 = new EntryBuffer_v2;
  pEntryBuffer_v2->vIndex(pHiddenIndex);
  pEntryBuffer_v2->vPartitionNo(0);
  
  s256 sRawData;
  EntWrk_v2 xEntWrk_v2;
  s256 sRowData;
  i8 iRowLength;
  bool bBlankLine;
  bool bFormatted;
  bool bAppended;
  i8 iRawLength;
  
  for (i8 iInt = 0; iInt < 10; iInt++) {
    sprintf(sRawData.aChar(),"%lld,%s,1900-01-01 00:00:%lld",
            iInt,IntLib::sAsText(iInt),iInt);
    vTrace("Record: %s\n"_Stz,sRawData.aChar());
    
    char* aData = xEntWrk_v2.aData_v2();
    iRawLength  = sRawData.iLength();
    bFormatted  = Row::bFormatRow(pColVec,pFormatters,
                                  sRawData,iRawLength,
                                  (Stz*)aData,iRowLength,
                                  bBlankLine);
    xEntWrk_v2.vDataSize(iRowLength);
    ((Row*)aData)->vDump(pColVec);
    
    xEntWrk_v2.vTxId(0);
    xEntWrk_v2.vHasData(true);
    pEntryBuffer_v2->bAppendEntry_v2(xEntWrk_v2);
  }
  
  pEntryBuffer_v2->vShow();
  
  delete pEntryBuffer_v2;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
