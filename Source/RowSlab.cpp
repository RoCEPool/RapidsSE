/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,RowSlab,CN_ROWSLAB)
__aNewFunction(RowSlab,CN_ROWSLAB)
__vDeleteFunction(RowSlab,CN_ROWSLAB)
__aFooterFunction(RowSlab)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowSlab::RowSlab(StreamDef* pStreamDef)
: Object(CN_ROWSLAB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  m_nItems = 0;
  
  m_nMaxRowSize = 128;  // FOR TESTING PURPOSES

  m_nCols  = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < C_MAX_DEFCOLUMNS; i_Col++)
    m_xaColPix[i_Col].m_uAsI4 = 0;
  
  pStreamDef->vToColPix(m_xaColPix);
  
  // m_xWsp

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowSlab::~RowSlab(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get a row cell                                                             */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iCell(i8 iRow, i8 iCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8* iaCells = (i8*)m_xWsp.ayArea();

  i8 iLglCell = iRow * m_nCols + iCell;
  
  i8 i_Cell = C_FIRST_CELL - iLglCell;
  
  i8 iVal =iaCells[i_Cell];
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                         */
/*----------------------------------------------------------------------------*/
void RowSlab::vDeposit(i8& i_Cell, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  // move to previous cell
  i_Cell--;
  
  i8* iaCells = (i8*)m_xWsp.ayArea();
  
  iaCells[i_Cell] = iVal;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* begin deposit of string                                                    */
/*----------------------------------------------------------------------------*/
char* RowSlab::azString(i8& i_Cell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  char* azStzCell = (char*)m_xWsp.ayLatterHead();
  
  i_Cell--;

  i8* iaCells = (i8*)m_xWsp.ayArea();
  
  iaCells[i_Cell] = (i8)azStzCell;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return azStzCell;
}

/*----------------------------------------------------------------------------*/
/* take bytes for deposited string                                            */
/*----------------------------------------------------------------------------*/
void RowSlab::vTakeBytes(i8 nBytes)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m_xWsp.vTakeHead(nBytes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* start of new row                                                           */
/*----------------------------------------------------------------------------*/
bool RowSlab::bNewRow(i8& i_Cell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  if (m_xWsp.nFree() < m_nMaxRowSize)
    return false;
  
  if (C_UNKNOWN == i_Cell)
    i_Cell = C_FIRST_CELL + 1;
    
  m_nItems++;
  
  i8 nCellBytes = m_nCols * C_BYTES_PER_WORD;
  m_xWsp.vTakeTail(nCellBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate column hints                                                      */
/*----------------------------------------------------------------------------*/
void RowSlab::vColHints(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
