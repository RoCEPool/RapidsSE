/*============================================================================*/
/* GenVar.cpp - Generic Variable  (which contains a nested value)             */
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
  
  xNestedValue.m_xHeader.vSetMagic();
  xNestedValue.m_xHeader.vSetClass(CN_NONE);
  
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
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Cell* aFipPtr)
{
  xNestedValue.m_xHeader.vSetClass(CN_FIPVAL);
  xNestedValue.m_wWord = (w8)aFipPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xNestedValue.m_xHeader.vSetClass(CN_INTVAL);
  xNestedValue.m_wWord = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object into workspace                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObject)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xNestedValue.m_xHeader.vSetClass(CN_PTRVAL);
  xNestedValue.m_wWord = (w8)pObject;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xNestedValue.m_xHeader.vSetClass(CN_STRVAL);
  xNestedValue.m_wWord = StrLib::nCopy(xNestedValue.m_caStr,szStz->aChar()) + 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get object address from nested value                                       */
/*----------------------------------------------------------------------------*/
Cell* GenVar::aFipPtr(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Cell* aFip = (Cell*)xNestedValue.m_wWord.m_iI8;
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* get integer from nested value                                              */
/*----------------------------------------------------------------------------*/
i8 GenVar::iInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 iInt = xNestedValue.m_wWord;
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
ForthValue* GenVar::aNestedValue(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  ForthValue* aValue = reinterpret_cast<ForthValue*>(&xNestedValue);
  
  return aValue;
}

/*----------------------------------------------------------------------------*/
/* get object address from nested value                                       */
/*----------------------------------------------------------------------------*/
Object* GenVar::pObject(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Object* pObj = (Object*)xNestedValue.m_wWord.m_iI8;
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 GenVar::nSize(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nSize = xNestedValue.m_wWord;
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stz from the nested value                                              */
/*----------------------------------------------------------------------------*/
Stz* GenVar::szStz(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Stz* sz = (Stz*)xNestedValue.m_caStr;
  
  return sz;
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
