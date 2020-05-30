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
  
  m_nMaxRowSize = 0;

  m_nCols  = 0;
  for (i8 i_ColHint = 0; i_ColHint < C_MAX_DEFCOLUMNS; i_ColHint++)
    m_xaColHint[i_ColHint] = 0;
  
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
i8 RowSlab::iCell(i8 i_Row, i8 i_Cell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* deposit an i8 value                                                        */
/*----------------------------------------------------------------------------*/
void RowSlab::vDeposit(i8& i_Cell, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  
}

/*----------------------------------------------------------------------------*/
/* begin deposit of string                                                    */
/*----------------------------------------------------------------------------*/
char* RowSlab::azString(i8& i_Cell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* take bytes for deposited string                                            */
/*----------------------------------------------------------------------------*/
void RowSlab::vTakeBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

}

/*----------------------------------------------------------------------------*/
/* start of new row                                                           */
/*----------------------------------------------------------------------------*/
bool RowSlab::bNewRow(i8& i_Cell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

  if (C_UNKNOWN == i_Cell)
    i_Cell = C_FIRST_CELL;
  else
    i_Cell--;
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
