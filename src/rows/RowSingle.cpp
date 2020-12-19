/*============================================================================*/
/* RowSingle.cpp -- Computable form of single Row                             */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowSingle* Rows::_pRowSingle(void* aAddr)
{
  RowSingle* pRowSingle = reinterpret_cast<RowSingle*>(aAddr);
  
  return pRowSingle;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowSingle::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWSINGLE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* RowSingle::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowSingle::operator delete(void* aAddr)
{
  RowSingle* pRowSingle = reinterpret_cast<RowSingle*>(aAddr);
  
  HeapMem::vFree(pRowSingle);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowSingle::RowSingle(StreamDef* pStreamDef)
: Object(CN_ROWSINGLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_pStreamDef  = pStreamDef;
  m_nColumns    = pStreamDef->nColumns(C_ALL);
  m_iSubSet     = C_ALL;
  
  m_bGetAll     = true;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
  m_iLastCol    = C_UNKNOWN;
    
  // - - - - - - - - - - - - - - - - - -
  
  vResolveRowPix(pStreamDef);

  Row* aRow = aAllotNewRow();
    
  _checkInitialised_(CN_ROWSINGLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowSingle::~RowSingle(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get address of cell array                                                  */
/*----------------------------------------------------------------------------*/
byte* RowSingle::ayCellArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  byte* ayArray = (byte*) m_aRowCur->aCellArray();

  return ayArray;
}

/*----------------------------------------------------------------------------*/
/* get address of column datum                                                */
/*----------------------------------------------------------------------------*/
Dtm* RowSingle::aColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm* aDtm = RowBox_t<C_ROWSINGLE_WSP>::aColDtm(i_Col);
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Dtm RowSingle::wColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm* aDtm = RowBox_t<C_ROWSINGLE_WSP>::aColDtm(i_Col);

  return * aDtm;;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
char* RowSingle::azColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  char* azVal = (char*)szColVal(i_Col);
  
  return azVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
i8 RowSingle::iColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 iVal = RowBox_t<C_ROWSINGLE_WSP>::iColVal(i_Col);

  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
d8 RowSingle::dColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  d8 dVal = RowBox_t<C_ROWSINGLE_WSP>::dColVal(i_Col);
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Stz* RowSingle::szColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Stz* szVal = RowBox_t<C_ROWSINGLE_WSP>::szColVal(i_Col);
  
  return szVal;
}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
void RowSingle::vCopyRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  RASSERT(m_oRowPix.m_bInitialised);
  
  RowBox_t<C_ROWSINGLE_WSP>::vReset();
  vResetValues();
  Row* aRow = aAllotNewRow();
  
  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
  
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (pRow->bIsColNull(i_Col)) {
        vSetColNull(i_Col);
        continue;
      }
      
      Stz* szValue = pRow->szColVal_ofs(i_Col);
      vDepositColVal(i_Col,szValue);
      
      vTrace("Deposit: %s\n"_Stz,szValue);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    vDepositColVal(i_Col, pRow->iColVal_int(i_Col));
  }
}

/*----------------------------------------------------------------------------*/
/* is given column (of current row) null ?                                    */
/*----------------------------------------------------------------------------*/
bool RowSingle::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  bool bIsNull = RowBox_t<C_ROWSINGLE_WSP>::bIsColNull(i_Col);
  
  return bIsNull;
}

/*----------------------------------------------------------------------------*/
/* print the row                                                              */
/*----------------------------------------------------------------------------*/
void RowSingle::vPrintRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  HV_Record hvRecord(m_pStreamDef->aRowPix());
  hvRecord->vToCsvRecord(pRowCurrent());
  hvRecord->vPrintAsRow();
}

/*----------------------------------------------------------------------------*/
/* reset the row                                                              */
/*----------------------------------------------------------------------------*/
void RowSingle::vResetRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  RowBox_t<C_ROWSINGLE_WSP>::vResetRow();
}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
void RowSingle::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  RowBox_t<C_ROWSINGLE_WSP>::vReset();
}

/*----------------------------------------------------------------------------*/
/* set column value to be null                                                */
/*----------------------------------------------------------------------------*/
void RowSingle::vSetColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  RowBox_t<C_ROWSINGLE_WSP>::vColSetNull(i_Col);
}

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
i8 RowSingle::nSumVchSizes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  RASSERT(m_oRowPix.m_bInitialised);

  i8 nSizeVch = 0;
  
  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (bIsColNull(i_Col)) {
        continue;
      }
      
      Stz* szValue = szColVal(i_Col);
      nSizeVch += szValue->nSize();
      
      continue;
    }

  }
  
  return nSizeVch;
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
Stz* RowSingle::szStzAlottable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  Stz* szAllottable = RowBox_t<C_ROWSINGLE_WSP>::szStzAllot_begin();
  
  return szAllottable;
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
Stz* RowSingle::szStzArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  Stz* szArea = RowBox_t<C_ROWSINGLE_WSP>::szStzArea();
  
  return szArea;
}

/*----------------------------------------------------------------------------*/
/* given number of bytes have been taken from head/stz area                   */
/*----------------------------------------------------------------------------*/
void RowSingle::vStzBytesTaken(i8 nBytesTaken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  RowBox_t<C_ROWSINGLE_WSP>::vTakeHead(nBytesTaken);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowSingle::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
