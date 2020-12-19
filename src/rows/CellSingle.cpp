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
CellSingle::CellSingle(void)
: Object(CN_CELLSINGLE)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_eStringSemantics = SI_NONE;;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(&AnyVal_DtmCell.m_wxCell,
                sizeof(AnyVal_DtmCell.m_wxCell),
                CN_CELLSINGLE);
  MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_CELLSINGLE);
#endif
  
  _checkInitialised_(CN_CELLSINGLE);
  
  _postcondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CellSingle::CellSingle(eSI eStringSemantics)
: Object(CN_CELLSINGLE)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_eStringSemantics = eStringSemantics;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(&AnyVal_DtmCell.m_wxCell,
                sizeof(AnyVal_DtmCell.m_wxCell),
                CN_CELLSINGLE);
  MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_CELLSINGLE);
#endif
  
  _checkInitialised_(CN_CELLSINGLE);
  
  _postcondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CellSingle::~CellSingle(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as AnyVal*                                      */
/*----------------------------------------------------------------------------*/
CellSingle::operator AnyVal*(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  return (AnyVal*) & AnyVal_DtmCell;
}

/*----------------------------------------------------------------------------*/
/* assignment operator (float)                                                */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(d8 dColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(dColVal);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (integer)                                              */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(i8 iColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(iColVal);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (string)                                               */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(Stz* szColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(szColVal);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
Dtm* CellSingle::aCellAddress(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  Dtm* aColCell = & AnyVal_DtmCell.m_wxCell;
  
  return aColCell;
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
char* CellSingle::azChar(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szStz(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return (Stz*) m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
d8 CellSingle::dGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  d8 dColVal = AnyVal_DtmCell.m_wxCell.dGetColVal();
  
  return dColVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
i8 CellSingle::iGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  i8 iColVal = AnyVal_DtmCell.m_wxCell.iGetColVal();
  
  return iColVal;
}

/*----------------------------------------------------------------------------*/
/* get string value of given column                                           */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  Stz* szColVal = AnyVal_DtmCell.m_wxCell.szGetColVal_offset();

  return szColVal;
}

/*----------------------------------------------------------------------------*/
/* print the strings                                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPrintString(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  // ... TBC
}

/*----------------------------------------------------------------------------*/
/* put float value into given column                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(d8 dColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  AnyVal_DtmCell.m_wxCell.vPutColVal(dColVal);
}

/*----------------------------------------------------------------------------*/
/* put integer value of given column                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(i8 iColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  AnyVal_DtmCell.m_wxCell.vPutColVal(iColVal);
}

/*----------------------------------------------------------------------------*/
/* put string value of given column                                           */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(Stz* szColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  // depends on the semantics
  
  Stz* StzBegin = szStringAdd_begin();
  
  i8 nStringSize =
    AnyVal_DtmCell.m_wxCell.nPutColVal_offset(szColVal,StzBegin);
  
  vStringAdd_end(nStringSize);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin add of string                                                        */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szStringAdd_begin(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_caStringArea[0] = '\0';
  return (Stz*)m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* end add of string                                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vStringAdd_end(i8 nStringSize)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
