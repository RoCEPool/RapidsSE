/*============================================================================*/
/* WriteBehind.cpp -- Working WriteBehind Buffer                              */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "WriteBehind.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,WriteBehind,CN_WRITEBEHIND)
__aNewFunction(WriteBehind,CN_WRITEBEHIND)
__vDeleteFunction(WriteBehind,CN_WRITEBEHIND)
__aFooterFunction(WriteBehind)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WriteBehind::WriteBehind(void)
: Object(CN_WRITEBEHIND)
, m_okLock(this,C_DONT_LOG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pStreamDef = AM::pStreamDefFind(C_TDEF_EVENTS);
  
  m_pRowBuffer = nullptr;
    
  _checkInitialised_(CN_WRITEBEHIND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WriteBehind::~WriteBehind(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a row                                                                */
/*----------------------------------------------------------------------------*/
Row* WriteBehind::pAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  if (! m_pRowBuffer) {
    m_pRowBuffer = new RowBuffer(CN_ROWBUFFER,m_pStreamDef);
  }
  
  Row* pRow = m_pRowBuffer->pAllotRow();
  
  if (! pRow) {
    delete m_pRowBuffer;
    m_pRowBuffer = new RowBuffer(CN_ROWBUFFER,m_pStreamDef);
    Row* pRow = m_pRowBuffer->pAllotRow();
    RASSERT(pRow);
  }
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* deposit var-arg list of column values into __events row                    */
/*----------------------------------------------------------------------------*/
void WriteBehind::vDepositEvent(i8 iVargsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  ASSERT(C_VARGS_BEGIN == iVArgsBegin);
  
  m_okLock.vGrabWrite();
  
  Row* pRow = pAllotRow();

  va_list xVarArgs;
  va_start(xVarArgs,iVargsBegin);

  pRow->vDeposit(xVarArgs);
  
  va_end(xVarArgs);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get current row buffer                                                     */
/*----------------------------------------------------------------------------*/
RowBuffer* WriteBehind::pRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set current row buffer                                                     */
/*----------------------------------------------------------------------------*/
void WriteBehind::vRowBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pRowBuffer = pRowBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
