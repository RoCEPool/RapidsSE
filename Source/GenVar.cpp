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
/* assign/copy a forth instruction pointer                                    */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Cell* aFip)
{
  u.m_xNstVal.m_xHeader.vClass(CN_FIPVAL);
  u.m_xNstVal.m_wWord = (w8)aFip;
  
//  u.m_xFipVar = aFipPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vClass(CN_INTVAL);
  u.m_xNstVal.m_wWord = iInt;
  
//  u.m_xIntVar = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vClass(CN_PTRVAL);
  u.m_xNstVal.m_wWord = (w8)pObj;
  
//  u.m_xPtrVar = pObj;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void GenVar::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  u.m_xNstVal.m_xHeader.vClass(CN_STRVAL);
  u.m_xNstVal.m_wWord = StrLib::nCopy(u.m_xNstVal.m_caStr,szStz->azChar()) + 1;
  
//  u.m_xStrVar = szStz;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* GenVar::azChar(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  char* az = (char*)u.m_xNstVal.m_caStr;
  
//  char* az = u.m_xStrVar.azChar();

  return az;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Cell* GenVar::aFip(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Cell* aFip = (Cell*)u.m_xNstVal.m_wWord;
  
//  Cell* aFip = u.m_xFipVar.aFip();

  return aFip;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                               */
/*----------------------------------------------------------------------------*/
i8 GenVar::iInt(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 iInt = u.m_xNstVal.m_wWord;
  
//  i8 iInt = u.m_xIntVar.iInt();
  
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
/* get object address                                                         */
/*----------------------------------------------------------------------------*/
Object* GenVar::pObj(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Object* pObj = (Object*)u.m_xNstVal.m_wWord;
  
//  Object* pObj = u.m_xPtrVar.pObj();

  return pObj;
}

/*----------------------------------------------------------------------------*/
/* get address of the embedded sen variable                                   */
/*----------------------------------------------------------------------------*/
SenVar* GenVar::aEmbeddedSenVar(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  SenVar* aSenVar = & u.m_xSenVar;
  
  return aSenVar;
}

/*----------------------------------------------------------------------------*/
/* get size of of embedded value on stack                                     */
/*----------------------------------------------------------------------------*/
i8 GenVar::nStackBytes(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  i8 nBytes = u.m_xSenVar.nStackBytes();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 GenVar::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  //i8 nSize = u.m_xNstVal.m_wWord;
  
  i8 nSize = u.m_xStrVal.nSize();

  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* GenVar::szStz(void)
{
  _precondition_(_MAGIC_,CN_GENVAR,_FOOTER_);
  
  Stz* sz = (Stz*)u.m_xNstVal.m_caStr;
  
//  Stz* sz = u.m_xStrVar.szStz();

  return sz;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(GenVar)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
