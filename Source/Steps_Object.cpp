/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// object: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Object(void)
{
  vAddPrimitive("object.class@"_Str,     &vObjectClassFetch, false);
  vAddPrimitive("object.class!"_Str,     &vObjectClassStore, false);
  vAddPrimitive("object@"_Str,           &vObjectFetch,      false);
  vAddPrimitive("object.magic!"_Str,    &vObjectMagicMark,  false);
  vAddPrimitive("object!"_Str,           &vObjectStore,      false);
  vAddPrimitive("object.wordbody!"_Str,  &vObjectWordBodySet,false);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassFetch(NNP_Task nnpTask)
{
  ENTRY("vObjectClassFetch"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.class@"_Str);
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  nnpTask->m_pDataStack->vPushInt(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassStore(NNP_Task nnpTask)
{
  ENTRY("vObjectClassStore"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.object.class!"_Str);
  
  eCN eClassNo;
  if ( ! nnpTask->m_pDataStack->bPop(eClassNo) )
    THROW_INVALID_DSTACK("object.object.class!"_Str);
  
  pObject->vClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Task::vObjectDump(NNP_Task nnpTask)
{
  ENTRY("vObjectDump"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.dump"_Str);
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vObjectFetch(NNP_Task nnpTask)
{
  ENTRY("vObjectFetch"_Str);
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_pReturnStack->vPeek(iPick,&xObjectWsp);
  
  nnpTask->m_pDataStack->vPushValue(xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Task::vObjectMagicMark(NNP_Task nnpTask)
{
  ENTRY("vObjectMagicMark"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_RSTACK("object.magic!"_Str);
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vObjectStore(NNP_Task nnpTask)
{
  ENTRY("vObjectStore"_Str);
  
  i8 iPeek = 0;
  
  ValWsp xObjectWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(xObjectWsp) )
    THROW_INVALID_DSTACK("object!"_Str);
  
  nnpTask->m_pReturnStack->vPoke(iPeek,&xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Task::vObjectWordBodySet(NNP_Task nnpTask)
{
  ENTRY("vObjectWordBodySet"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.wordbody!"_Str);
  
  i8 iIsBody;
  if ( ! nnpTask->m_pDataStack->bPop(iIsBody) )
    THROW_INVALID_DSTACK("object.object.class!"_Str);
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
