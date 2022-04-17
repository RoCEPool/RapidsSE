/*============================================================================*/
/* PipeIn.cpp - Pipe In                                                       */
/*============================================================================*/

#include "STORE.h"
#include "PipeIn.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PipeIn,CN_PIPEIN)
__aNewFunction(PipeIn,CN_PIPEIN)
__vDeleteFunction(PipeIn,CN_PIPEIN)
__aFooterFunction(PipeIn)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PipeIn::PipeIn(void)
: Object(CN_PIPEIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEIN,_FOOTER_);
  
  m_pQueueLeft  = new OperandQueue;
  m_pQueueRight = new OperandQueue;
  m_pQueueOoO   = new OperandQueue;

  _checkInitialised_(CN_PIPEIN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPEIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PipeIn::~PipeIn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEIN,_FOOTER_);
  
  delete m_pQueueLeft;
  delete m_pQueueRight;
  delete m_pQueueOoO;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get left input queue                                                       */
/*----------------------------------------------------------------------------*/
OperandQueue* PipeIn::pQueueLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEIN,_FOOTER_);

  return m_pQueueLeft;
}

/*----------------------------------------------------------------------------*/
/* get right input queue                                                      */
/*----------------------------------------------------------------------------*/
OperandQueue* PipeIn::pQueueRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEIN,_FOOTER_);

  return m_pQueueRight;
}

/*----------------------------------------------------------------------------*/
/* get out-of-order input queue                                               */
/*----------------------------------------------------------------------------*/
OperandQueue* PipeIn::pQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPEIN,_FOOTER_);

  return m_pQueueOoO;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
