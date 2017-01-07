/*============================================================================*/
/* Task_Address.cpp - task address group                                      */
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// address: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Address(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("eql",             &vAddressEqual,     false);
  vAddPrimitive("@",               &vAddressFetch,     false);
  vAddPrimitive("!",               &vAddressStore,     false);
}

/*----------------------------------------------------------------------------*/
/* eql -- test top two address on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vAddressEqual(NNP_Task nnpTask)
{
  ENTRY("vAddressEql");
  
  ValWsp xAddressA;
  ValWsp xAddressB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xAddressA,xAddressB) )
    THROW_INVALID_DSTACK("eql");
  
  i8 iA = xAddressA.iAsInt();
  i8 iB = xAddressB.iAsInt();
  
  i8 r = iA == iB;
  
  nnpTask->m_xDataStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vAddressFetch(NNP_Task nnpTask)
{
  ENTRY("vAddressFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("@");
  
  ValWsp xValFetched;
  
  eCN eClass = pObject->eClass();
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  switch (eClass) {
    case CN_INTVAL:
      xValFetched = pValue->pAsIntVal()->iInt();
      break;
      
    case CN_STRVAL:
      xValFetched = pValue->pAsStrVal()->sStr();
      break;
      
    case CN_PTRVAL:
      xValFetched = pValue->pAsPtrVal()->pPtr();
      break;
      
    default:
      THROW("Invalid eClass %lld",eClass);
  }
  
  nnpTask->m_xDataStack.vPush(xValFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vAddressStore(NNP_Task nnpTask)
{
  ENTRY("vAddressStore");
  
  Object* pObject;
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("!");
  
  ValWsp xValStored;
  if ( ! nnpTask->m_xDataStack.bPop(xValStored) )
    THROW_INVALID_DSTACK("!");
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
    case CN_INTVAL:
      *pValue->pAsIntVal() = xValStored.iInt();
      break;
      
    case CN_STRVAL:
      *pValue->pAsStrVal() = xValStored.sStr();
      break;
      
    case CN_PTRVAL:
      *pValue->pAsPtrVal() = xValStored.pPtr();
      break;
      
    default:
      THROW("Invalid eClass %lld",eClass);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
