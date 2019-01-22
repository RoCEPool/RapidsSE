/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vObjectClassFetch(MP_Task mpTask)
{
  ENTRY("vObjectClassFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.class@"_Stz);
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  mpTask->m_pDataStack->vPushI8(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassStore(MP_Task mpTask)
{
  ENTRY("vObjectClassStore"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.object.class!"_Stz);
  
  eCN eClassNo;
  if ( ! mpTask->m_pDataStack->bPop(eClassNo) )
    THROW_INVALID_DSTACK("object.object.class!"_Stz);
  
  pObject->vSetClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Step::vObjectDump(MP_Task mpTask)
{
  ENTRY("vObjectDump"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.dump"_Stz);
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, Stz::aTab(1));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectFetch(MP_Task mpTask)
{
  ENTRY("vObjectFetch"_Stz);
  
  i8 iPick = 0;
  
  GenVar xObjectVar;
  
  mpTask->m_pReturnStack->vPeekValue(iPick,xObjectVar);
  
  mpTask->m_pDataStack->vPushGenVar(xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Step::vObjectMagicMark(MP_Task mpTask)
{
  ENTRY("vObjectMagicMark"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_RSTACK("object.magic!"_Stz);
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectStore(MP_Task mpTask)
{
  ENTRY("vObjectStore"_Stz);
  
  i8 iPeek = 0;
  
  GenVar xObjectVar;
  
  if ( ! mpTask->m_pDataStack->bPop(xObjectVar) )
    THROW_INVALID_DSTACK("object!"_Stz);
  
  mpTask->m_pReturnStack->vPokeGenVar(iPeek,xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Step::vObjectWordBodySet(MP_Task mpTask)
{
  ENTRY("vObjectWordBodySet"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.wordbody!"_Stz);
  
  i8 iIsBody;
  if ( ! mpTask->m_pDataStack->bPop(iIsBody) )
    THROW_INVALID_DSTACK("object.object.class!"_Stz);
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
