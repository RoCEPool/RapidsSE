/*============================================================================*/
/* RowSingle.cpp -- Computable form of single Row                             */
/*============================================================================*/

#include "BASE.h"

#include "RowSingle.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowSingle* STORE::_pRowSingle(void* aAddr)
{
  RowSingle* pRowSingle = reinterpret_cast<RowSingle*>(aAddr);
  
  return pRowSingle;
}

void* aRowSingle_GetCellArray(void* pRowSingle, char** aazStrArea) {
  return RowSingle::aGetCellArray((RowSingle*) pRowSingle, aazStrArea);
}

void vRowSingle_Print(void* pRowSingle) {
  RowSingle::vPrint((RowSingle*)pRowSingle);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowSingle::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_ROWSINGLE);
  
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
  
  Heaps::vFree(pRowSingle);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowSingle::RowSingle(StreamDef* pStreamDef)
: Object(CN_ROWSINGLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
    
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
    
  // - - - - - - - - - - - - - - - - - -
  
  vResolveRowPix(pStreamDef);
  
  Row* aRow = pAllotRow();
  UNUSED(aRow);
  
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* discrad the string area used                                               */
/*----------------------------------------------------------------------------*/
void RowSingle::vDiscardStr(RowSingle* pRowSingle) {
  _static_

  pRowSingle->vResetHeads();
}

/*----------------------------------------------------------------------------*/
/* get the cell array                                                         */
/*----------------------------------------------------------------------------*/
void* RowSingle::aGetCellArray(RowSingle* pRowSingle, char** aazStrArea) {
  _static_

  *aazStrArea = (char*)pRowSingle->szStzAlottable();
  return pRowSingle->pRowCurrent()->aCellArray();
}

/*----------------------------------------------------------------------------*/
/* get the cell array                                                         */
/*----------------------------------------------------------------------------*/
void RowSingle::vPrint(RowSingle* pRowSingle) {
  _static_

  pRowSingle->vPrintRow();
}

/*----------------------------------------------------------------------------*/
/* allot string area                                                          */
/*----------------------------------------------------------------------------*/
void RowSingle::vRowStrSize(RowSingle* pRowSingle, i8 nRowStrSize) {
  _static_

  pRowSingle->vTakeHead(nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* repair the rowSingle                                                       */
/*----------------------------------------------------------------------------*/
void RowSingle::vRepair(Object* pObject)
{
  _static_

  RowSingle* pRowSingle = _pRowSingle(pObject);

  // delete the rowSingle
  delete pRowSingle;
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
  
  RASSERT(m_pRowPix);
  
  RowBox_t<C_ROWSINGLE_WSP>::vReset();
  vResetValues();
  Row* aRow = pAllotRow();
  UNUSED(aRow);
  
  for (i8 i_Col = 0; i_Col < nColumns(); i_Col++) {
  
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
      
      //vTrace("Deposit: %s\n"_Stz,szValue);
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

  HV_Record hvRecord(m_pStreamDef->pRowPix());
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
  
  RASSERT(m_pRowPix);

  i8 nSizeVch = 0;
  
  for (i8 i_Col = 0; i_Col < nColumns(); i_Col++) {
    
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
