/*============================================================================*/
/* CellSegment.cpp -- Segment of Cells                                        */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Rows,CellSegment,CN_CELLSEGMENT)
__aNewFunction(CellSegment,CN_CELLSEGMENT)
__vDeleteFunction(CellSegment,CN_CELLSEGMENT)
__aFooterFunction(CellSegment)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CellSegment::CellSegment(eSI eStringSemantics)
: Object(CN_CELLSEGMENT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  m_eStringSemantics = eStringSemantics;
  m_i_CellDeposit    = C_UNKNOWN;
  m_azStringAdd      = m_caStringArea;
  
  * m_azStringAdd = '\0';

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_wxaCellArray,sizeof(m_wxaCellArray),CN_CELLSEGMENT);
  MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_CELLSEGMENT);
#endif
  
  _checkInitialised_(CN_CELLSEGMENT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CellSegment::~CellSegment(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of colun cell                                                  */
/*----------------------------------------------------------------------------*/
Dtm* CellSegment::aColumCell(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  Dtm* aColCell = & m_wxaCellArray[i_Col];
  
  return aColCell;
}

/*----------------------------------------------------------------------------*/
/* deposit float value into column by cursor                                  */
/*----------------------------------------------------------------------------*/
void CellSegment::vDepositColVal(d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  ++ m_i_CellDeposit;
  
  vPutColVal(m_i_CellDeposit,dColVal);
}

/*----------------------------------------------------------------------------*/
/* deposit integer value into column by cursor                                */
/*----------------------------------------------------------------------------*/
void CellSegment::vDepositColVal(i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  ++ m_i_CellDeposit;
  
  vPutColVal(m_i_CellDeposit,iColVal);
}

/*----------------------------------------------------------------------------*/
/* deposit string value into column by cursor                                 */
/*----------------------------------------------------------------------------*/
void CellSegment::vDepositColVal(Stz* szColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  ++ m_i_CellDeposit;
  
  vPutColVal(m_i_CellDeposit,szColVal);
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
d8 CellSegment::dGetColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  d8 dColVal = m_wxaCellArray[i_Col].m_wFlt;
  
  return dColVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
i8 CellSegment::iGetColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  i8 iColVal = m_wxaCellArray[i_Col].m_wInt;
  
  return iColVal;
}

/*----------------------------------------------------------------------------*/
/* get string value of given column                                           */
/*----------------------------------------------------------------------------*/
Stz* CellSegment::szGetColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* print the strings                                                          */
/*----------------------------------------------------------------------------*/
void CellSegment::vPrintStrings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);

  // ... TBC
}

/*----------------------------------------------------------------------------*/
/* put float value into given column                                          */
/*----------------------------------------------------------------------------*/
void CellSegment::vPutColVal(i8 i_Col, d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  m_wxaCellArray[i_Col].m_wFlt = dColVal;
}

/*----------------------------------------------------------------------------*/
/* put integer value of given column                                          */
/*----------------------------------------------------------------------------*/
void CellSegment::vPutColVal(i8 i_Col, i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  m_wxaCellArray[i_Col].m_wInt = iColVal;
}

/*----------------------------------------------------------------------------*/
/* put string value of given column                                           */
/*----------------------------------------------------------------------------*/
void CellSegment::vPutColVal(i8 i_Col, Stz* szColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  // depends on the semantics
  
  Stz* szDst = szStringAdd_begin();
  
  i8 nStringSize = StrLib::nCopy((char*)szDst,(char*)szColVal) + 1;
  
  vStringAdd_end(nStringSize);
  
  Dtm* aCell = & m_wxaCellArray[i_Col];
  
  aCell->vSetStz_offset(szDst);
}

/*----------------------------------------------------------------------------*/
/* reset this cell segment                                                    */
/*----------------------------------------------------------------------------*/
void CellSegment::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);

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
Stz* CellSegment::szStringAdd_begin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  return (Stz*)m_azStringAdd;
}

/*----------------------------------------------------------------------------*/
/* end add of string                                                          */
/*----------------------------------------------------------------------------*/
void CellSegment::vStringAdd_end(i8 nStringSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CELLSEGMENT,_FOOTER_);
  
  m_azStringAdd += nStringSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
