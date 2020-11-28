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
      
      if (m_waColVal[i_Col].bIsNull()) {
        vTrace(" NULL "_Stz);
        continue;
      }
      
      Stz* acSrcString = szColVal_ofs(i_Col);
      
      vTrace(" %s "_Stz, acSrcString);
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    vTrace(" %ld "_Stz, m_waColVal[i_Col].m_iWrd);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of column datum                                                */
/*----------------------------------------------------------------------------*/
Dtm* Row::aColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  Dtm* aDtm = & m_waColVal[i_Col];
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Dtm Row::wColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  Dtm wDtm = m_waColVal[i_Col];
  
  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
char* Row::azColVal_str(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  char* azVal = m_waColVal[i_Col].m_azStr;
  
  return azVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
i8 Row::iColVal_int(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  i8 iVal = m_waColVal[i_Col].m_iWrd;
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_int(i8 i_Col, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_waColVal[i_Col].m_iWrd = iVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
d8 Row::dColVal_flt(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  d8 dVal = m_waColVal[i_Col].m_dWrd;
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_flt(i8 i_Col, d8 dVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_waColVal[i_Col].m_dWrd = dVal;
}

/*----------------------------------------------------------------------------*/
/* get string column                                                          */
/*----------------------------------------------------------------------------*/
Stz* Row::szColVal_ofs(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  // get the poolBuffer from the row address
  PoolBuffer* pPoolBuffer = PoolBuffer::pWithinPoolBuffer(this);
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(pPoolBuffer);
  
  Object* pRowBuffer =
  reinterpret_cast<Object*>(pHeapBuffer->aFirstChunk());
  
  // get the offset of the string
  i8 i_Offset = m_waColVal[i_Col].m_iWrd;
  
  // get address of string within pool buffer
  Stz* pStz = pAdd<Stz*>(pRowBuffer,i_Offset);
  
  return pStz;
}

/*----------------------------------------------------------------------------*/
/* set column value                                                           */
/*----------------------------------------------------------------------------*/
void Row::vColVal_ofs(i8 i_Col, i8 iOfs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_waColVal[i_Col].m_iWrd = iOfs;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Stz* Row::szColVal_vch(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  Stz* szVal = m_waColVal[i_Col].m_szVch;
  
  return szVal;
}

/*----------------------------------------------------------------------------*/
/* is column a null value?                                                    */
/*----------------------------------------------------------------------------*/
bool Row::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  bool bNull = m_waColVal[i_Col].bIsNull();
  
  return bNull;
}

/*----------------------------------------------------------------------------*/
/* is column a null value?                                                    */
/*----------------------------------------------------------------------------*/
void Row::vSetColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_waColVal[i_Col].vSetNull();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
