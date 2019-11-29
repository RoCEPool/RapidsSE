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
  
  u.m_xStkVal.vMagic();
  u.m_xStkVal.vClass(CN_NONE);
  
  _postcondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GenVar::GenVar(StkVal* aStkVal)
: Object(CN_GENVAR)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xStkVal.vMagic();
  u.m_xStkVal.vClass(CN_NONE);
  
  eCN eClassNo = aStkVal->eClass();
  
  switch (eClassNo) {
    case CN_FIPVAL: u.m_xFipVal = aStkVal->aFip();    break;
    case CN_INTVAL: u.m_xIntVal = aStkVal->iInt();    break;
    case CN_PTRVAL: u.m_xPtrVal = aStkVal->pObj();    break;
    case CN_STRVAL: u.m_xStrVal.vAssign(aStkVal->szStz());  break;
      
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
  
  return _aAnyVal( & u.m_xStkVal );
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as StkVal*                                      */
/*----------------------------------------------------------------------------*/
GenVar::operator StkVal*(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);

  return & u.m_xStkVal;
}


/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(StkVal* aStkVal)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);

  eCN eClass = aStkVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL: u.m_xFipVal = aStkVal->aFip();    break;
    case CN_INTVAL: u.m_xIntVal = aStkVal->iInt();    break;
    case CN_PTRVAL: u.m_xPtrVal = aStkVal->pObj();    break;
    case CN_STRVAL: u.m_xStrVal.vAssign(aStkVal->szStz());  break;
      
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
  
  u.m_xFipVal = aFip;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xPtrVal = pObj;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xStrVal.vAssign(szStz);
  
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
void GenVar::vCopy(StkVal* aStkVal)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xStkVal.vCopy(aStkVal);
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
  
  if (CN_INTVAL == u.m_xStkVal.eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
StkVal* GenVar::aEmbeddedStkVal(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  return & u.m_xStkVal;
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
i8 GenVar::nStackBytes(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nBytes = u.m_xStrVal.nStackBytes();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 GenVar::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nSize = u.m_xStrVal.nSize();

  return nSize;
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
/* convert to stz string                                                      */
/*----------------------------------------------------------------------------*/
void GenVar::vToString(Stz* szString)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xStkVal.vToString(szString);
}

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN GenVar::eValClass(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  return u.m_xStkVal.eClass();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(GenVar)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
