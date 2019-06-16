/*============================================================================*/
/* GenVar.cpp -- Generic Variable                                             */
/*============================================================================*/

#include "Forth.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenVar* Forth::_pGenVar(void* aAddr)
{
  GenVar* pValWrk = reinterpret_cast<GenVar*>(aAddr);
  
  RASSERT_TYPE(pValWrk,CN_GENVAR);
  
  return pValWrk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* GenVar::operator new(size_t)
{
  vFatal("fatal: GenVar class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenVar::GenVar(void)
: Object(CN_GENVAR)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xValue.m_xHeader.vSetMagic();
  xValue.m_xHeader.vSetClass(CN_NONE);
  
  _postcondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GenVar::~GenVar(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign/copy a float into workspace                                         */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(d8 dFlt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xValue.m_xHeader.vSetClass(CN_FLTVAL);
  xValue.m_xDtm.m_dFlt = dFlt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xValue.m_xHeader.vSetClass(CN_INTVAL);
  xValue.m_xDtm.m_iInt = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object into workspace                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObject)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xValue.m_xHeader.vSetClass(CN_PTRVAL);
  xValue.m_xDtm.m_pObject = pObject;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xValue.m_xHeader.vSetClass(CN_STRVAL);
  xValue.m_xDtm.m_nSize = StrLib::nCopy(xValue.m_caStr,szStz->aChar()) + 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 GenVar::iInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 iInt = xValue.m_xDtm.m_iInt;
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* get stz in the wsp area                                                    */
/*----------------------------------------------------------------------------*/
Stz* GenVar::szStz(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Stz* sz = (Stz*)xValue.m_caStr;
  
  return sz;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* get address of the intval in the wsp area                                  */
/*----------------------------------------------------------------------------*/
IntVal* GenVar::aNestedIntVal(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  IntVal* aNestedIntVal = reinterpret_cast<IntVal*>(aNestedValue());
  
  return aNestedIntVal;
}

/*----------------------------------------------------------------------------*/
/* get address of the ptrval in the wsp area                                  */
/*----------------------------------------------------------------------------*/
PtrVal* GenVar::aNestedPtrVal(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  PtrVal* aNestedPtrVal = reinterpret_cast<PtrVal*>(aNestedValue());
  
  return aNestedPtrVal;
}

/*----------------------------------------------------------------------------*/
/* get address of the value in the wsp area                                   */
/*----------------------------------------------------------------------------*/
Value* GenVar::aNestedValue(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
 
  Value* aNestedValue = reinterpret_cast<Value*>(&xValue);

  return aNestedValue;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* GenVar::aFooter(void)
{
  _precondition_(_MAGIC_,CN_GENVAR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
