/*============================================================================*/
/* TypeValueArea.cpp -- Self-Describing / Dynamically Typed Value + Area      */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypeValueArea::TypeValueArea(void)
{
  #ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_SENCELL);
  #endif
 }

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypeValueArea::~TypeValueArea(void) noexcept(false)
{
 
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 TypeValueArea::iAsInt(void)
{
  return m_wxValue.iAsI8();
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypeValueArea::iBcd(void)
{
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vBcd(i8 iBcd)
{
  vType(CN_BCDVAL);
  m_wxValue.vPutColVal(iBcd);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypeValueArea::iBol(void)
{
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vBol(i8 iBcd)
{
  vType(CN_BOLVAL);
  m_wxValue.vPutColVal(iBcd);
}

/*----------------------------------------------------------------------------*/
/* set type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vInit(void)
{
  vType(CN_NONE);
  vInt(-1);

  m_caStringArea[0] = '\0';
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_SENCELL);
#endif

}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vStz(Stz* szStz, i8 iAllottedSize)
{
  vType(CN_STZVAL);
  
  i8 nBytesFollowing = StrLib::nCopy(m_caStringArea, iAllottedSize, szStz->azChar()) + 1;
  
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
/* get char*                                                                  */
/*----------------------------------------------------------------------------*/
char* TypeValueArea::azChar(void)
{
  ASSERT(CN_STZVAL == eType());

  return m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* copy source stack value into this one                                      */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vCopy(TypeValueArea* aSrcVal, i8 nBytes)
{
  if (! nBytes)
    nBytes = aSrcVal->nStackUsage();

  if (nBytes > 40000)
    MY::pTask()->vSeeFrameStack();

  MemLib::vCopy(this,aSrcVal,nBytes,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
CodeCell* TypeValueArea::aFip(void)
{
  return m_wxValue.aGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vFip(CodeCell* aFip)
{
  vType(CN_FIPVAL);
  m_wxValue.vPutColVal(aFip);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
d8 TypeValueArea::dFlt(void)
{
  return m_wxValue.dGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vFlt(d8 dFlt)
{
  vType(CN_FLTVAL);
  m_wxValue.vPutColVal(dFlt);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypeValueArea::iInt(void)
{
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vInt(i8 iInt)
{
  vType(CN_INTVAL);
  m_wxValue.vPutColVal(iInt);
}

/*----------------------------------------------------------------------------*/
/* set magic                                                                  */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vMagic(void)
{
  m_wxType.vSetMagic();
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
Object* TypeValueArea::pObj(void)
{
  return m_wxValue.pGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vObj(Object* pObject)
{
  vType(CN_PTRVAL);
  m_wxValue.vPutColVal(pObject);
}

/*----------------------------------------------------------------------------*/
/* get how many bytes are needed to push this StzVal on stack                 */
/*----------------------------------------------------------------------------*/
i8 TypeValueArea::nStackUsage(void)
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
Stz* TypeValueArea::szStz(void)
{
  ASSERT(CN_STZVAL == eType());
  return reinterpret_cast<Stz*>(m_caStringArea);
}

/*----------------------------------------------------------------------------*/
/* get size of str                                                            */
/*----------------------------------------------------------------------------*/
i8 TypeValueArea::nStzSize(void)
{
  ASSERT(CN_STZVAL == eType());
  return m_wxValue.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* set size of str                                                            */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vStzSize(i8 nSize)
{
  m_wxValue = nSize;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vToStr(String* psDst)
{
  if (CN_BOLVAL == eType())
    StrLib::vToString(psDst->azStr(), psDst->nAreaBytes(), iBol());
  else
  if (CN_INTVAL == eType())
    StrLib::vToString(psDst->azStr(), psDst->nAreaBytes(), iInt());
  else
  if (CN_STZVAL == eType())
    psDst->vCopy((Stz*) m_caStringArea);
  else
  if (CN_PTRVAL == eType())
    StrLib::vToPointer(psDst->azStr(), psDst->nAreaBytes(), pObj());
  else
    NEVER_REACH;

}

/*----------------------------------------------------------------------------*/
/* get type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
eCN TypeValueArea::eType(void)
{
  return (eCN)m_wxType.eGetClassNo();
}

/*----------------------------------------------------------------------------*/
/* set type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
void TypeValueArea::vType(eCN eType)
{
  m_wxType.vSetClassNo(eType);
}

/*----------------------------------------------------------------------------*/
/* get address of type+value                                                  */
/*----------------------------------------------------------------------------*/
TypeValue* TypeValueArea::aTypeValue(void)
{
  return this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
