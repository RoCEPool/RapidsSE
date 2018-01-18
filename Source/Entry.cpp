/*============================================================================*/
/* Entry.cpp -- buffer entry                                                  */
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
Entry* DBobjects::_pEntry(void* aAddr)
{
  Entry* pEntry = reinterpret_cast<Entry*>(aAddr);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Entry class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize, Entry* pEntry)
{
  RASSERT(iSize);
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(eCN eClassNo, i8 nThisBytes, i8 nOffsets)
: Item(eClassNo, nThisBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_iTxId      = -1;
  //m_xId
  m_nOffsets   = nOffsets;
  
  _setFooter_();
  
  _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an Entry based on given EntWrk incl Object & Footer */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(EntWrk& xEntWrk)
{
  i8 nOffsets = xEntWrk.nOffsets();
  
  //# ASSERT(nOffsets >= 1);
  
  i8 nKeyBytes = xEntWrk.nKeySize();
  
  i8 nDataBytes = xEntWrk.nDataSize();
  
  i8 nBytes = Entry::nBytesRequired(nOffsets,nKeyBytes,nDataBytes);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry incl Object & Footer                    */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequired(i8 nOffsets, i8 nKeyBytes, i8 nDataBytes)
{
  i8 nOffsetBytes = RightVec::nBytesRequired(nOffsets);

  //# ASSERT(nOffsets >= 1);
  
  i8 nBytes = sizeof(Entry)
               + nOffsetBytes
               + nKeyBytes
               + nDataBytes
               + sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Entry Proper ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
Id& Entry::xId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vId(Id xId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that this entry is within                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* Entry::pEntryBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  bn nBufferNo = m_xId.iBuffer();
//  EntryBuffer* pEntryBuffer =
//    reinterpret_cast<EntryBuffer*>(gpBufferPool->pToBuffer(nBufferNo));

  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(gpBufferPool->pToBuffer(nBufferNo));

  EntryBuffer* pEntryBuffer =
    reinterpret_cast<EntryBuffer*>(pHeapBuffer->pFirstChunk());
  
  ASSERT(pEntryBuffer);
  ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence incl Object & Footer             */
/*----------------------------------------------------------------------------*/
i8 Entry::nSizeEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); 
  
  return nSizeItem();
}

/*----------------------------------------------------------------------------*/
/*-- Row Data ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* Entry::aRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  
  char* aKey = pAdd<char*>(aVblData(),nOffsetBytes);
  // assuming key is CT_INTEGER for hidden index
  
  Row* aRow = pAdd<Row*>(aKey, sizeof(i8));
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get row data address                                                       */
/*----------------------------------------------------------------------------*/
char* Entry::aRowData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);

  char* aKey = pAdd<char*>(aVblData(),nOffsetBytes);
  // assuming key is CT_INTEGER for hidden index
  char* aRow = pAdd<char*>(aKey, sizeof(i8));                        //ALKA-TODO
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get row data size                                                          */
/*----------------------------------------------------------------------------*/
i8 Entry::nRowSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  i8 iRowSize = nSizeItem() - (nOffsetBytes + sizeof(Entry) + sizeof(Footer) + sizeof(i8));
  
  return iRowSize;
}

/*----------------------------------------------------------------------------*/
/*-- Variable data area ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
char* Entry::aVblData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  char* aVblData = pAdd<char*>(this,sizeof(Entry));
  
  return pPoolCheck<char*>(aVblData);
}

/*----------------------------------------------------------------------------*/
/*-- Offsets -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get highest offset                                                         */
/*----------------------------------------------------------------------------*/
on Entry::nHighestOffset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= gpAttachMap->m_nIndexOffsets);
  
  on nHighest(m_nOffsets - 1);
  
  return nHighest;
}

/*----------------------------------------------------------------------------*/
/* get number of offsets                                                      */
/*----------------------------------------------------------------------------*/
i8 Entry::nOffsets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  return m_nOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset for given level                                           */
/*----------------------------------------------------------------------------*/
i8 Entry::iRightOffset(on nOffsetNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gpAttachMap->m_nIndexOffsets);
  
  if (nOffsetNo >= m_nOffsets)
    return C_NIL_OFFSET;
  
  i8 iRight = aRightVec()->iOffset(nOffsetNo);
  
  return iRight;
}

/*----------------------------------------------------------------------------*/
/* set right offset for given level                                           */
/*----------------------------------------------------------------------------*/
void Entry::vRightOffset(on nOffsetNo, i8 iRightOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gpAttachMap->m_nIndexOffsets);
  
  aRightVec()->vOffset(nOffsetNo,iRightOffset);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Key ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get Key                                                                    */
/*----------------------------------------------------------------------------*/
Key* Entry::aKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  
  Key* aKey = pAdd<Key*>(aVblData(),nOffsetBytes);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* replace existing key with given entry wsp                                  */
/*----------------------------------------------------------------------------*/
void Entry::vReplaceKey(EntWrk& xEntWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ASSERT(nSizeItem());
  
  // copy Key
  Key* aEntryKey = this->aKey();
  
  Key* aEntWrkKey = xEntWrk.aKey();
  i8 nKeyBytes = xEntWrk.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWrkKey,nKeyBytes,eClass());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Index -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set entry given entry wsp                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(EntWrk& xEntWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ASSERT(nSizeItem());
  
  // set RowId
  m_xId = xEntWrk.xId();
  
  // set Offsets
  m_nOffsets = xEntWrk.nOffsets();
  
  // set Transaction Id
  m_iTxId = xEntWrk.iTxId();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // copy RightVec
  RightVec* aEntryRightVec = this->aRightVec();
  RightVec* aEntWrkRightVec = xEntWrk.aRightVec();
  i8 nRightBytes = xEntWrk.nRightSize();
  
  MemLib::vCopy(aEntryRightVec,aEntWrkRightVec,nRightBytes,eClass());
 
  // copy Key
  Key* aEntryKey = this->aKey();
  Key* aEntWrkKey = xEntWrk.aKey();
  i8 nKeyBytes = xEntWrk.nKeySize();
  
  ASSERT(nKeyBytes);
  MemLib::vCopy(aEntryKey,aEntWrkKey,nKeyBytes,eClass());
  
  // copy data
  if (xEntWrk.bHasData()) {
    char* aEntryData = this->aRowData();
    char* aEntWrkData = xEntWrk.aData();
    i8 nDataSize = xEntWrk.nDataSize();
    
    ASSERT(nDataSize);
    MemLib::vCopy(aEntryData,aEntWrkData,nDataSize,eClass());
    
    //char* aRow = this->aRow();
    //StrVal* pStrVal = reinterpret_cast<StrVal*>(aRow);
    //vTrace("%s"_Stz, pStrVal->aStr());
  }
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
}

/*----------------------------------------------------------------------------*/
/* set entry given eanother entry                                             */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(Id xId, i8 iKey, Entry* pOldEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ASSERT(nSizeItem());
  
  // set RowId
  m_xId  = xId;
  
  // copy TxId
  m_iTxId = pOldEntry->m_iTxId;
  
  // copy Key
  i8 nKeyBytes = 8;
  Key* aEntryKey = this->aKey();
  
  MemLib::vCopy(aEntryKey,&iKey,nKeyBytes,eClass());
  
  // copy data
  char* aEntryData = this->aRowData();
  i8 nDataSize = pOldEntry->nRowSize();
  
  ASSERT(nDataSize);
  
  //StrVal* pStrVal = new((StrVal*)aEntryData) StrVal((StrVal*)aEntryData);
  //pStrVal->vAssign(sRow.aStr(),nDataSize);
  MemLib::vCopy(aEntryData,pOldEntry->aRowData(),nDataSize,eClass());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iBase, Index* pIndex, bool bShowKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  Key* aKey = this->aKey();
  i8 iKey;
  
  if (m_xId.iBuffer() == 0) {
    ASSERT(m_xId.iRow() == 0);
    iKey = -1;
  }
  else {
    DtmLib::nCopy(DM_INT,&iKey,aKey,nullptr);
  }
  
  vTrace("Entry %p: Size: %llu Offsets: %llu "_Stz,
         this,
         nSizeEntry(),
         m_nOffsets
         );
  
  vTrace("Id %3lli.%03lli "_Stz,
         m_xId.iBuffer(),
         m_xId.iRow()
         );
  
  vTrace("Tx %lld.%03lld "_Stz,
         Tx::iTxNode(m_iTxId),
         Tx::iTxNo(m_iTxId)
         );
  
  if (bShowKey)
    aKey->vShow(pIndex->pIdxColVec());

  if (pIndex->bIsHidden()) {
    char* pRow = aRowData();
    if (nRowSize() > 0)
      RowData::vShow((Stz*)aRowData(), pIndex->pTable()->pColumnVec());
  }
  
  for (on nRight = 0_on; nRight < m_nOffsets; nRight++) {
    
    vTrace(" Offset(%lld): %lld "_Stz,
           *nRight,
           iRightOffset(nRight) == C_NIL_OFFSET ?
           C_NIL_OFFSET :
           iBase + iRightOffset(nRight) + nSizeEntry()
           );
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Row ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
//  StrVal* pStr = (StrVal*)aVblData();
//  
//  vTrace("Entry %p: ThisBytes %lld %s (%lld bytes) %s\n"_Stz,
//         this,
//         //m_iTxId,
//         m_nThisBytes,
//         aVblData(),
//         StrLib::iSize(aVblData()),
//         pStr->s()
//         );
  
  //StrVal* pStrVal = reinterpret_cast<StrVal*>(aRow());
  
  vTrace("Entry %p: ThisBytes %lld %p (%lld bytes) \n"_Stz,
         this,
         //m_iTxId,
         nSizeItem(),
         aRowData(),
         nRowSize()
         );
}

/*----------------------------------------------------------------------------*/
/* extract an index key into KeyWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void Entry::vExtractIndexKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  ValWrk xValWrk;
  
  // VCH cannot be > 256 bytes
  s4K s;                                                       //ALKA_ROWFORMAT
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get the column value from the row for the iColumnNo
    char* pDst = (char*)RowData::aColumnDatum(aRowData(), iColumnNo);
    
    switch(pColumn->eDatumType()) {
        
      case DM_BOL: {
        Bol xBol = *((Bol*)pDst);
        xValWrk = xBol;
        break;
      }
        
      case DM_DTE: {
        Dte xDte = *((Dte*)pDst);
        xValWrk = xDte;
        break;
      }
        
      case DM_BCD: {
        Bcd xBcd = *((Bcd*)pDst);
        xValWrk = xBcd;
        break;
      }
        
      case DM_FLT: {
        Flt xFlt = *((Flt*)pDst);
        xValWrk = xFlt.dAsD8();
        break;
      }
        
      case DM_INT: {
        Int xInt = *((Int*)pDst);
        xValWrk = xInt.iAsI8();
        break;
      }
        
      case DM_IVL: {
        Ivl xIvl = *((Ivl*)pDst);
        xValWrk = xIvl;
        break;
      }
        
      case DM_TME: {
        Tme xTme = *((Tme*)pDst);
        //bool b = Tme::bExtract(pDst,&xTme,iLength,pColumn);
        //ASSERT(b);
        xValWrk = xTme;
        break;
      }
        
      case DM_TSP: {
        Tsp xTsp = *((Tsp*)pDst);
        //bool b = Tsp::bExtract(pDst,&xTsp,iLength,pColumn);
        //ASSERT(b);
        xValWrk = xTsp;
        break;
      }
        
      case DM_VBN: {
        NEVER_REACH;
        break;
      }
        
      case DM_VCH: {
        s = (Stz*)pDst;
        xValWrk = s;
        break;
      }
        
      default:
        NEVER_REACH;
    }
    
    // add the column value to the key
    xKeyWrk.vAddDatumInteger(xValWrk, pColumn);
  }
  
}

/*----------------------------------------------------------------------------*/
/* extract a sort key into keyWrk from this row using given index columns     */
/*----------------------------------------------------------------------------*/
void Entry::vExtractSortKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  w8 wColumnWord;
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get address of the column datum from the row for the iColumnNo
    char* aColumnDatum = RowData::aColumnDatum(aRowData(),iColumnNo);
    
    // convert column datum into column word
    switch (pColumn->eDatumType()) {
      
      case Datums::DM_BOL:
        wColumnWord = (*((Bol*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_DTE:
        wColumnWord = (*((Dte*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_BCD:
        wColumnWord = (*((Bcd*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_FLT:
        MemLib::vCopy(&wColumnWord,aColumnDatum,sizeof(w8),CN_ENTRY);
        break;
      
      case Datums::DM_INT:
        wColumnWord = (*((Int*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_IVL:
        wColumnWord = (*((Ivl*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_TME:
        wColumnWord = (*((Tme*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_TSP:
        wColumnWord = (*((Tsp*)aColumnDatum)).iAsI8();
        break;
      
      case Datums::DM_VBN:
        NEVER_REACH;
        break;
      
      case Datums::DM_VCH:
        wColumnWord = (w8)aColumnDatum;
        break;
      
      default:
      NEVER_REACH;
    }
    
    // add the column word to the key
    xKeyWrk.vAddWord(wColumnWord);
  }
  
}

/*----------------------------------------------------------------------------*/
/* extract a sort key into keyWrk from this row using given index columns     */
/*----------------------------------------------------------------------------*/
char* Entry::pExtractSortKey(char* aElement, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  w8 wColumnWord;
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get address of the column datum from the row for the iColumnNo
    char* aColumnDatum = RowData::aColumnDatum(aRowData(),iColumnNo);
    
    // convert column datum into column word
    switch (pColumn->eDatumType()) {
        
      case Datums::DM_BOL:
        wColumnWord = (*((Bol*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_DTE:
        wColumnWord = (*((Dte*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_BCD:
        wColumnWord = (*((Bcd*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_FLT:
        MemLib::vCopy(&wColumnWord,aColumnDatum,sizeof(w8),CN_ENTRY);
        break;
        
      case Datums::DM_INT:
        wColumnWord = (*((Int*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_IVL:
        wColumnWord = (*((Ivl*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_TME:
        wColumnWord = (*((Tme*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_TSP:
        wColumnWord = (*((Tsp*)aColumnDatum)).iAsI8();
        break;
        
      case Datums::DM_VBN:
        NEVER_REACH;
        break;
        
      case Datums::DM_VCH:
        wColumnWord = (w8)aColumnDatum;
        break;
        
      default:
        NEVER_REACH;
    }
    
    // add the column word to the element
    MemLib::vCopy(aElement, &wColumnWord, sizeof(w8), CN_TABLE);
    aElement = pAdd<char*>(aElement,sizeof(w8));
  }
  
  return aElement;
}

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 Entry::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vTxId(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bufferno from id of key                                                */
/*----------------------------------------------------------------------------*/
bn Entry::nBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  bn nBufferNo = m_xId.iBuffer();
  
  return nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get the RightVec for this Entry                                            */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_); 
  
  RightVec* aRightVec = _aRightVec(aVblData());
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of footer                                                      */
/*----------------------------------------------------------------------------*/
Footer* Entry::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iFooterOffset = nSizeItem() - sizeof(Footer);
  
  Footer* aFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
