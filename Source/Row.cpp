/*============================================================================*/
/* Row.cpp -- Row                                                             */
/*============================================================================*/

#include "DBobjects.h"

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
void* Row::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Row class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t iSize, Row* pRow)
{
  RASSERT(iSize);
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row::Row(i8 iDataBytes)
: Object(CN_ROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m_iTxId      = -1;
  m_iDataBytes = iDataBytes;

  _setFooter_();
  
  _checkInitialised_(CN_ROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row::~Row(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get data                                                                   */
/*----------------------------------------------------------------------------*/
char* Row::pData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  char* pDataAddr = pAdd<char*>(this,sizeof(Row));
  
  return pPoolCheck<char*>(pDataAddr);
}

/*----------------------------------------------------------------------------*/
/* get data bytes                                                             */
/*----------------------------------------------------------------------------*/
i8 Row::iDataBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  return m_iDataBytes;
}

/*----------------------------------------------------------------------------*/
/* set data bytes                                                             */
/*----------------------------------------------------------------------------*/
void Row::vDataBytes(i8 iDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_iDataBytes = iDataBytes;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Row::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  StrVal* pStr = (StrVal*)pData();
  
  vTrace("Row %p: TxId %lld DataBytes %lld %s (%lld bytes) %s\n"_Str,
         this,
         m_iTxId,
         m_iDataBytes,
         pData(),
         StrLib::iSize(pData()),
         pStr->s()
         );
}

/*----------------------------------------------------------------------------*/
/* get a key as a keywsp wsp from this row for the given index columns        */
/*----------------------------------------------------------------------------*/
void Row::vGetKey(KeyWsp& xKeyWsp, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
 
  char* pRowData = pData();
  
  i8 nCols = pColVec->nItems();
  i8 iCol = -1;
  ValWsp xValWsp;
  s256 s;

  // get the row data
  StrVal* pStrVal = (StrVal*)pData();
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    // get the column from the index column vector
    Column* pColumn = pColVec->pGet(iColumn);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    char* pDst = s.s();
    char* aCursor = reinterpret_cast<char*>(pStrVal->aStr());
    
    // get the column value from the row for the iColumnNo
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
        xValWsp = xBol.iAsI8();
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
        xValWsp = xDte.iAsI8();
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
        xValWsp = xIvl.iAsI8();
        break;
      }
        
      case CT_TIME: {
        Tme xTme;
        bool b = DtmLib::bExtractTme(&pDst,&xTme);
        ASSERT(b);
        xValWsp = xTme.iAsI8();
        break;
      }
        
      case CT_TIMESTAMP: {
        Tsp xTsp;
        bool b = DtmLib::bExtractTsp(&pDst,&xTsp);
        ASSERT(b);
        xValWsp = xTsp.iAsI8();
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
    xKeyWsp.vAddElement(xValWsp);
  }

}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this row                                              */
/*----------------------------------------------------------------------------*/
i8 Row::nSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);

  i8 iBytes = sizeof(Row) + sizeof(Footer) + m_iDataBytes;
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* get tx id                                                                  */
/*----------------------------------------------------------------------------*/
i8 Row::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);

  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set tx id                                                                  */
/*----------------------------------------------------------------------------*/
void Row::vTxId(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);

  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for row                                                 */
/*----------------------------------------------------------------------------*/
i8 Row::iBytesRequired(Str* aRow)
{
  i8 iBytes = sizeof(Row) + sizeof(Footer) + aRow->nSize();
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* get bytes required for row                                                 */
/*----------------------------------------------------------------------------*/
i8 Row::iBytesRequired(i8 iDataBytes)
{
  i8 iBytes = sizeof(Row) + sizeof(Footer) + iDataBytes;
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Row::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(Row) + m_iDataBytes);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
