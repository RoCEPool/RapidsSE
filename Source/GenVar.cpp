/*============================================================================*/
/* GenVar.cpp - Generic Variable  (which contains a nested value)             */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GenVar* SenVM::_pGenVar(void* aAddr)
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
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: GenVar class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenVar::GenVar(void)
: Object(CN_GENVAR)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vMagic();
  u.m_xNstVal.m_xHeader.vClass(CN_NONE);
  
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
void GenVar::operator =(Cell* aFip)
{
  u.m_xNstVal.m_xHeader.vClass(CN_FIPVAL);
  u.m_xNstVal.m_wWord = (w8)aFip;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vClass(CN_INTVAL);
  u.m_xNstVal.m_wWord = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object into workspace                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vClass(CN_PTRVAL);
  u.m_xNstVal.m_wWord = (w8)pObj;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vClass(CN_STRVAL);
  u.m_xNstVal.m_wWord = StrLib::nCopy(u.m_xNstVal.m_caStr,szStz->azChar()) + 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get azChar from the nested value                                           */
/*----------------------------------------------------------------------------*/
char* GenVar::azChar(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  char* az = (char*)u.m_xNstVal.m_caStr;
  
  return az;
}

/*----------------------------------------------------------------------------*/
/* get object address from nested value                                       */
/*----------------------------------------------------------------------------*/
Cell* GenVar::aFip(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Cell* aFip = (Cell*)u.m_xNstVal.m_wWord;
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* get integer from nested value                                              */
/*----------------------------------------------------------------------------*/
i8 GenVar::iInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 iInt = u.m_xNstVal.m_wWord;
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool GenVar::bIsInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  if (CN_INTVAL == u.m_xNstVal.m_xHeader.eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get class of embedded stack value                                          */
/*----------------------------------------------------------------------------*/
eCN GenVar::eEmbeddedClass(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  StkVal* aStkVal = aEmbeddedStkVal();
  
  eCN eClassNo = aStkVal->eClass();
  
  return eClassNo;
}

/*----------------------------------------------------------------------------*/
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
StkVal* GenVar::aEmbeddedStkVal(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  StkVal* aStkVal = reinterpret_cast<StkVal*>( & u.m_xNstVal );
  
  return aStkVal;
}

/*----------------------------------------------------------------------------*/
/* get object address from nested value                                       */
/*----------------------------------------------------------------------------*/
Object* GenVar::pObj(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Object* pObj = (Object*)u.m_xNstVal.m_wWord;
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 GenVar::nSize(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nSize = u.m_xNstVal.m_wWord;
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stz from the nested value                                              */
/*----------------------------------------------------------------------------*/
Stz* GenVar::szStz(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Stz* sz = (Stz*)u.m_xNstVal.m_caStr;
  
  return sz;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(GenVar)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
