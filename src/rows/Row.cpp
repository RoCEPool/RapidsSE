/*============================================================================*/
/* Row.cpp -- Computable form of Row                                        */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row* Rows::_pRow(void* aAddr)
{
  Row* pRow = reinterpret_cast<Row*>(aAddr);
  
  ASSERT(!pRow || CN_ROW == pRow->eClass());
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Row class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row::Row(void)
: Object(CN_ROW)
{
 
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row::~Row(void) noexcept(false)
{
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size of row (needed for MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem))  */
/*----------------------------------------------------------------------------*/
i8 Row::nSizeBytes(StreamDef* pStreamDef)
{
  i8 nSize = sizeof(Row) + (pStreamDef->nColumns(0) - 1) * sizeof(Dtm);
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get size of row + dtm[]                                                    */
/*----------------------------------------------------------------------------*/
i8 Row::nRowProper(RowPix* pRowPix)
{
  i8 nSize = sizeof(Row) + ( (pRowPix->m_nColumns - 1) * sizeof(Dtm) );
  
  return nSize;
}


/*----------------------------------------------------------------------------*/
/* dump the row                                                               */
/*----------------------------------------------------------------------------*/
void Row::vDump(StreamDef* pStreamDef)
{
  vTrace("ROW: %p"_Stz, this);
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (m_wxaColumnCell[i_Col].bIsNull()) {
        vTrace(" NULL "_Stz);
        continue;
      }
      
      Stz* acSrcString = szColVal_ofs(i_Col);
      
      vTrace(" %s "_Stz, acSrcString);
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    vTrace(" %ld "_Stz, m_wxaColumnCell[i_Col].m_iWrd);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of cell array                                                  */
/*----------------------------------------------------------------------------*/
m8* Row::aCellArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m8* aArray = (m8*) & m_wxaColumnCell[0];
  
  return aArray;
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
m8* Row::aColCell(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m8* aCell = (m8*) & m_wxaColumnCell[i_Col];
  
  return aCell;
}

/*----------------------------------------------------------------------------*/
/* get address of column datum                                                */
/*----------------------------------------------------------------------------*/
Dtm* Row::aColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  Dtm* aDtm = & m_wxaColumnCell[i_Col];
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Dtm Row::wColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  Dtm wDtm = m_wxaColumnCell[i_Col];
  
  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
i8 Row::iColVal_int(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  i8 iVal = m_wxaColumnCell[i_Col].m_iWrd;
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_int(i8 i_Col, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m_wxaColumnCell[i_Col].m_iWrd = iVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
d8 Row::dColVal_flt(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  d8 dVal = m_wxaColumnCell[i_Col].m_dWrd;
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_flt(i8 i_Col, d8 dVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m_wxaColumnCell[i_Col].m_dWrd = dVal;
}

/*----------------------------------------------------------------------------*/
/* get string column                                                          */
/*----------------------------------------------------------------------------*/
Stz* Row::szColVal_ofs(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  // add cell contents (i.e. offset) to cell address to string address
  
  m8* amCellAddress = (m8*) & m_wxaColumnCell[i_Col];
  i8  i_Ofs = m_wxaColumnCell[i_Col].m_iWrd;

  Stz* pStz = aAdd<Stz*>(amCellAddress,i_Ofs);
  
  return pStz;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_ofs(i8 i_Col, i8 iOfs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m_wxaColumnCell[i_Col].m_iWrd = iOfs;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_ofs(i8 i_Col, Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  i8 i_Ofs = - nDifference(&m_wxaColumnCell[i_Col],szStz);
  
  m_wxaColumnCell[i_Col].m_iWrd = i_Ofs;
}

/*----------------------------------------------------------------------------*/
/* is column a null value?                                                    */
/*----------------------------------------------------------------------------*/
bool Row::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  bool bNull = m_wxaColumnCell[i_Col].bIsNull();
  
  return bNull;
}

/*----------------------------------------------------------------------------*/
/* is column a null value?                                                    */
/*----------------------------------------------------------------------------*/
void Row::vSetColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  m_wxaColumnCell[i_Col].vSetNull();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
