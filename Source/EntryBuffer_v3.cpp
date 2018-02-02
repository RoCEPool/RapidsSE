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
EntryBuffer_v3* DBobjects::_pEntryBuffer_v3(void* aAddr)
{
  EntryBuffer_v3* pEntryBuffer = reinterpret_cast<EntryBuffer_v3*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v3::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v3::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3::EntryBuffer_v3(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability   = BD_PERMANENT;
  m_bIsLast       = false;
  
  m_pHigherBuffer = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_nBufferLevel  = 0;
  m_nPartitionNo  = -1;
  
  for (i4 i_Entry = 0; i_Entry < (C_ENTRIES_0_V3 - 1); i_Entry++)
    m_iEntryOffset0[i_Entry] = C_ENTRY_NONE_V3;
  
  m_pHeadEntry = nullptr;
  m_pFirstEntry = nullptr;
  m_pFirstFree  = nullptr;
  
  m_iFreeOffsetHead = C_ENTRY_NONE_V3;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3::~EntryBuffer_v3(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer_v3::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v3::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vHigherBuffer(EntryBuffer_v3* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherBuffer);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vNextBuffer(EntryBuffer_v3* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v3* EntryBuffer_v3::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vPrevBuffer(EntryBuffer_v3* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer level                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vBufferLevel(i8 nBufferLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nBufferLevel = nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v3::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get first entry                                                            */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstEntry;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
FreeEntry* EntryBuffer_v3::pFirstFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pFirstFree;
}

/*----------------------------------------------------------------------------*/
/* get head entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pHeadEntry;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer_v3::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vPartitionNo(i8 nPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nPartitionNo = nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v3::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given entry to free list                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v3::pAllotEntryBytes(i8 nBytes)
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
/* attempt to append a new row entry into this buffer                         */
/*----------------------------------------------------------------------------*/

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
/* set up the initial offset free chains                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v3::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (m_nItems >= gpAttachMap->m_nIndexEntries)
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on free space
    if (nFree < nBytes)
      return false;
    else
      return true;
  }
  
}

/*----------------------------------------------------------------------------*/
/* attempt to insert a new key entry into this buffer                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create a new next buffer                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition on entry prior to key                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove given entry                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove entry from entry offset arrays                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v3::vShow(void)
{
  /*
  s256 sHigherBuffer;
  if (m_pHigherBuffer)
    StrLib::wToString(((EntryBuffer_v2*)m_pHigherBuffer)->m_nBufferLevel,
                      sHigherBuffer.aChar());
  else
    sHigherBuffer = "..."_Stz;
  
  vTrace("\n"_Stz);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  vTrace("EntryBuffer %p (buffer %lld): %s index %s partition %lld level L%lld\n"
         "  HigherBuffer %p (L%s) entries %lld prevBuffer %p nextBuffer %p\n"
         "  ReclaimSpace: %lld FirstFree: %p LastFree %p\n"
         "  HeadEntry2: %lld HeadEntry1: %lld HeadEntry0: %lld \n"
         "  HeadFree2: %lld HeadFree1: %lld HeadFree0: %lld \n"_Stz,
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
         m_pNextBuffer,
         m_nReclaimSpace,
         m_pFirstFree,
         m_pLastFree,
         m_iHeadEntry2,
         m_iHeadEntry1,
         m_iHeadEntry0,
         m_iHeadFree2,
         m_iHeadFree1,
         m_iHeadFree0);
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
 
  vTrace("Entry Offsets 0:\n"_Stz);
//  for (i8 i_Entry = 0; i_Entry < m_nItems; i_Entry++)
//    vTrace("  [%2lld]: %lld\n"_Stz,i_Entry,m_pEntryOffset0[i_Entry]);
  if (m_iHeadEntry0 != C_ENTRY_NONE) {
    for (i8 i_Entry = 0; i_Entry < m_nItems; i_Entry++)
      vTrace("  [%2lld]: %lld %lld %lld\n"_Stz,
             i_Entry,m_pEntryOffset0[i_Entry].m_iEntryNo,
             m_pEntryOffset0[i_Entry].m_iNextEntry == C_ENTRY_NONE ?
             -1 : m_pEntryOffset0[i_Entry].m_iNextEntry,
                     m_pEntryOffset0[i_Entry].m_iOffset);
  }
  if (m_iHeadEntry1 != C_ENTRY_NONE) {
    vTrace("Entry Offsets 1:\n"_Stz);
    for (i8 i_Entry = 0; i_Entry < m_nEntryOffset1; i_Entry++)
      vTrace("  [%2lld]: %lld %lld %lld\n"_Stz,
             i_Entry,m_pEntryOffset1[i_Entry].m_iEntryNo,
             m_pEntryOffset1[i_Entry].m_iNextEntry == C_ENTRY_NONE ?
                -1 : m_pEntryOffset1[i_Entry].m_iNextEntry,
                     m_pEntryOffset1[i_Entry].m_iOffset);
  }
  if (m_iHeadEntry2 != C_ENTRY_NONE) {
    vTrace("Entry Offsets 2:\n"_Stz);
    for (i8 i_Entry = 0; i_Entry < m_nEntryOffset2; i_Entry++)
      vTrace("  [%2lld]: %lld %lld %lld\n"_Stz,
             i_Entry,m_pEntryOffset2[i_Entry].m_iEntryNo,
             m_pEntryOffset2[i_Entry].m_iNextEntry == C_ENTRY_NONE ?
             -1 : m_pEntryOffset2[i_Entry].m_iNextEntry,
                     m_pEntryOffset2[i_Entry].m_iOffset);
  }
  
  vTrace("\n"_Stz);
  vTrace("Entries:\n"_Stz);
  i8 iEntry = 0;
  for (Entry* pEntry = pFirstEntry();
       pEntry != nullptr;
       pEntry = pNextEntry(pEntry)) {
    vTrace("%4lld %8lld: "_Stz,iEntry, nSub(pEntry,m_xWsp.aArea()));
    i8 iOffset = iToOffsetAbs(pEntry);
    pEntry->vShow_v2(iOffset,m_pIndex,true);
    iEntry++;
  }

  if ( ! m_pIndex->bIsHidden()) {
    vTrace("\n"_Stz);
    vTrace("Ordered Entries:\n"_Stz);
    i8 iIndex = m_iHeadEntry0;
    while (iIndex != C_ENTRY_NONE) {
      Entry* pEntry = pGetEntry(m_pEntryOffset0[iIndex].m_iEntryNo, 0_ln);
      vTrace("%4lld %8lld: "_Stz,iIndex, nSub(pEntry,m_xWsp.aArea()));
      i8 iOffset = iToOffsetAbs(pEntry);
      pEntry->vShow_v2(iOffset,m_pIndex,true);
      iIndex = m_pEntryOffset0[iIndex].m_iNextEntry;
    }
  }
  
  if (gpAttachMap->m_nIndexEntries) {
    if ( ! m_pIndex->bIsHidden() ) {
      vTrace("\n"_Stz);
      vTrace("FreeChains2:\n"_Stz);
      for (i4 i_Entry = m_iHeadFree2;
           i_Entry != C_ENTRY_NONE;
           i_Entry = - m_pEntryOffset2[i_Entry].m_iNextEntry) {
        vTrace("  %lld: %lld %lld %lld\n"_Stz,
               i_Entry,m_pEntryOffset2[i_Entry].m_iEntryNo,
                       m_pEntryOffset2[i_Entry].m_iNextEntry,
                       m_pEntryOffset2[i_Entry].m_iOffset);
      }
      vTrace("FreeChains1:\n"_Stz);
      for (i4 i_Entry = m_iHeadFree1;
           i_Entry != C_ENTRY_NONE;
           i_Entry = - m_pEntryOffset1[i_Entry].m_iNextEntry) {
        vTrace("  %lld: %lld %lld %lld\n"_Stz,
               i_Entry,m_pEntryOffset1[i_Entry].m_iEntryNo,
                       m_pEntryOffset1[i_Entry].m_iNextEntry,
                       m_pEntryOffset1[i_Entry].m_iOffset);
      }
    }
    vTrace("FreeChains0:\n"_Stz);
    for (i4 i_Entry = m_iHeadFree0;
         i_Entry != C_ENTRY_NONE;
         i_Entry = - m_pEntryOffset0[i_Entry].m_iNextEntry) {
      vTrace("  %lld: %lld %lld %lld\n"_Stz,
             i_Entry,m_pEntryOffset0[i_Entry].m_iEntryNo,
                     m_pEntryOffset0[i_Entry].m_iNextEntry,
                     m_pEntryOffset0[i_Entry].m_iOffset);
    }
  }
  
  vTrace("\n"_Stz);
  vTrace("FreeEntries:\n"_Stz);
  for (FreeEntry* pFreeEntry = pFirstFree();
       pFreeEntry != nullptr;
       pFreeEntry = pFreeEntry->pNextFree()) {
    vTrace("  %p bytes: %lld next: %p prev: %p\n"_Stz,
           pFreeEntry,pFreeEntry->nSizeItem(),
           pFreeEntry->pNextFree(),pFreeEntry->pPrevFree());
  }
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
   */
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
/* dump the prior vector                                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fix the higher level buffers                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a free entry (or C_ENTRY_NONE if none)                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next entry offset array                                     */
/*----------------------------------------------------------------------------*/
NEO* EntryBuffer_v3::aNEO(void)
{
  Footer* aFooter = this->aFooter();
  
  NEO* aNEO = pAdd<NEO*>(aFooter,sizeof(Footer));
  
  return aNEO;
}

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
i8 EntryBuffer_v3::iToOffsetAbs(Entry* pEntry)
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
Footer* EntryBuffer_v3::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Unit Test ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void DBobjects::vEntryBuffer_v3_UnitTest(Table* pTable)
{
  /*
//  for (i8 iInt = 0; iInt < 1000; iInt++) {
//    i8 nOffsets = Random::iLogLevel(gpAttachMap->m_nIndexOffsets+1, C_FLAT_FREQ);
//    vTrace("nOffsets %lld\n"_Stz,nOffsets);
//  }
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  Index* pIndex1 = pIdxVec->pGet(1);
  IdxColVec* pIdxColVec1 = pIndex1->pIdxColVec();
  
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
 
  EntryBuffer_v3* pFirstBuffer = new EntryBuffer_v3;
  EntryBuffer_v3* pCurrentBuffer = pFirstBuffer;
  pCurrentBuffer->vIndex(pHiddenIndex);
  pCurrentBuffer->vPartitionNo(0);
  //pCurrentBuffer->vShow();

  EntryBuffer_v3* pFirstIndexBuffer = new EntryBuffer_v3;
  EntryBuffer_v3* pCurrentIndexBuffer = pFirstIndexBuffer;
  pCurrentIndexBuffer->vIndex(pIndex1);
  pCurrentIndexBuffer->vPartitionNo(0);
  pCurrentIndexBuffer->vShow();
  
  s256 sRecData;
  i8 iRecLength;
  
  RowWrk_v2 xRowEntWrk;
  i8 iRowLength;
  
  bool bBlankLine;
  bool bFormatted;
  bool bAppended;
  
  KeyWrk_v2 xKeyWrk;
  
  // keys in ascending order
  //for (i8 iInt = 0; iInt <= 9; iInt++) {
  //  i8 iKey = iInt;
    
  // keys in descending order
  //for (i8 iInt = 9; iInt >= 0; iInt--) {
  //  i8 iKey = iInt;
    
  // keys in random order
  for (i8 iInt = 0; iInt <= 20; iInt++) {
    i8 iKey = Random::iRandom(20);
  
    sprintf(sRecData.aChar(),"%lld,%s,1900-01-01 00:00:%lld",
            iKey,IntLib::sAsText(iInt),iInt);
    //vTrace("Record: %s\n"_Stz,sRawData.aChar());
    
    char* aRowData = xRowEntWrk.aData_v2();
    iRecLength = sRecData.iLength();
    bFormatted = Row::bFormatRow(pColVec,pFormatters,
                                 sRecData,iRecLength,
                                 (Stz*)aRowData,iRowLength,
                                 bBlankLine);
    xRowEntWrk.vDataSize(iRowLength);
    ((Row*)aRowData)->vDump(pColVec);
    
    xRowEntWrk.vTxId(0);
    //pCurrentBuffer->vShow();
    Entry* pRowEntry = pCurrentBuffer->pAppendRow_v2(xRowEntWrk);
    if ( ! pRowEntry ) {
      pCurrentBuffer = pCurrentBuffer->pNewNextBuffer();
      pRowEntry = pCurrentBuffer->pAppendRow_v2(xRowEntWrk);
      ASSERT(pRowEntry);
    }
    //pCurrentBuffer->vShow();

    //pCurrentBuffer->vAddFree(pRowEntry);
    
    xKeyWrk.vReset();
    pRowEntry->vExtractIndexKey_v2(xKeyWrk,pIdxColVec1);
    xKeyWrk.vShow(pIdxColVec1);
    
    Entry* pKeyEntry = pCurrentIndexBuffer->pInsertKey_v2(xKeyWrk);
    if ( ! pKeyEntry ) {
      pCurrentIndexBuffer = pCurrentIndexBuffer->pNewNextBuffer();
      pKeyEntry = pCurrentIndexBuffer->pInsertKey_v2(xKeyWrk);
      ASSERT(pKeyEntry);
    }
    pCurrentIndexBuffer->vShow();
  }

  for (pCurrentBuffer = pFirstBuffer;
       pCurrentBuffer;
       pCurrentBuffer = pCurrentBuffer->pNextBuffer())
    pCurrentBuffer->vShow();

  for (pCurrentIndexBuffer = pFirstIndexBuffer;
       pCurrentIndexBuffer;
       pCurrentIndexBuffer = pCurrentIndexBuffer->pNextBuffer())
    pCurrentIndexBuffer->vShow();
   */
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
