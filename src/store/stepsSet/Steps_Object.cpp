/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"

extern SizeHeap gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Object(void)
{
  vAddStep("object.class@"_Stz,       &Step::vObjectClassFetch);
  vAddStep("object.class!"_Stz,       &Step::vObjectClassStore);
  vAddStep("object@"_Stz,             &Step::vObjectFetch);
  vAddStep("object.magic!"_Stz,       &Step::vObjectMagicMark);
  vAddStep("object.postRecvWord@"_Stz,&Step::vObjectPostRecvWordFetch);
  vAddStep("object.preSendWord@"_Stz, &Step::vObjectPreSendWordFetch);
  vAddStep("object!"_Stz,             &Step::vObjectStore);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassFetch(void)
{
  ENTRY();
  
  Object* pObject = m_oDataStack.pPopPtr(__func__);
  
  eCN eClassNo = pObject->eGetClassNo();
  
  m_oDataStack.vPushInt(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassStore(void)
{
  ENTRY();
  
  Object* pObject = m_oDataStack.pPopPtr(__func__);
  
  i8 iClassNo = m_oDataStack.iPopInt(__func__);

  pObject->vSetClassNo((eCN)iClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectFetch(void)
{
  ENTRY();
  
  i8 iPick = 0;
  
  SenCell xObjectVar;
  
  m_oReturnStack.vPeekTypeValueArea(iPick, xObjectVar);
  
  m_oDataStack.vPushSenCell(xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                              */
/*----------------------------------------------------------------------------*/
void Step::vObjectMagicMark(void)
{
  ENTRY();
  
  Object* pObject = m_oDataStack.pPopPtr(__func__);
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Object.postRecvWord@ -- fetch received word for class of object            */
/*----------------------------------------------------------------------------*/
void Step::vObjectPostRecvWordFetch(void)
{
  ENTRY();
  
  Object* pObject = m_oDataStack.pPopPtr(__func__);
  
  eCN eClassNo = pObject->eGetClassNo();
  
  Word* pWord = gxaSizeHeap[eClassNo].m_pPostRecvWord;
  
  m_oDataStack.vPushPtr(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.preSendWord@ -- fetch send word for class of object                 */
/*----------------------------------------------------------------------------*/
void Step::vObjectPreSendWordFetch(void)
{
  ENTRY();
  
  Object* pObject = m_oDataStack.pPopPtr(__func__);
  
  eCN eClassNo = pObject->eGetClassNo();
  
  Word* pWord = gxaSizeHeap[eClassNo].m_pPreSendWord;
  
  m_oDataStack.vPushPtr(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectStore(void)
{
  ENTRY();
  
  i8 iPeek = 0;
  
  SenCell xObjectVar;
  
  m_oDataStack.vPopSenCell(xObjectVar, __func__);
  
  m_oReturnStack.vPokeItem(iPeek, xObjectVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
