/*============================================================================*/
/* CellSingle.cpp -- Segment of Cells                                        */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Rows,CellSingle,CN_CELLSINGLE)
__aNewFunction(CellSingle,CN_CELLSINGLE)
__vDeleteFunction(CellSingle,CN_CELLSINGLE)
__aFooterFunction(CellSingle)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CellSingle::CellSingle(eSI eStringSemantics)
: Object(CN_CELLSINGLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_eStringSemantics = eStringSemantics;
  m_i_CellDeposit    = C_UNKNOWN;
  m_azStringAdd      = m_caStringArea;
  
  * m_azStringAdd = '\0';

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_wxaCellArray,sizeof(m_wxaCellArray),CN_CELLSINGLE);
  MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_CELLSINGLE);
#endif
  
  _checkInitialised_(CN_CELLSINGLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CellSingle::~CellSingle(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
Dtm* CellSingle::aColumCell(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  Dtm* aColCell = & m_wxaCellArray[i_Col];
  
  return aColCell;
}

/*----------------------------------------------------------------------------*/
/* deposit float value into column by cursor                                  */
/*----------------------------------------------------------------------------*/
void CellSingle::vDepositColVal(d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  ++ m_i_CellDeposit;
  
  vPutColVal(m_i_CellDeposit,dColVal);
}

/*----------------------------------------------------------------------------*/
/* deposit integer value into column by cursor                                */
/*----------------------------------------------------------------------------*/
void CellSingle::vDepositColVal(i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  ++ m_i_CellDeposit;
  
  vPutColVal(m_i_CellDeposit,iColVal);
}

/*----------------------------------------------------------------------------*/
/* deposit string value into column by cursor                                 */
/*----------------------------------------------------------------------------*/
void CellSingle::vDepositColVal(Stz* szColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  ++ m_i_CellDeposit;
  
  vPutColVal(m_i_CellDeposit,szColVal);
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
d8 CellSingle::dGetColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  d8 dColVal = m_wxaCellArray[i_Col].dGetColVal();
  
  return dColVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
i8 CellSingle::iGetColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  i8 iColVal = m_wxaCellArray[i_Col].iGetColVal();
  
  return iColVal;
}

/*----------------------------------------------------------------------------*/
/* get string value of given column                                           */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szGetColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  Stz* szColVal = m_wxaCellArray[i_Col].szGetColVal_offset();

  return szColVal;
}

/*----------------------------------------------------------------------------*/
/* print the strings                                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPrintStrings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  // ... TBC
}

/*----------------------------------------------------------------------------*/
/* put float value into given column                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(i8 i_Col, d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_wxaCellArray[i_Col].vPutColVal(dColVal);
}

/*----------------------------------------------------------------------------*/
/* put integer value of given column                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(i8 i_Col, i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_wxaCellArray[i_Col].vPutColVal(iColVal);
}

/*----------------------------------------------------------------------------*/
/* put string value of given column                                           */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(i8 i_Col, Stz* szColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  // depends on the semantics
  
  Stz* StzBegin = szStringAdd_begin();
  
  i8 nStringSize = m_wxaCellArray[i_Col].nPutColVal_offset(szColVal,StzBegin);
  
  vStringAdd_end(nStringSize);
}

/*----------------------------------------------------------------------------*/
/* reset this cell segment                                                    */
/*----------------------------------------------------------------------------*/
void CellSingle::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  m_i_CellDeposit = C_UNKNOWN;
  m_azStringAdd = m_caStringArea;
  
  * m_azStringAdd = '\0';
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin add of string                                                        */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szStringAdd_begin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  return (Stz*)m_azStringAdd;
}

/*----------------------------------------------------------------------------*/
/* end add of string                                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vStringAdd_end(i8 nStringSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_azStringAdd += nStringSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
