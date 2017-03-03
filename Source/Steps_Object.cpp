/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("object.class@",     &vObjectClassFetch, false);
  vAddPrimitive("object.class!",     &vObjectClassStore, false);
  vAddPrimitive("object@",           &vObjectFetch,      false);
  vAddPrimitive("object.magic!",    &vObjectMagicMark,  false);
  vAddPrimitive("object!",           &vObjectStore,      false);
  vAddPrimitive("object.wordbody!",  &vObjectWordBodySet,false);
}

/*----------------------------------------------------------------------------*/
/* object.class@ -- fetch class number from object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassFetch(NNP_Task nnpTask)
{
  ENTRY("vObjectClassFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.class@");
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  nnpTask->m_pDataStack->vPush(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.class! -- store class number into object                            */
/*----------------------------------------------------------------------------*/
void Task::vObjectClassStore(NNP_Task nnpTask)
{
  ENTRY("vObjectClassStore");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.object.class!");
  
  eCN eClassNo;
  if ( ! nnpTask->m_pDataStack->bPop(eClassNo) )
    THROW_INVALID_DSTACK("object.object.class!");
  
  pObject->vClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Task::vObjectDump(NNP_Task nnpTask)
{
  ENTRY("vObjectDump");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.dump");
  
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
  ENTRY("vObjectFetch");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_pReturnStack->vPeek(iPick,&xObjectWsp);
  
  nnpTask->m_pDataStack->vPush(xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.magic! -- put magic number into object                             */
/*----------------------------------------------------------------------------*/
void Task::vObjectMagicMark(NNP_Task nnpTask)
{
  ENTRY("vObjectMagicMark");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_RSTACK("object.magic!");
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vObjectStore(NNP_Task nnpTask)
{
  ENTRY("vObjectStore");
  
  i8 iPeek = 0;
  
  ValWsp xObjectWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(xObjectWsp) )
    THROW_INVALID_DSTACK("object!");
  
  nnpTask->m_pReturnStack->vPoke(iPeek,&xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody! -- mark object as a part of a wordbody                    */
/*----------------------------------------------------------------------------*/
void Task::vObjectWordBodySet(NNP_Task nnpTask)
{
  ENTRY("vObjectWordBodySet");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("object.wordbody!");
  
  i8 iIsBody;
  if ( ! nnpTask->m_pDataStack->bPop(iIsBody) )
    THROW_INVALID_DSTACK("object.object.class!");
  
  pObject->vWordBody(iIsBody);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
