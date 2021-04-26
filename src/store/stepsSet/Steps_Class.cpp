/*============================================================================*/
/* Steps_Class.cpp - class steps group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

extern SizeHeap gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Class(void)
{
  vAddStep("class::name"_Stz,          &Step::vClassName);
  vAddStep("class::postRecvWord!"_Stz, &Step::vClassPostRecvWordStore);
  vAddStep("class::preSendWord!"_Stz,  &Step::vClassPreSendWordStore);
}

/*----------------------------------------------------------------------------*/
/* class::name -- translate class no to name                                  */
/*----------------------------------------------------------------------------*/
void Step::vClassName(void)
{
  ENTRY();
  
  i8 iClassNo = m_pDataStack->iPopInt(__func__);
  
  s32 sClassName;
  
  sClassName = aCN[iClassNo];
  
  m_pDataStack->vPushStr(sClassName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* class::preSendWord! -- store pre-send word for class                       */
/*----------------------------------------------------------------------------*/
void Step::vClassPreSendWordStore(void)
{
  ENTRY();
  
  eCN eClassNo = _eClassNo(m_pDataStack->iPopInt(__func__));
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
    
  gxaSizeHeap[eClassNo].m_pPreSendWord = pWord;

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* class::postRecvWord! -- store post-recv word for class                     */
/*----------------------------------------------------------------------------*/
void Step::vClassPostRecvWordStore(void)
{
  ENTRY();
  
  eCN eClassNo = _eClassNo(m_pDataStack->iPopInt(__func__));
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
    
  gxaSizeHeap[eClassNo].m_pPostRecvWord = pWord;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
