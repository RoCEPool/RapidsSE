/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "STORE.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Object(void)
{
  vAddStep("object.class@"_Stz,     &Step::vObjectClassFetch);
  vAddStep("object.class!"_Stz,     &Step::vObjectClassStore);
  vAddStep("object@"_Stz,           &Step::vObjectFetch);
  vAddStep("object.magic!"_Stz,     &Step::vObjectMagicMark);
  vAddStep("object!"_Stz,           &Step::vObjectStore);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassFetch(void)
{
  ENTRY();
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  
  eCN eClassNo = pObject->eClass();
  
  m_pDataStack->vPushInt(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassStore(void)
{
  ENTRY();
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  
  i8 iClassNo = m_pDataStack->iPopInt(__func__);

  pObject->vClass((eCN)iClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectFetch(void)
{
  ENTRY();
  
  i8 iPick = 0;
  
  CellSingle xObjectVar;
  
  m_pReturnStack->vPeekItem(iPick,xObjectVar);
  
  m_pDataStack->vPushItem(xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Step::vObjectMagicMark(void)
{
  ENTRY();
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  
  pObject->vMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectStore(void)
{
  ENTRY();
  
  i8 iPeek = 0;
  
  CellSingle xObjectVar;
  
  m_pDataStack->vPopItem(xObjectVar,__func__);
  
  m_pReturnStack->vPokeItem(iPeek,xObjectVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
