/*============================================================================*/
/* Entry.cpp -- buffer entry                                                  */
/*============================================================================*/

#include "DBobjects.h"

extern i8 gnIndexOffsets;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Entry* DBobjects::_pBufferItem(void* aAddr)
{
  Entry* pBufferItem = reinterpret_cast<Entry*>(aAddr);
  
  return pBufferItem;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Entry* DBobjects::_pEntry(void* aAddr)
{
  Entry* pEntry = reinterpret_cast<Entry*>(aAddr);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Entry* DBobjects::_aRow(void* aAddr)
{
  Entry* aRow = reinterpret_cast<Entry*>(aAddr);
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Entry class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize, Entry* pBufferItem)
{
  RASSERT(iSize);
  return pBufferItem;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(EntWsp& xEntWsp)
: Object(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  _setFooter_();
  
  vSetEntry(xEntWsp);
  
  _checkInitialised_(CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(i8 nBytes, i8 iOffsets)                   //! should we pass EntWsp?
: Object(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  // initialise bytes
  m_nThisBytes = nBytes;
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() && 0 == m_xId.iRow());
  
  // initialise offsets
  m_iOffsets = iOffsets;
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  // entry data will be initialised by vSetEntry       //! Is this correct form?
  
  _setFooter_();
  
  _checkInitialised_(CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
    
  m_nThisBytes = 0;
  m_iOffsets = 0;
  
  //_setFooter_();
  
  _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(eCN eClassNo, i8 nThisBytes)
: Object(CN_ROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //m_iTxId      = -1;
  m_nThisBytes = nThisBytes;
  
  _setFooter_();
  
  _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an Entry based on given EntWsp incl Object & Footer */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequiredEntry(EntWsp& xEntWsp)
{
  i8 nOffsets = xEntWsp.nOffsets();
  
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  i8 nBytes = Entry::nBytesRequiredEntry(nOffsets,nKeyBytes);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry incl Object & Footer                    */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequiredEntry(i8 nOffsets, i8 nKeyBytes)
{
  i8 nEntryDataBytes = EntryData::nBytesRequired(nOffsets,nKeyBytes);
  
  i8 nBytes = sizeof(Entry) + nEntryDataBytes + sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get bytes required for row                                                 */
/*----------------------------------------------------------------------------*/
i8 Entry::nBytesRequiredRow(i8 iDataBytes)
{
  i8 nBytes = sizeof(Entry) + iDataBytes + sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Offsets -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get highest offset                                                         */
/*----------------------------------------------------------------------------*/
on Entry::nHighestOffset(void)
{
  ASSERT(m_iOffsets >= 0 && m_iOffsets <= gnIndexOffsets);
  
  on nHighest(m_iOffsets - 1);
  
  return nHighest;
}

/*----------------------------------------------------------------------------*/
/* get nOffsets                                                               */
/*----------------------------------------------------------------------------*/
i8 Entry::nOffsets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_iOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset for given level                                           */
/*----------------------------------------------------------------------------*/
i8 Entry::iRightOffset(on nOffsetNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gnIndexOffsets);
  
  if (nOffsetNo >= m_iOffsets)
    return C_NIL_OFFSET;
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_iOffsets);
  
  i8 iRight = aRightVec->iOffset(nOffsetNo);
  
  return iRight;
}

/*----------------------------------------------------------------------------*/
/* set right offset for given level                                           */
/*----------------------------------------------------------------------------*/
void Entry::vRightOffset(on nOffsetNo, i8 iRightOffset)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gnIndexOffsets);
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_iOffsets);
  
  aRightVec->vOffset(nOffsetNo,iRightOffset);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get the RightVec for this Entry                                            */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = this->aEntryData();
  
  RightVec* aRightVec = aEntryData->aRightVec(m_iOffsets);
  
  return aRightVec;
}

/*----------------------------------------------------------------------------*/
/*-- Entry -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get EntryData                                                              */
/*----------------------------------------------------------------------------*/
EntryData* Entry::aEntryData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* aEntryData = pAdd<EntryData*>(this,sizeof(Entry));
  
  return aEntryData;
}

/*----------------------------------------------------------------------------*/
/* get Key                                                                    */
/*----------------------------------------------------------------------------*/
Key* Entry::aKey(void)
{
  _precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  EntryData* pEntryData = this->aEntryData();
  
  Key* aKey = pEntryData->aKey(m_iOffsets);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* set entry given entry wsp                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(m_nThisBytes);
  
  // set RowId
  m_xId  = xEntWsp.xId();
  
  // set Offsets
  m_iOffsets = xEntWsp.nOffsets();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // copy RightVec
  RightVec* aEntryRightVec = this->aRightVec();
  RightVec* aEntWspRightVec = xEntWsp.aRightVec();
  i8 nRightBytes = xEntWsp.nRightSize();
  
  MemLib::vCopy(aEntryRightVec,aEntWspRightVec,nRightBytes);
  
  // copy Key
  Key* aEntryKey = this->aKey();
  Key* aEntWspKey = xEntWsp.aKey();
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWspKey,nKeyBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iOffset, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* aKey = this->aKey();
  i8 iKey;
  
  if (m_xId.iBuffer() == 0) {
    ASSERT(m_xId.iRow() == 0);
    iKey = -1;
  }
  else {
    ValLib::nCopyDatum(CN_INTVAL,&iKey,aKey);
  }
  
  vTrace("Entry %p: Size: %llu Offsets: %llu "_Str,
         this,
         nSizeEntry(),
         m_iOffsets
         );
  
  vTrace("Id %3lli.%3lli "_Str,
         m_xId.iBuffer(),
         m_xId.iRow()
         );
  aKey->vShow(pColVec);
  
  on nRight;
  for (nRight = 0_on; nRight < m_iOffsets; nRight++) {
    
    vTrace(" Offset(%lld): %lld "_Str,
           *nRight,
           iRightOffset(nRight) == C_NIL_OFFSET ?
           C_NIL_OFFSET :
           iRightOffset(nRight) + iOffset + nSizeEntry());
  }
  
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* get indexbuffer of this entry                                              */
/*----------------------------------------------------------------------------*/
IndexBuffer* Entry::pIndexBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  bn nBufferNo = m_xId.iBuffer();
  IndexBuffer* pIndexBuffer =
  reinterpret_cast<IndexBuffer*>(gpBufferPool->pToBuffer(nBufferNo));
  
  return pIndexBuffer;
}

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
/* set key given entry wsp                                                    */
/*----------------------------------------------------------------------------*/
void Entry::vKey(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  ASSERT(m_nThisBytes);
  
  // copy Key
  Key* aEntryKey = this->aKey();
  
  Key* aEntWspKey = xEntWsp.aKey();
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  MemLib::vCopy(aEntryKey,aEntWspKey,nKeyBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence incl Object & Footer             */
/*----------------------------------------------------------------------------*/
i8 Entry::nSizeEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_nThisBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Entry ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
char* Entry::aRowData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  char* aRowData = pAdd<char*>(this,sizeof(Entry));
  
  return pPoolCheck<char*>(aRowData);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  StrVal* pStr = (StrVal*)aRowData();
  
  vTrace("Entry %p: DataBytes %lld %s (%lld bytes) %s\n"_Str,
         this,
         //m_iTxId,
         m_nThisBytes,
         aRowData(),
         StrLib::iSize(aRowData()),
         pStr->s()
         );
}

/*----------------------------------------------------------------------------*/
/* extract a key into keywsp from this row using given index columns          */
/*----------------------------------------------------------------------------*/
void Entry::vExtractKey(KeyWsp& xKeyWsp, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  char* pRowData = aRowData();
  
  i8 nCols = pIdxColVec->nItems();
  ValWsp xValWsp;
  s256 s;
  
  // get the row data
  StrVal* pStrVal = (StrVal*)aRowData();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(iColumn);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    char* pDst = s.s();
    char* aCursor = reinterpret_cast<char*>(pStrVal->aStr());
    
    // get the column value from the row for the iColumnNo
    i8 iCol = -1;
    do {
      bool bFindPipe = StrLib::bExtractItem(&aCursor, pDst, '|');
      ASSERT(bFindPipe);
      iCol++;
      aCursor++;
    } while (iColumnNo != iCol);
    
    switch(pColumn->eColumnType()) {
        
      case CT_BOOLEAN: {
        Bol xBol;
        bool b = DtmLib::bExtractBol(&pDst,&xBol);
        ASSERT(b);
        xValWsp = xBol;
        break;
      }
        
      case CT_CHAR: {
        ASSERT(0);
        break;
      }
        
      case CT_DATE: {
        Dte xDte;
        bool b = DtmLib::bExtractDte(&pDst,&xDte);
        ASSERT(b);
        xValWsp = xDte;
        break;
      }
        
      case CT_DECIMAL: {
        ASSERT(0);
        break;
      }
        
      case CT_FLOAT: {
        Flt xFlt;
        bool b = DtmLib::bExtractFlt(&pDst,&xFlt);
        ASSERT(b);
        xValWsp = xFlt.dAsD8();
        break;
      }
        
      case CT_INTEGER: {
        xValWsp = StrLib::iToI8(pDst);
        break;
      }
        
      case CT_INTERVAL: {
        Ivl xIvl;
        bool b = DtmLib::bExtractIvl(&pDst,&xIvl);
        ASSERT(b);
        xValWsp = xIvl;
        break;
      }
        
      case CT_TIME: {
        Tme xTme;
        bool b = DtmLib::bExtractTme(&pDst,&xTme);
        ASSERT(b);
        xValWsp = xTme;
        break;
      }
        
      case CT_TIMESTAMP: {
        Tsp xTsp;
        bool b = DtmLib::bExtractTsp(&pDst,&xTsp);
        ASSERT(b);
        xValWsp = xTsp;
        break;
      }
        
      case CT_VARBINARY: {
        ASSERT(0);
        break;
      }
        
      case CT_VARCHAR: {
        xValWsp = s;
        break;
      }
        
      default:
        ASSERT(0);
    }
    
    // add the column value to the key
    xKeyWsp.vAddElement(xValWsp, pColumn);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this row                                              */
/*----------------------------------------------------------------------------*/
i8 Entry::nSizeRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  return m_nThisBytes;
}

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 Entry::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  return -1; //m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vTxId(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  //m_iTxId = iTxId;
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Entry::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iFooterOffset = m_nThisBytes - sizeof(Footer);
  
  Footer* pFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
