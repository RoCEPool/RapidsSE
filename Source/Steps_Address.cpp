/*============================================================================*/
/* Steps_Address.cpp - address step group                                     */
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
  
  vAddPrimitive("eql"_Str,             &vAddressEqual,     false);
  vAddPrimitive("@"_Str,               &vAddressFetch,     false);
  vAddPrimitive("!"_Str,               &vAddressStore,     false);
}

/*----------------------------------------------------------------------------*/
/* eql -- test top two address on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vAddressEqual(NNP_Task nnpTask)
{
  ENTRY("vAddressEql");
  
  ValWsp xAddressA;
  ValWsp xAddressB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xAddressA,xAddressB) )
    THROW_INVALID_DSTACK("eql");
  
  i8 iA = xAddressA.iAsInt();
  i8 iB = xAddressB.iAsInt();
  
  i8 r = iA == iB;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vAddressFetch(NNP_Task nnpTask)
{
  ENTRY("vAddressFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("@");
  
  ValWsp xValFetched;
  
  eCN eClass = pObject->eClass();
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  switch (eClass) {
    case CN_INTVAL:
      xValFetched = _pIntVal(pValue)->iInt();
      break;
      
    case CN_STRVAL:
      xValFetched = _pStrVal(pValue);
      break;
      
    case CN_PTRVAL:
      xValFetched = _pPtrVal(pValue)->pPtr();
      break;
      
    case CN_IPVAL:
      xValFetched = (Object*)_pIpVal(pValue)->pIpPtr();
      break;
  
    default:
      THROW("Invalid eClass %lld"_Str,eClass);
  }
  
  nnpTask->m_pDataStack->vPush(xValFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vAddressStore(NNP_Task nnpTask)
{
  ENTRY("vAddressStore");
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("!");
  
  ValWsp xValStored;
  if ( ! nnpTask->m_pDataStack->bPop(xValStored) )
    THROW_INVALID_DSTACK("!");
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
    case CN_INTVAL:
      *_pIntVal(pValue) = _pIntVal(&xValStored)->iInt();
      break;
      
    case CN_STRVAR:
    case CN_STRVAL:
      _pStrVal(pValue)->vAssign( _pStrVal(&xValStored)->aStr() );
      break;
      
    case CN_PTRVAL:
      *_pPtrVal(pValue) = _pPtrVal(&xValStored)->pPtr();
      break;
      
    case CN_IPVAL:
      *_pIpVal(pValue) = _pIpVal(&xValStored)->pIpPtr();
      break;
  
    default:
      THROW("Invalid eClass %lld"_Str,eClass);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
