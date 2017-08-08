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
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_iTxId      = -1;
  m_nThisBytes = nThisBytes;
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
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
/* get the index buffer that this entry references                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* Entry::pIndexBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  bn nBufferNo = m_xId.iBuffer();
  EntryBuffer* pIndexBuffer =
  reinterpret_cast<EntryBuffer*>(gpBufferPool->pToBuffer(nBufferNo));
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry occurence incl Object & Footer             */
/*----------------------------------------------------------------------------*/
i8 Entry::nSizeEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  return m_nThisBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Row Data ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* get row data pointer                                                       */
/*----------------------------------------------------------------------------*/
char* Entry::aRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  i8 iRowSize = m_nThisBytes - (nOffsetBytes + sizeof(Entry) + sizeof(Footer) + sizeof(i8));
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= gnIndexOffsets);
  
  on nHighest(m_nOffsets - 1);
  
  return nHighest;
}

/*----------------------------------------------------------------------------*/
/* get number of offsets                                                      */
/*----------------------------------------------------------------------------*/
i8 Entry::nOffsets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  return m_nOffsets;
}

/*----------------------------------------------------------------------------*/
/* get right offset for given level                                           */
/*----------------------------------------------------------------------------*/
i8 Entry::iRightOffset(on nOffsetNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gnIndexOffsets);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ASSERT(nOffsetNo >= 0_on && nOffsetNo < gnIndexOffsets);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  
  Key* aKey = pAdd<Key*>(aVblData(),nOffsetBytes);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* replace existing key with given entry wsp                                  */
/*----------------------------------------------------------------------------*/
void Entry::vReplaceKey(EntWrk& xEntWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ASSERT(m_nThisBytes);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ASSERT(m_nThisBytes);
  
  // set RowId
  m_xId  = xEntWrk.xId();
  
  // set Offsets
  m_nOffsets = xEntWrk.nOffsets();
  
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
    char* aEntryData = this->aRow();
    char* aEntWrkData = xEntWrk.aData();
    i8 nDataSize = xEntWrk.nDataSize();
    
    ASSERT(nDataSize);
    MemLib::vCopy(aEntryData,aEntWrkData,nDataSize,eClass());
    
    //char* aRow = this->aRow();
    //StrVal* pStrVal = reinterpret_cast<StrVal*>(aRow);
    //vTrace("%s"_Stz, pStrVal->aStr());
  }
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
}
/*----------------------------------------------------------------------------*/
/* set entry given eanother entry                                             */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(Id xId, i8 iKey, Entry* pOldEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ASSERT(m_nThisBytes);
  
  // set RowId
  m_xId  = xId;
  
  // copy Key
  i8 nKeyBytes = 8;
  Key* aEntryKey = this->aKey();
  
  MemLib::vCopy(aEntryKey,&iKey,nKeyBytes,eClass());
  
  // copy data
  char* aEntryData = this->aRow();;
  i8 nDataSize = pOldEntry->nRowSize();
  
  ASSERT(nDataSize);
  
  //StrVal* pStrVal = new((StrVal*)aEntryData) StrVal((StrVal*)aEntryData);
  //pStrVal->vAssign(sRow.aStr(),nDataSize);
  MemLib::vCopy(aEntryData,pOldEntry->aRow(),nDataSize,eClass());
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iBase, Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
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
  
  vTrace("Id %3lli.%3lli "_Stz,
         m_xId.iBuffer(),
         m_xId.iRow()
         );
  
  aKey->vShow(pIndex->pIdxColVec());

#ifdef _ROWFORMAT
  if (pIndex->bIsHidden()) {
    char* pRow = aRow();
    if (nRowSize() > 0)
      RowData::vShow((Stz*)aRow(), pIndex->pTable()->pColumnVec());
  }
#else
  if (pIndex->bIsHidden()) {
    StrVal* pStrVal = reinterpret_cast<StrVal*>(aRow());
    vTrace("%s"_Stz, pStrVal->aStr());
  }
#endif
  
  for (on nRight = 0_on; nRight < m_nOffsets; nRight++) {
    
    vTrace(" Offset(%lld): %lld "_Stz,
           *nRight,
           iRightOffset(nRight) == C_NIL_OFFSET ?
           C_NIL_OFFSET :
           iBase + iRightOffset(nRight) + nSizeEntry());
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
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
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(aRow());
  
  vTrace("Entry %p: ThisBytes %lld %s (%lld bytes) %s\n"_Stz,
         this,
         //m_iTxId,
         m_nThisBytes,
         aRow(),
         nRowSize(),
         pStrVal->aStr()
         );
}

/*----------------------------------------------------------------------------*/
/* extract a key into KeyWrk from this row using given index columns          */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
void Entry::vExtractKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ValWrk xValWrk;
  
  // VCH cannot be > 256 bytes
  s256 s;                                                       //ALKA_ROWFORMAT
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get the column value from the row for the iColumnNo
    char* pDst = (char*)RowData::pColumn(aRow(), iColumnNo);
    
    switch(pColumn->eColumnType()) {
        
      case CT_BOOLEAN: {
        Bol xBol = *((Bol*)pDst);
        xValWrk = xBol;
        break;
      }
        
      case CT_CHAR: {
        NEVER_REACH;
        break;
      }
        
      case CT_DATE: {
        Dte xDte = *((Dte*)pDst);
        xValWrk = xDte;
        break;
      }
        
      case CT_DECIMAL: {
        Bcd xBcd = *((Bcd*)pDst);
        xValWrk = xBcd;
        break;
      }
        
      case CT_FLOAT: {
        Flt xFlt = *((Flt*)pDst);
        xValWrk = xFlt.dAsD8();
        break;
      }
        
      case CT_INTEGER: {
        Int xInt = *((Int*)pDst);
        xValWrk = xInt.iAsI8();
        break;
      }
        
      case CT_INTERVAL: {
        Ivl xIvl = *((Ivl*)pDst);
        xValWrk = xIvl;
        break;
      }
        
      case CT_TIME: {
        Tme xTme = *((Tme*)pDst);
        //bool b = Tme::bExtract(pDst,&xTme,iLength,pColumn);
        //ASSERT(b);
        xValWrk = xTme;
        break;
      }
        
      case CT_TIMESTAMP: {
        Tsp xTsp = *((Tsp*)pDst);
        //bool b = Tsp::bExtract(pDst,&xTsp,iLength,pColumn);
        //ASSERT(b);
        xValWrk = xTsp;
        break;
      }
        
      case CT_VARBINARY: {
        NEVER_REACH;
        break;
      }
        
      case CT_VARCHAR: {
        xValWrk = s;
        break;
      }
        
      default:
        NEVER_REACH;
    }
    
    // add the column value to the key
    xKeyWrk.vAddDatum(xValWrk, pColumn);
  }
  
}

#else
void Entry::vExtractKey(KeyWrk& xKeyWrk, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  ValWrk xValWrk;
  s256 s;
  
  // get the data
  StrVal* pStrVal = (StrVal*)aRow();
  
  i8 nCols = pIdxColVec->nItems();
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    char* pDst = s.aChar();
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
        bool b = Bol::bExtract(&pDst,&xBol,pColumn);
        ASSERT(b);
        xValWrk = xBol;
        break;
      }
        
      case CT_CHAR: {
        NEVER_REACH;
        break;
      }
        
      case CT_DATE: {
        Dte xDte;
        bool b = Dte::bExtract(&pDst,&xDte,pColumn);
        ASSERT(b);
        xValWrk = xDte;
        break;
      }
        
      case CT_DECIMAL: {
        Bcd xBcd;
        bool b = Bcd::bExtract(&pDst,&xBcd,pColumn);
        ASSERT(b);
        xValWrk = xBcd;
        break;
      }
        
      case CT_FLOAT: {
        Flt xFlt;
        bool b = Flt::bExtract(&pDst,&xFlt,pColumn);
        ASSERT(b);
        xValWrk = xFlt.dAsD8();
        break;
      }
        
      case CT_INTEGER: {
//        xValWrk = StrLib::iToI8(pDst);
//        break;
        Int xInt;
        bool b = Int::bExtract(&pDst,&xInt,pColumn);
        ASSERT(b);
        if (b)
          xValWrk = xInt.iAsI8();
        break;
      }
        
      case CT_INTERVAL: {
        Ivl xIvl;
        bool b = Ivl::bExtract(&pDst,&xIvl,pColumn);
        ASSERT(b);
        xValWrk = xIvl;
        break;
      }
        
      case CT_TIME: {
        Tme xTme;
        bool b = Tme::bExtract(&pDst,&xTme,pColumn);
        ASSERT(b);
        xValWrk = xTme;
        break;
      }
        
      case CT_TIMESTAMP: {
        Tsp xTsp;
        bool b = Tsp::bExtract(&pDst,&xTsp,pColumn);
        ASSERT(b);
        xValWrk = xTsp;
        break;
      }
        
      case CT_VARBINARY: {
        NEVER_REACH;
        break;
      }
        
      case CT_VARCHAR: {
        xValWrk = s;
        break;
      }
        
      default:
        NEVER_REACH;
    }
    
    // add the column value to the key
    xKeyWrk.vAddDatum(xValWrk, pColumn);
  }
  
}
#endif

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 Entry::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  return -1; //m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vTxId(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
  bn nBufferNo = m_xId.iBuffer();
  
  return nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get the RightVec for this Entry                                            */
/*----------------------------------------------------------------------------*/
RightVec* Entry::aRightVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_); _class2_(CN_ENTRY,CN_ROW);
  
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
  _precondition_(_ADDRESS_,_MAGIC_); _class2_(CN_ENTRY,CN_ROW);
  
  i8 iFooterOffset = m_nThisBytes - sizeof(Footer);
  
  Footer* aFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
