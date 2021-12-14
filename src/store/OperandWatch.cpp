/*============================================================================*/
/* OperandWatch.cpp - Operand Watch                                           */
/*============================================================================*/

#include "STORE.h"
#include "OperandWatch.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,OperandWatch,CN_OPERANDWATCH)
__aNewFunction(OperandWatch,CN_OPERANDWATCH)
__vDeleteFunction(OperandWatch,CN_OPERANDWATCH)
__aFooterFunction(OperandWatch)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OperandWatch::OperandWatch(void)
: Object(CN_OPERANDWATCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  vReset();
  
  _checkInitialised_(CN_OPERANDWATCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OperandWatch::~OperandWatch(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get an nCnt                                                                */
/*----------------------------------------------------------------------------*/
i8 OperandWatch::nCnt(i8 i_Operand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  return m_xaOperand[i_Operand].m_nCnt;
}

/*----------------------------------------------------------------------------*/
/* get an iMax                                                                */
/*----------------------------------------------------------------------------*/
i8 OperandWatch::iFirst(i8 i_Operand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  return m_xaOperand[i_Operand].m_iFirst;
}

/*----------------------------------------------------------------------------*/
/* get an iMax                                                                */
/*----------------------------------------------------------------------------*/
i8 OperandWatch::iFirstUs(i8 i_Operand, i8 iParticipantTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  Timer xTimer;
  
  xTimer.vSet(iParticipantTsc, m_xaOperand[i_Operand].m_iFirst);
  
  return xTimer.iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* get an iMin                                                                */
/*----------------------------------------------------------------------------*/
i8 OperandWatch::iLast(i8 i_Operand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  return m_xaOperand[i_Operand].m_iLast;
}

/*----------------------------------------------------------------------------*/
/* get an iMin                                                                */
/*----------------------------------------------------------------------------*/
i8 OperandWatch::iLastUs(i8 i_Operand, i8 iParticipantTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  Timer xTimer;
  
  xTimer.vSet(iParticipantTsc, m_xaOperand[i_Operand].m_iLast);
  
  return xTimer.iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* reset all items                                                            */
/*----------------------------------------------------------------------------*/
void OperandWatch::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  for (i8 i_Operand = 0; i_Operand < ZK_ENUMS; i_Operand++) {
    
    m_xaOperand[i_Operand].m_nCnt   = 0;
    m_xaOperand[i_Operand].m_iFirst = 0;
    m_xaOperand[i_Operand].m_iLast  = 0;

  }
}

/*----------------------------------------------------------------------------*/
/* saw an operand at some time point                                          */
/*----------------------------------------------------------------------------*/
void OperandWatch::vSaw(i8 i_Operand, i8 iAt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDWATCH,_FOOTER_);
  
  m_xaOperand[i_Operand].m_nCnt ++;
  
  if (! m_xaOperand[i_Operand].m_iFirst)
    m_xaOperand[i_Operand].m_iFirst = iAt;
  

  m_xaOperand[i_Operand].m_iLast = iAt;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
