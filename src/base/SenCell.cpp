/*============================================================================*/
/* CellSingle.cpp -- Segment of Cells                                        */
/*============================================================================*/

#include "BASE.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,SenCell,CN_CELLSINGLE)
__aNewFunction(SenCell,CN_CELLSINGLE)
__vDeleteFunction(SenCell,CN_CELLSINGLE)
__aFooterFunction(SenCell)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SenCell::SenCell(void)
: Object(CN_CELLSINGLE)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
    
  _checkInitialised_(CN_CELLSINGLE);
  
  _postcondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SenCell::~SenCell(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as TypedCell*                                   */
/*----------------------------------------------------------------------------*/
SenCell::operator TypeValueArea*(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  return & m_xTypeValueArea;
}

/*----------------------------------------------------------------------------*/
/* assignment operator (float)                                                */
/*----------------------------------------------------------------------------*/
void SenCell::operator =(d8 dColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(dColVal);
  m_xTypeValueArea.vType(CN_FLTVAL);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (integer)                                              */
/*----------------------------------------------------------------------------*/
void SenCell::operator =(i8 iColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(iColVal);
  
  m_xTypeValueArea.vType(CN_INTVAL);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (string)                                               */
/*----------------------------------------------------------------------------*/
void SenCell::operator =(Object* pObject)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  vPutColVal(pObject);

  m_xTypeValueArea.vType(CN_PTRVAL);
}

/*----------------------------------------------------------------------------*/
/* assignment operator (string)                                               */
/*----------------------------------------------------------------------------*/
void SenCell::operator =(Stz* szColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
//  StzVal* aStzVal = (StzVal*) & m_xTypedCell;
//
//  aStzVal->vStz(szColVal);
//
//  m_xTypedCell.vType(CN_STZVAL);
  
  m_xTypeValueArea.vStz(szColVal);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN SenCell::eAnyClass(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  return m_xTypeValueArea.eType();
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
char* SenCell::azChar(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return m_xTypeValueArea.m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
CodeCell* SenCell::aFip(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return m_xTypeValueArea.aFip();
}

/*----------------------------------------------------------------------------*/
/* get address of column cell                                                 */
/*----------------------------------------------------------------------------*/
Stz* SenCell::szStz(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return (Stz*) m_xTypeValueArea.m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
d8 SenCell::dGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  d8 dColVal = m_xTypeValueArea.m_wxValue.dGetColVal();
  
  return dColVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
i8 SenCell::iGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  i8 iColVal = m_xTypeValueArea.m_wxValue.iGetColVal();
  
  return iColVal;
}

/*----------------------------------------------------------------------------*/
/* get string value of given column                                           */
/*----------------------------------------------------------------------------*/
Stz* SenCell::szGetColVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  RASSERT(0);
  
  Stz* szColVal = m_xTypeValueArea.m_wxValue.szGetColVal_offset();

  return szColVal;
}

/*----------------------------------------------------------------------------*/
/* does cell contain an int val                                               */
/*----------------------------------------------------------------------------*/
bool SenCell::bIsIntVal(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  bool b = eAnyClass() == CN_INTVAL ? true : false;

  return b;
}

/*----------------------------------------------------------------------------*/
/* print the strings                                                          */
/*----------------------------------------------------------------------------*/
void SenCell::vPrintString(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  // ... TBC
}

/*----------------------------------------------------------------------------*/
/* put float value into given column                                          */
/*----------------------------------------------------------------------------*/
void SenCell::vPutColVal(d8 dColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_xTypeValueArea.m_wxValue.vPutColVal(dColVal);
}

/*----------------------------------------------------------------------------*/
/* put integer value of given column                                          */
/*----------------------------------------------------------------------------*/
void SenCell::vPutColVal(i8 iColVal)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_xTypeValueArea.m_wxValue.vPutColVal(iColVal);
}

/*----------------------------------------------------------------------------*/
/* put string value of given column                                           */
/*----------------------------------------------------------------------------*/
void SenCell::vPutColVal(Object* pObject)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  m_xTypeValueArea.m_wxValue.vPutColVal(pObject);
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void SenCell::vToString(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);
  
  eCN eClass = this->eAnyClass();
  
  switch (eClass) {
            
    case CN_FIPVAL: {
//      FipVal* aFipVal = _aFipVal(&m_xTypedCell.m_wxValue);
//      aFipVal->vToStz(szStz);
      m_xTypeValueArea.vToStz(szStz);
      break;
    }
      
    case CN_INTVAL: {
//      IntVal* aIntVal = _aIntVal(&m_xTypedCell.m_wxValue);
//      aIntVal->vToStz(szStz);
      m_xTypeValueArea.vToStz(szStz);
      break;
    }
      
    case CN_PTRVAL: {
//      PtrVal* aPtrVal = _aPtrVal(&m_xTypedCell.m_wxValue);
//      aPtrVal->vToStz(szStz);
      m_xTypeValueArea.vToStz(szStz);
      break;
    }
      
    case CN_STZVAL: {
      //StzVal* aStzVal = _aStzVal(&m_xTypedCell.m_wxValue);
      m_xTypeValueArea.vToStz(szStz);
      break;
    }
            
    default:
      vTrace("CellSingle::vToString -- invalid eClass %lld"_Stz,eClass);
      NEVER_REACH;
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get address of value nested in the wsp                                     */
/*----------------------------------------------------------------------------*/
TypeValue* SenCell::aTypeValue(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return m_xTypeValueArea.aTypeValue();
}

/*----------------------------------------------------------------------------*/
/* get address of value nested in the wsp                                     */
/*----------------------------------------------------------------------------*/
TypeValueArea* SenCell::aTypeValueArea(void)
{
  _precondition_(_MAGIC_,CN_CELLSINGLE,_FOOTER_);

  return & m_xTypeValueArea;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
