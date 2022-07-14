/*============================================================================*/
/* TypeValueDscr.cpp -- Type + Value + Descriptor                             */
/*============================================================================*/

#include "BASE.h"

#include "TypeValueDscr.hpp"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeValueDscr::TypeValueDscr(void)
{
  m_pLocalVars = nullptr;
  m_i_Local    = C_UNKNOWN;
  
  m_bLocalStr  = false;

  m_pLocalStr  = nullptr;
  
  #ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_caArea_tbr,sizeof(m_caArea_tbr),CN_SENCELL);
  #endif
 }

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypeValueDscr::~TypeValueDscr(void) noexcept(false)
{
 
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get pointer to string area                                                 */
/*----------------------------------------------------------------------------*/
char* TypeValueDscr::azArea(void)
{
  return m_caArea_tbr;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 TypeValueDscr::iAsInt(void)
{
  return m_wxValue.iAsI8();
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypeValueDscr::iBcd(void)
{
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vBcd(i8 iBcd)
{
  vType(CN_BCDVAL);
  m_wxValue.vPutColVal(iBcd);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypeValueDscr::iBol(void)
{
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vBol(i8 iBcd)
{
  vType(CN_BOLVAL);
  m_wxValue.vPutColVal(iBcd);
}

/*----------------------------------------------------------------------------*/
/* get char*                                                                  */
/*----------------------------------------------------------------------------*/
char* TypeValueDscr::azChar(void)
{
  ASSERT(CN_STZVAL == eType());

  return m_caArea_tbr;
}

/*----------------------------------------------------------------------------*/
/* set type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vInit(void)
{
  vType(CN_NONE);
  vInt(-1);

  m_caArea_tbr[0] = '\0';
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caArea_tbr,sizeof(m_caArea_tbr),CN_SENCELL);
#endif

}

/*----------------------------------------------------------------------------*/
/* copy source stack value into this one                                      */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vCopy(TypeValueDscr* aSrcVal, i8 nBytes)
{
  if (! nBytes)
    nBytes = aSrcVal->nStackUsage();
  
  MemLib::vCopy(this,aSrcVal,nBytes,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
CodeCell* TypeValueDscr::aFip(void)
{
  return m_wxValue.aGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vFip(CodeCell* aFip)
{
  vType(CN_FIPVAL);
  m_wxValue.vPutColVal(aFip);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
d8 TypeValueDscr::dFlt(void)
{
  return m_wxValue.dGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vFlt(d8 dFlt)
{
  vType(CN_FLTVAL);
  m_wxValue.vPutColVal(dFlt);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypeValueDscr::iInt(void)
{
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vInt(i8 iInt)
{
  vType(CN_INTVAL);
  m_wxValue.vPutColVal(iInt);
}

/*----------------------------------------------------------------------------*/
/* set magic                                                                  */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vMagic(void)
{
  m_wxType.vSetMagic();
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
Object* TypeValueDscr::pObj(void)
{
  return m_wxValue.pGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vObj(Object* pObject)
{
  vType(CN_PTRVAL);
  m_wxValue.vPutColVal(pObject);
}

/*----------------------------------------------------------------------------*/
/* get how many bytes are needed to push this StzVal on stack                 */
/*----------------------------------------------------------------------------*/
i8 TypeValueDscr::nStackUsage(void)
{
  i8 nBytes;
  
  if (CN_STZVAL == eType())
    nBytes = sizeof(m_wxType)
               + sizeof(m_wxValue)
               + m_wxValue.iGetColVal();
  else
    nBytes = sizeof(m_wxType)
               + sizeof(m_wxValue);

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get adddress of Stz area                                                   */
/*----------------------------------------------------------------------------*/
Stz* TypeValueDscr::szStz(void)
{
  ASSERT(CN_STZVAL == eType());
  return reinterpret_cast<Stz*>(m_caArea_tbr);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vStz(Stz* szStz)
{
  vType(CN_STZVAL);
  
  i8 nBytesFollowing = StrLib::nCopy(m_caArea_tbr,szStz->azChar()) + 1;
  
  m_wxValue = nBytesFollowing;
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
//void TypedCell::vStz(char* acnChar, i8 nLength)
//{
//  vType(CN_STZVAL);
//
//  m_nBytesFollowing = StrLib::nCopy(m_caStz,acnChar) + 1;
//}

/*----------------------------------------------------------------------------*/
/* get size of str                                                            */
/*----------------------------------------------------------------------------*/
i8 TypeValueDscr::nStzSize(void)
{
  ASSERT(CN_STZVAL == eType());
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* set size of str                                                            */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vStzSize(i8 nSize)
{
  m_wxValue = nSize;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vToStz(Stz* szDst)
{
  if (CN_INTVAL == eType())
    * szDst = iInt();
  else
  if (CN_STZVAL == eType())
    StrLib::vCopy(szDst->azChar(),m_caArea_tbr);
  else
  if (CN_PTRVAL == eType())
    * szDst = reinterpret_cast<void*>(pObj());
  else
    NEVER_REACH;

}

/*----------------------------------------------------------------------------*/
/* get type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
eCN TypeValueDscr::eType(void)
{
  return (eCN)m_wxType.eGetClassNo();
}

/*----------------------------------------------------------------------------*/
/* set type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
void TypeValueDscr::vType(eCN eType)
{
  m_wxType.vSetClassNo(eType);
}

/*----------------------------------------------------------------------------*/
/* get address of type+value                                                  */
/*----------------------------------------------------------------------------*/
TypeValue* TypeValueDscr::aTypeValue(void)
{
  return this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
