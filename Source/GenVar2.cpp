/*============================================================================*/
/* GenVar2.cpp - Generic Variable  (which contains a nested value)             */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenVar2* SenVM::_pGenVar2(void* aAddr)
{
  GenVar2* pValWrk = reinterpret_cast<GenVar2*>(aAddr);
  
  RASSERT_TYPE(pValWrk,CN_GENVAR);
  
  return pValWrk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* GenVar2::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: GenVar2 class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenVar2::GenVar2(void)
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
GenVar2::~GenVar2(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void GenVar2::operator =(Cell* aFipPtr)
{
  xNestedValue.m_xHeader.vSetClass(CN_FIPVAL);
  xNestedValue.m_wWord = (w8)aFipPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void GenVar2::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xNestedValue.m_xHeader.vSetClass(CN_INTVAL);
  xNestedValue.m_wWord = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object into workspace                    */
/*----------------------------------------------------------------------------*/
void GenVar2::operator =(Object* pObject)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xNestedValue.m_xHeader.vSetClass(CN_PTRVAL);
  xNestedValue.m_wWord = (w8)pObject;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar2::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  xNestedValue.m_xHeader.vSetClass(CN_STRVAL);
  xNestedValue.m_wWord = StrLib::nCopy(xNestedValue.m_caStr,szStz->azChar()) + 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get object address from nested value                                       */
/*----------------------------------------------------------------------------*/
Cell* GenVar2::aFipPtr(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Cell* aFip = (Cell*)xNestedValue.m_wWord.m_iI8;
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* get integer from nested value                                              */
/*----------------------------------------------------------------------------*/
i8 GenVar2::iInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 iInt = xNestedValue.m_wWord;
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool GenVar2::bIsInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  if (CN_INTVAL == xNestedValue.m_xHeader.eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
StkVal* GenVar2::aNestedValue(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  StkVal* aValue = reinterpret_cast<StkVal*>(&xNestedValue);
  
  return aValue;
}

/*----------------------------------------------------------------------------*/
/* get object address from nested value                                       */
/*----------------------------------------------------------------------------*/
Object* GenVar2::pObject(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Object* pObj = (Object*)xNestedValue.m_wWord.m_iI8;
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 GenVar2::nSize(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nSize = xNestedValue.m_wWord;
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stz from the nested value                                              */
/*----------------------------------------------------------------------------*/
Stz* GenVar2::szStz(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Stz* sz = (Stz*)xNestedValue.m_caStr;
  
  return sz;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(GenVar2)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
