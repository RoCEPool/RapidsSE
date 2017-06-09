/*============================================================================*/
/* BufferItem.cpp -- abstract buffer item                                     */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BufferItem* DBobjects::_pBufferItem(void* aAddr)
{
  BufferItem* pBufferItem = reinterpret_cast<BufferItem*>(aAddr);
  
  return pBufferItem;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row* DBobjects::_aRow(void* aAddr)
{
  Row* aRow = reinterpret_cast<Row*>(aAddr);
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BufferItem::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: BufferItem class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BufferItem::operator new(size_t iSize, BufferItem* pBufferItem)
{
  RASSERT(iSize);
  return pBufferItem;
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferItem::BufferItem(eCN eClassNo)
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
BufferItem::BufferItem(eCN eClassNo, i8 nThisBytes)
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
BufferItem::~BufferItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for an Entry based on given EntWsp incl Object & Footer */
/*----------------------------------------------------------------------------*/
i8 BufferItem::nBytesRequiredEntry(EntWsp& xEntWsp)
{
  i8 nOffsets = xEntWsp.nOffsets();
  
  i8 nKeyBytes = xEntWsp.nKeySize();
  
  i8 nBytes = Entry::nBytesRequiredEntry(nOffsets,nKeyBytes);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry incl Object & Footer                    */
/*----------------------------------------------------------------------------*/
i8 BufferItem::nBytesRequiredEntry(i8 nOffsets, i8 nKeyBytes)
{
  i8 nEntryDataBytes = EntryData::nBytesRequired(nOffsets,nKeyBytes);
  
  i8 nBytes = sizeof(Entry) + nEntryDataBytes + sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get bytes required for row                                                 */
/*----------------------------------------------------------------------------*/
i8 BufferItem::nBytesRequiredRow(i8 iDataBytes)
{
  i8 nBytes = sizeof(Row) + iDataBytes + sizeof(Footer);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
char* BufferItem::aRowData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  char* aRowData = pAdd<char*>(this,sizeof(Row));
  
  return pPoolCheck<char*>(aRowData);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void BufferItem::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  StrVal* pStr = (StrVal*)aRowData();
  
  vTrace("Row %p: DataBytes %lld %s (%lld bytes) %s\n"_Str,
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
void BufferItem::vExtractKey(KeyWsp& xKeyWsp, IdxColVec* pIdxColVec)
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
i8 BufferItem::nSizeRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  return m_nThisBytes;
}

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 BufferItem::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  return -1; //m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void BufferItem::vTxId(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  //m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BufferItem::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iFooterOffset = m_nThisBytes - sizeof(Footer);
  
  Footer* pFooter = pAdd<Footer*>(this,iFooterOffset);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
