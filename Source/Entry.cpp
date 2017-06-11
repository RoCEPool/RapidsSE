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
Entry::Entry(eCN eClassNo, i8 nThisBytes, i8 nOffsets)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //m_iTxId      = -1;
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
  i8 nOffsetBytes = RightVec::nBytesRequired(nOffsets);
  
  i8 nBytes = sizeof(Entry)
               + nKeyBytes
               + nOffsetBytes
               + sizeof(Footer);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nThisBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Variable data area ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
char* Entry::aVblData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  char* aRowData = pAdd<char*>(this,sizeof(Entry));
  
  return pPoolCheck<char*>(aRowData);
}

/*----------------------------------------------------------------------------*/
/*-- Offsets -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get highest offset                                                         */
/*----------------------------------------------------------------------------*/
on Entry::nHighestOffset(void)
{
  ASSERT(m_nOffsets >= 0 && m_nOffsets <= gnIndexOffsets);
  
  on nHighest(m_nOffsets - 1);
  
  return nHighest;
}

/*----------------------------------------------------------------------------*/
/* get nOffsets                                                               */
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
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
  _precondition_(_MAGIC_,CN_ENTRY,_FOOTER_);
  
  i8 nOffsetBytes = RightVec::nBytesRequired(m_nOffsets);
  
  Key* aKey = pAdd<Key*>(aVblData(),nOffsetBytes);
  
  return aKey;
}

/*----------------------------------------------------------------------------*/
/* replace existing key with given entry wsp                                  */
/*----------------------------------------------------------------------------*/
void Entry::vReplaceKey(EntWsp& xEntWsp)
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
/*-- Index -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  m_nOffsets = xEntWsp.nOffsets();
  
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
void Entry::vShow(i8 iBase, IdxColVec* pColVec)
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
         m_nOffsets
         );
  
  vTrace("Id %3lli.%3lli "_Str,
         m_xId.iBuffer(),
         m_xId.iRow()
         );
  aKey->vShow(pColVec);
  
  for (on nRight = 0_on; nRight < m_nOffsets; nRight++) {
    
    vTrace(" Offset(%lld): %lld "_Str,
           *nRight,
           iRightOffset(nRight) == C_NIL_OFFSET ?
           C_NIL_OFFSET :
           iBase + iRightOffset(nRight) + nSizeEntry());
  }
  
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/*-- Row ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  StrVal* pStr = (StrVal*)aVblData();
  
  vTrace("Entry %p: ThisBytes %lld %s (%lld bytes) %s\n"_Str,
         this,
         //m_iTxId,
         m_nThisBytes,
         aVblData(),
         StrLib::iSize(aVblData()),
         pStr->s()
         );
}

/*----------------------------------------------------------------------------*/
/* extract a key into keywsp from this row using given index columns          */
/*----------------------------------------------------------------------------*/
void Entry::vExtractKey(KeyWsp& xKeyWsp, IdxColVec* pIdxColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  ValWsp xValWsp;
  s256 s;
  
  // get the data
  StrVal* pStrVal = (StrVal*)aVblData();
  
  i8 nCols = pIdxColVec->nItems();
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
    xKeyWsp.vAddDatum(xValWsp, pColumn);
  }
  
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
