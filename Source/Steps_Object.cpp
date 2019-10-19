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
void Step::vObjectClassFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectClassFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectClassStore"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  eCN eClassNo;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(eClassNo) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pObject->vSetClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Step::vObjectDump(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectDump"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, Stz::szTab(1));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectFetch"_Stz);
  
  i8 iPick = 0;
  
  GenVar xObjectVar;
  
  mpTask->m_pForth->m_pReturnStack->vPeekValue(iPick,xObjectVar);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Step::vObjectMagicMark(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectMagicMark"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__FILE__,__LINE__,FK_RSTACK,__STEP__);
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectStore"_Stz);
  
  i8 iPeek = 0;
  
  GenVar xObjectVar;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xObjectVar) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  mpTask->m_pForth->m_pReturnStack->vPokeGenVar(iPeek,xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Step::vObjectWordBodySet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vObjectWordBodySet"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  i8 iIsBody;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iIsBody) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
