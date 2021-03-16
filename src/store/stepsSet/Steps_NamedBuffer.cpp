/*============================================================================*/
/* Steps_NamedBuffer.cpp - NamedBuffer step group                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "NamedBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_NamedBuffer(void)
{
  vAddStep("namedBuffer.name@"_Stz, &Step::vNamedBufferNameFetch);
  vAddStep("namedBuffer.name!"_Stz, &Step::vNamedBufferNameStore);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
