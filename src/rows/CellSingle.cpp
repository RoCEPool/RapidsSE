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
/* auto conversion operator : as TypedCell*                                   */
/*----------------------------------------------------------------------------*/
CellSingle::operator TypedCell*(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  return & m_xTypedCell;
}

/*----------------------------------------------------------------------------*/
/* assignment operator (float)                                                */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(d8 dColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(dColVal);
  m_xTypedCell.vType(CN_FLTVAL);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (integer)                                              */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(i8 iColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(iColVal);
  
  m_xTypedCell.vType(CN_INTVAL);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (string)                                               */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(Object* pObject)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(pObject);

  m_xTypedCell.vType(CN_PTRVAL);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (string)                                               */
/*----------------------------------------------------------------------------*/
void CellSingle::operator =(Stz* szColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
//  StzVal* aStzVal = (StzVal*) & m_xTypedCell;
//
//  aStzVal->vStz(szColVal);
//
//  m_xTypedCell.vType(CN_STZVAL);
  
  m_xTypedCell.vStz(szColVal);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN CellSingle::eAnyClass(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  return m_xTypedCell.eType();
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
Dtm* CellSingle::aCellAddress(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  Dtm* aColCell = & m_xTypedCell.m_wxValueCell;
  
  return aColCell;
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
char* CellSingle::azChar(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return m_xTypedCell.m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
ForthCell* CellSingle::aFip(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return m_xTypedCell.aFip();
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szStz(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return (Stz*) m_xTypedCell.m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
d8 CellSingle::dGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  d8 dColVal = m_xTypedCell.m_wxValueCell.dGetColVal();
  
  return dColVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
i8 CellSingle::iGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  i8 iColVal = m_xTypedCell.m_wxValueCell.iGetColVal();
  
  return iColVal;
}

/*----------------------------------------------------------------------------*/
/* get string value of given column                                           */
/*----------------------------------------------------------------------------*/
Stz* CellSingle::szGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  RASSERT(0);
  
  Stz* szColVal = m_xTypedCell.m_wxValueCell.szGetColVal_offset();

  return szColVal;
}

/*----------------------------------------------------------------------------*/
/* does cell contain an int val                                               */
/*----------------------------------------------------------------------------*/
bool CellSingle::bIsIntVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  bool b = eAnyClass() == CN_INTVAL ? true : false;

  return b;
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
  
  m_xTypedCell.m_wxValueCell.vPutColVal(dColVal);
}

/*----------------------------------------------------------------------------*/
/* put integer value of given column                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(i8 iColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_xTypedCell.m_wxValueCell.vPutColVal(iColVal);
}

/*----------------------------------------------------------------------------*/
/* put string value of given column                                           */
/*----------------------------------------------------------------------------*/
void CellSingle::vPutColVal(Object* pObject)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_xTypedCell.m_wxValueCell.vPutColVal(pObject);
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void CellSingle::vToString(Stz* szStz)
{
  eCN eClass = this->eAnyClass();
  
  switch (eClass) {
            
    case CN_FIPVAL: {
//      FipVal* aFipVal = _aFipVal(&m_xTypedCell.m_wxValueCell);
//      aFipVal->vToStz(szStz);
      m_xTypedCell.vToStz(szStz);
      break;
    }
      
    case CN_INTVAL: {
//      IntVal* aIntVal = _aIntVal(&m_xTypedCell.m_wxValueCell);
//      aIntVal->vToStz(szStz);
      m_xTypedCell.vToStz(szStz);
      break;
    }
      
    case CN_PTRVAL: {
//      PtrVal* aPtrVal = _aPtrVal(&m_xTypedCell.m_wxValueCell);
//      aPtrVal->vToStz(szStz);
      m_xTypedCell.vToStz(szStz);
      break;
    }
      
    case CN_STZVAL: {
      //StzVal* aStzVal = _aStzVal(&m_xTypedCell.m_wxValueCell);
      m_xTypedCell.vToStz(szStz);
      break;
    }
            
    default:
      vTrace("CellSingle::vToString -- invalid eClass %lld"_Stz,eClass);
      NEVER_REACH;
      break;
  }
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
