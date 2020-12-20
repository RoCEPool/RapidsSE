/*============================================================================*/
/* TypedCell.cpp -- Self-Describing / Dynamically Typed Cell                  */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TypedCell::TypedCell(void)
{
  #ifdef WITH_CHECK_INITIALISED
    MemLib::vZero(m_caStringArea,sizeof(m_caStringArea),CN_CELLSINGLE);
  #endif
 }

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TypedCell::~TypedCell(void) noexcept(false)
{
 
}

/*----------------------------------------------------------------------------*/
/* get type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
eCN TypedCell::eType(void)
{
  return (eCN)m_wxType.eClass();
}

/*----------------------------------------------------------------------------*/
/* set type/class of embedded value                                           */
/*----------------------------------------------------------------------------*/
void TypedCell::vType(eCN eType)
{
  m_wxType.vClass(eType);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void TypedCell::vStz(Stz* szStz)
{
  vType(CN_STZVAL);
  
  i8 nBytesFollowing = StrLib::nCopy(m_caStringArea,szStz->azChar()) + 1;
  
  m_wxValueCell = nBytesFollowing;
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
char* TypedCell::azChar(void)
{
  ASSERT(CN_STZVAL == eType());

  return m_caStringArea;
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
ForthCell* TypedCell::aFip(void)
{
  return m_wxValueCell.aGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypedCell::vFip(ForthCell* aFip)
{
  vType(CN_FIPVAL);
  m_wxValueCell.vPutColVal(aFip);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
i8 TypedCell::iInt(void)
{
  return m_wxValueCell.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypedCell::vInt(i8 iInt)
{
  vType(CN_INTVAL);
  m_wxValueCell.vPutColVal(iInt);
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
Object* TypedCell::pObj(void)
{
  return m_wxValueCell.pGetColVal();
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void TypedCell::vObj(Object* pObject)
{
  vType(CN_PTRVAL);
  m_wxValueCell.vPutColVal(pObject);
}

/*----------------------------------------------------------------------------*/
/* get how many bytes are needed to push this StzVal on stack                 */
/*----------------------------------------------------------------------------*/
i8 TypedCell::nStackUsage(void)
{
  i8 nBytes;
  
  if (CN_STZVAL == eType())
    nBytes = sizeof(m_wxType)
               + sizeof(m_wxValueCell)
               + m_wxValueCell.iGetColVal();
  else
    nBytes = sizeof(m_wxType)
               + sizeof(m_wxValueCell);

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get adddress of Stz area                                                   */
/*----------------------------------------------------------------------------*/
Stz* TypedCell::szStz(void)
{
  ASSERT(CN_STZVAL == eType());
  return reinterpret_cast<Stz*>(m_caStringArea);
}

/*----------------------------------------------------------------------------*/
/* get size of str                                                            */
/*----------------------------------------------------------------------------*/
i8 TypedCell::nStzSize(void)
{
  ASSERT(CN_STZVAL == eType());
  return m_wxValueCell.iGetColVal();
}

/*----------------------------------------------------------------------------*/
/* set size of str                                                            */
/*----------------------------------------------------------------------------*/
void TypedCell::vStzSize(i8 nSize)
{
  m_wxValueCell = nSize;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void TypedCell::vToStz(Stz* szDst)
{
  if (CN_STZVAL == eType())
    StrLib::vCopy(szDst->azChar(),m_caStringArea);
  else
  if (CN_PTRVAL == eType())
    * szDst = reinterpret_cast<void*>(pObj());
  else
    NEVER_REACH;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
