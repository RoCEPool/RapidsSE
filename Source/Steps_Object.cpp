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
  ENTRY("vObjectClassFetch"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  m_pDataStack->vPushI8(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Step::vObjectClassStore(void)
{
  ENTRY("vObjectClassStore"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
  eCN eClassNo;
  if ( ! m_pDataStack->bPopCN(eClassNo) )
    THROW_DSTACK();
  
  pObject->vSetClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Step::vObjectDump(void)
{
  ENTRY("vObjectDump"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, Stz::szTab(1));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectFetch(void)
{
  ENTRY("vObjectFetch"_Stz);
  
  i8 iPick = 0;
  
  GenVar xObjectVar;
  
  m_pReturnStack->vPeekValue(iPick,xObjectVar);
  
  m_pDataStack->vPushGenVar(xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Step::vObjectMagicMark(void)
{
  ENTRY("vObjectMagicMark"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_RSTACK();
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vObjectStore(void)
{
  ENTRY("vObjectStore"_Stz);
  
  i8 iPeek = 0;
  
  GenVar xObjectVar;
  
  if ( ! m_pDataStack->bPop(xObjectVar) )
    THROW_DSTACK();
  
  m_pReturnStack->vPokeGenVar(iPeek,xObjectVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Step::vObjectWordBodySet(void)
{
  ENTRY("vObjectWordBodySet"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPop(pObject) )
    THROW_DSTACK();
  
  i8 iIsBody;
  if ( ! m_pDataStack->bPop(iIsBody) )
    THROW_DSTACK();
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
