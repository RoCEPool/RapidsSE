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
  
  u.m_xAnyVal.vMagic();
  u.m_xAnyVal.vClass(CN_NONE);
  
  _postcondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenVar::GenVar(AnyVal* aAnyVal)
: Object(CN_GENVAR)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xAnyVal.vMagic();
  u.m_xAnyVal.vClass(CN_NONE);
  
  eCN eClassNo = aAnyVal->eClass();
  
  switch (eClassNo) {
    case CN_FIPVAL: u.m_xFipVal.vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: u.m_xIntVal.vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: u.m_xPtrVal.vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: u.m_xStrVal.vStz(aAnyVal->szStz());   break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClassNo);
  }

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
/* auto conversion operator : as AnyVal*                                      */
/*----------------------------------------------------------------------------*/
GenVar::operator AnyVal*(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);

  return & u.m_xAnyVal;
}


/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);

  eCN eClass = aAnyVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL: u.m_xFipVal.vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: u.m_xIntVal.vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: u.m_xPtrVal.vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: u.m_xStrVal.vStz(aAnyVal->szStz());   break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
  NEVER_REACH;
}



/*----------------------------------------------------------------------------*/
/* assign/copy a forth instruction pointer                                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Cell* aFip)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xFipVal.vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xIntVal.vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xPtrVal.vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xStrVal.vStz(szStz);
  
  _postcondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* GenVar::azChar(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  char* az = u.m_xStrVal.azChar();

  return az;
}

/*----------------------------------------------------------------------------*/
/* copy given source value into this var                                      */
/*----------------------------------------------------------------------------*/
void GenVar::vCopy(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xAnyVal.vCopy(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Cell* GenVar::aFip(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Cell* aFip = u.m_xFipVal.aFip();

  return aFip;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 GenVar::iInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 iInt = u.m_xIntVal.iInt();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool GenVar::bIsInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  if (CN_INTVAL == u.m_xAnyVal.eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
AnyVal* GenVar::aEmbeddedAnyVal(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  return & u.m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* get object address                                                         */
/*----------------------------------------------------------------------------*/
Object* GenVar::pObj(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Object* pObj = u.m_xPtrVal.pObj();

  return pObj;
}

/*----------------------------------------------------------------------------*/
/* get bytes required to push value on stack                                  */
/*----------------------------------------------------------------------------*/
i8 GenVar::nStackUsage(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nBytes = u.m_xStrVal.nStackUsage();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* GenVar::szStz(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Stz* sz = u.m_xStrVal.szStz();

  return sz;
}

/*----------------------------------------------------------------------------*/
/* get size of nested string value                                            */
/*----------------------------------------------------------------------------*/
i8 GenVar::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nSize = u.m_xStrVal.nStzSize();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* convert to stz string                                                      */
/*----------------------------------------------------------------------------*/
void GenVar::vToString(Stz* szString)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xAnyVal.vToString(szString);
}

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN GenVar::eValClass(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  return u.m_xAnyVal.eClass();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(GenVar)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
