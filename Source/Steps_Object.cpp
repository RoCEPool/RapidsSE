/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// object: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Object(void)
{
  vAddStep("object.class@"_Stz,     &Task::vObjectClassFetch, false);
  vAddStep("object.class!"_Stz,     &Task::vObjectClassStore, false);
  vAddStep("object@"_Stz,           &Task::vObjectFetch,      false);
  vAddStep("object.magic!"_Stz,     &Task::vObjectMagicMark,  false);
  vAddStep("object!"_Stz,           &Task::vObjectStore,      false);
  vAddStep("object.wordbody!"_Stz,  &Task::vObjectWordBodySet,false);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassFetch(MP_Task mpTask)
{
  ENTRY("vObjectClassFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.class@"_Stz);
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  mpTask->m_pDataStack->vPushInt(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassStore(MP_Task mpTask)
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
void Task::vObjectDump(MP_Task mpTask)
{
  ENTRY("vObjectDump"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.dump"_Stz);
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vObjectFetch(MP_Task mpTask)
{
  ENTRY("vObjectFetch"_Stz);
  
  i8 iPick = 0;
  
  ValWrk xObjectWsp;
  
  mpTask->m_pReturnStack->vPeekValWrk(iPick,xObjectWsp);
  
  mpTask->m_pDataStack->vPushValWrk(xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Task::vObjectMagicMark(MP_Task mpTask)
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
void Task::vObjectStore(MP_Task mpTask)
{
  ENTRY("vObjectStore"_Stz);
  
  i8 iPeek = 0;
  
  ValWrk xObjectWsp;
  
  if ( ! mpTask->m_pDataStack->bPop(xObjectWsp) )
    THROW_INVALID_DSTACK("object!"_Stz);
  
  mpTask->m_pReturnStack->vPokeValWrk(iPeek,xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Task::vObjectWordBodySet(MP_Task mpTask)
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
