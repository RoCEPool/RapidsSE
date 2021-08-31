/*============================================================================*/
/* Steps_NamedBuffer.cpp - NamedBuffer step group                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "NamedBuffer.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_NamedBuffer(void)
{
  vAddStep("namedBuffer.name@"_Stz, &Step::vNamedBufferNameFetch);
  vAddStep("namedBuffer.name!"_Stz, &Step::vNamedBufferNameStore);

  vAddStep("NamedBuffer[]"_Stz,            &Step::vNamedBufferGet);
  vAddStep("NamedBuffer[]\\clear"_Stz,     &Step::vNamedBufferGetAndClear);
  vAddStep("NamedBuffer[].delete"_Stz,     &Step::vNamedBufferDelete);
  vAddStep("NamedBuffer[].isNotNull"_Stz,  &Step::vNamedBufferIsNotNull);
  vAddStep("NamedBuffer[].isNull"_Stz,     &Step::vNamedBufferIsNull);
  vAddStep("NamedBuffer[].morphClass"_Stz, &Step::vNamedBufferMorphClass);
  vAddStep("NamedBuffer[].pipeIn"_Stz,     &Step::vNamedBufferPipeIn);
  vAddStep("NamedBuffer[].set"_Stz,        &Step::vNamedBufferSet);
  
}

/*----------------------------------------------------------------------------*/
/* namedBuffer.name@ -- fetch name of NamedBuffer                             */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferNameFetch(void)
{
  ENTRY();
  
  NamedBuffer* pNamedBuffer = _pNamedBuffer(m_pDataStack->pPopPtr(__func__));
  
  Stz* szBufferNm = pNamedBuffer->szBufferNm();
  
  m_pDataStack->vPushStz(szBufferNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* namedBuffer.name! -- store name of NamedBuffer                             */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferNameStore(void)
{
  ENTRY();
  
  NamedBuffer* pNamedBuffer = _pNamedBuffer(m_pDataStack->pPopPtr(__func__));
  
  s32 sBufferNm;
  m_pDataStack->vPopStr(sBufferNm,__func__);

  pNamedBuffer->vBufferNm(sBufferNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].delete -- operand delete                                     */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferDelete(void)
{
  ENTRY();

  NamedBuffer* pNamedBuffer = _pNamedBuffer(*m_p_TaskLogicUnit->m_xpNamedBuffer);
  delete pNamedBuffer;

  m_p_TaskLogicUnit->m_xpNamedBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].get -- fetch operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpNamedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].getAndClear -- fetch operand and set to nullptr              */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpNamedBuffer);

  m_p_TaskLogicUnit->m_xpNamedBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].isNotNull -- is operand not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpNamedBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].isNull -- is operand null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpNamedBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].morphClass -- change the class number of operand             */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferMorphClass(void)
{
  ENTRY();

  i8 iClassNo = m_pDataStack->iPopInt(__func__);

  NamedBuffer* pNamedBuffer = *m_p_TaskLogicUnit->m_xpNamedBuffer;

  pNamedBuffer->vSetClassNo((eCN)iClassNo);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].pipeIn -- move pipein operand into operandA                  */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferPipeIn(void)
{
  ENTRY();

  NamedBuffer* pNamedBuffer = _pNamedBuffer(*m_p_TaskLogicUnit->m_xpPipein);

  m_p_TaskLogicUnit->m_xpPipein = nullptr;

  m_p_TaskLogicUnit->m_xpNamedBuffer = pNamedBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* NamedBuffer[].set -- set operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vNamedBufferSet(void)
{
  ENTRY();

  NamedBuffer* pNamedBuffer = _pNamedBuffer(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pNamedBuffer || m_p_TaskLogicUnit->m_xpNamedBuffer.bIsNull());

  m_p_TaskLogicUnit->m_xpNamedBuffer = _pNamedBuffer(pNamedBuffer);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
