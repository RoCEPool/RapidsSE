/*============================================================================*/
/* BfrBuffer.cpp -- Bloom Filter Buffer                                       */
/*============================================================================*/

#include "BASE.h"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,BfrBuffer,CN_BFRBUFFER)
__aNewFunction(BfrBuffer,CN_BFRBUFFER)
__vDeleteFunction(BfrBuffer,CN_BFRBUFFER)
__aFooterFunction(BfrBuffer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BfrBuffer::BfrBuffer(void)
: DataBuffer(CN_BFRBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BFRBUFFER,_FOOTER_);

  m_nKeys = 0;

//#ifdef WITH_CHECK_INITIALISED

  for (i8 iWord = 0; iWord < C_MAX_WORDS; iWord++)
    m_uBitMap[iWord] = 0;

//#endif
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BFRBUFFER,_FOOTER_);


  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if (pLogicUnit)
    pLogicUnit->m_xpStats->m_nNewBfrBuffer++;

  _checkInitialised_(CN_BFRBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BfrBuffer::~BfrBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BFRBUFFER,_FOOTER_);

  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if(pLogicUnit)
    pLogicUnit->m_xpStats->m_nDelCsvBuffer++;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void BfrBuffer::vPostRecv(void* aBfrBuffer)
{
  _static_
  
  BfrBuffer* pBfrBuffer = _pBfrBuffer(aBfrBuffer);
  UNUSED(pBfrBuffer);
}

/*----------------------------------------------------------------------------*/
/* repair bfrBuffer                                                           */
/*----------------------------------------------------------------------------*/
void BfrBuffer::vRepair(Object* pObject)
{
  _static_
  BfrBuffer* pBfrBuffer = _pBfrBuffer(pObject);

  AM::vDeleteOperand(pBfrBuffer,nullptr);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is buffer valid                                                            */
/*----------------------------------------------------------------------------*/
bool BfrBuffer::bIsKeySet(u8 uKey) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BFRBUFFER, _FOOTER_);

  UNUSED(uKey);
  return true;
}

/*----------------------------------------------------------------------------*/
/* get no of keys                                                             */
/*----------------------------------------------------------------------------*/
i8 BfrBuffer::nKeys(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BFRBUFFER, _FOOTER_);

  return m_nKeys;
}

/*----------------------------------------------------------------------------*/
/* is buffer valid                                                            */
/*----------------------------------------------------------------------------*/
bool BfrBuffer::bSetKey(u8 uKey) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BFRBUFFER, _FOOTER_);

  UNUSED(uKey);
  return true;
}

/*----------------------------------------------------------------------------*/
/* is buffer valid                                                            */
/*----------------------------------------------------------------------------*/
bool BfrBuffer::bValidBfrBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BFRBUFFER,_FOOTER_);

  if (! bIsMagicValid()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }

  if (CN_BFRBUFFER != eGetClassNo()) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }

  if (! aFooter()->bValidFooter()) {
    vTrace("Not valid footer\n"_Stz);
    return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
