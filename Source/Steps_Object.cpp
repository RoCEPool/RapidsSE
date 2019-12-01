/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "Steps.h"

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
  vAddStep("object.wordbody!"_Stz,  &Step::vObjectWordBodySet);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassFetch(void)
{
  ENTRY(); // ENTRY("vObjectClassFetch"_Stz);
  
  Object* pObject = m_pDataStack->pPopPtr(__func__); eCN eClassNo;
  eClassNo = pObject->eClass();
  
  m_pDataStack->vPushInt(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassStore(void)
{
  ENTRY(); // ENTRY("vObjectClassStore"_Stz);
  
  Object* pObject = m_pDataStack->pPopPtr(__func__); i8 iClassNo = m_pDataStack->iPopInt(__func__);

  pObject->vClass((eCN)iClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Step::vObjectDump(void)
{
  ENTRY(); // ENTRY("vObjectDump"_Stz);
  
  Object* pObject = m_pDataStack->pPopPtr(__func__); eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, Stz::szTab(1));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectFetch(void)
{
  ENTRY(); // ENTRY("vObjectFetch"_Stz);
  
  i8 iPick = 0;
  
  GenVar xObjectVar;
  
  m_pReturnStack->vPeekVal(iPick,xObjectVar);
  
  m_pDataStack->vPushVal(xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Step::vObjectMagicMark(void)
{
  ENTRY(); // ENTRY("vObjectMagicMark"_Stz);
  
  Object* pObject = m_pDataStack->pPopPtr(__func__); pObject->vMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectStore(void)
{
  ENTRY(); // ENTRY("vObjectStore"_Stz);
  
  i8 iPeek = 0;
  
  GenVar xObjectVar;
  
  m_pDataStack->vPopVal(xObjectVar,__func__);
  
  m_pReturnStack->vPokeVal(iPeek,xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Step::vObjectWordBodySet(void)
{
  ENTRY(); // ENTRY("vObjectWordBodySet"_Stz);
  
  Object* pObject = m_pDataStack->pPopPtr(__func__); i8 iIsBody = m_pDataStack->iPopInt(__func__);
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
