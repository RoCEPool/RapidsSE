/*============================================================================*/
/* OnStack.cpp - root class for objects on thread stack                       */
/*============================================================================*/

#include "StackValues.h"

#include "TaskVal.hpp"
#include "WrdVal.hpp"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* OnStack::operator new(size_t iSize)
{
  vFatal("fatal: OnStack class cannot be new'd\n");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OnStack::OnStack(cn eClass)
: cn(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void OnStack::operator =(i8 iInt)
{
  vClass(CN_INTVAL);
  
  IntVal* pIntVal = pAsIntVal();
  
  *pIntVal = iInt;
  
}

/*----------------------------------------------------------------------------*/
/* overlap this as a intval                                                   */
/*----------------------------------------------------------------------------*/
IntVal* OnStack::pAsIntVal(void)
{
  IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
  
  return pIntVal;
}

/*----------------------------------------------------------------------------*/
/* overlap this as a ptrval                                                   */
/*----------------------------------------------------------------------------*/
PtrVal* OnStack::pAsPtrVal(void)
{
  PtrVal* pPtrVal = reinterpret_cast<PtrVal*>(this);
  
  return pPtrVal;
}

/*----------------------------------------------------------------------------*/
/* overlap this as a strval                                                   */
/*----------------------------------------------------------------------------*/
StrVal* OnStack::pAsStrVal(void)
{
  StrVal* pStrVal = reinterpret_cast<StrVal*>(this);
  
  return pStrVal;
}

/*----------------------------------------------------------------------------*/
/* overlap this as a valset                                                   */
/*----------------------------------------------------------------------------*/
ValSet* OnStack::pAsValSet(void)
{
  ValSet* pValSet = reinterpret_cast<ValSet*>(this);
  
  return pValSet;
}

/*----------------------------------------------------------------------------*/
/* overlap this as a wrdval                                                   */
/*----------------------------------------------------------------------------*/
WrdVal* OnStack::pAsWrdVal(void)
{
  WrdVal* pWrdVal = reinterpret_cast<WrdVal*>(this);
  
  return pWrdVal;
}

/*----------------------------------------------------------------------------*/
/* get class name                                                             */
/*----------------------------------------------------------------------------*/
char* OnStack::aClass(void)
{
  return (char*)pCN[eClass()];
}

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 OnStack::iAsInt(void)
{
  IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
  
  i8 i = pIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 OnStack::iInt(void)
{
  ASSERT(CN_INTVAL == eClass());
  
  IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
  
  i8 i = pIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get pointer                                                                */
/*----------------------------------------------------------------------------*/
void* OnStack::pPtr(void)
{
  ASSERT(CN_PTRVAL == eClass());
  
  PtrVal* pPtrVal = reinterpret_cast<PtrVal*>(this);
  
  void* p = pPtrVal->pPtr();
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get string                                                                 */
/*----------------------------------------------------------------------------*/
char* OnStack::sStr(void)
{
  ASSERT(CN_STRVAL == eClass());
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(this);
  
  char* s = (char*)pStrVal->sStr();
  
  return s;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
NNP_Task OnStack::nnpTask(void)
{
  ASSERT(CN_TASKVAL == eClass());
  
  TaskVal* pPtrTask = reinterpret_cast<TaskVal*>(this);
  
  NNP_Task nnp = pPtrTask->pTask();
  
  return nnp;
}

/*----------------------------------------------------------------------------*/
/* get word                                                                   */
/*----------------------------------------------------------------------------*/
Word* OnStack::pWord(void)
{
  ASSERT(CN_WRDVAL == eClass());
  
  WrdVal* pPtrWord = reinterpret_cast<WrdVal*>(this);
  
  Word* p = pPtrWord->pWord();
  
  return p;
}


/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 OnStack::iSize(void)
{
  i8 iSize = 0;
  
  eCN eClass = this->eClass();
  switch(eClass) {
      
    case CN_INTVAL: {
      IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
      iSize = pIntVal->iSize();
      break;
    }
      
    case CN_IPVAL: {
      IPval* pIPval = reinterpret_cast<IPval*>(this);
      iSize = pIPval->iSize();
      break;
    }
      
    case CN_OBJVAL: {
      ObjVal* pObjVal = reinterpret_cast<ObjVal*>(this);
      iSize = pObjVal->iSize();
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = reinterpret_cast<PtrVal*>(this);
      iSize = pPtrVal->iSize();
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = reinterpret_cast<StrVal*>(this);
      iSize = pStrVal->iSize();
      break;
    }
      
    case CN_TASKVAL: {
      TaskVal* pTaskVal = reinterpret_cast<TaskVal*>(this);
      iSize = pTaskVal->iSize();
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = reinterpret_cast<ValSet*>(this);
      iSize = pValSet->iSize();
      break;
    }
      
   case CN_WRDVAL: {
      WrdVal* pWrdVal = reinterpret_cast<WrdVal*>(this);
      iSize = pWrdVal->iSize();
      break;
    }
      
    default:
      ASSERT(0);
      break;
  }
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void OnStack::vToCommand(char* aWorkspace)
{
  eCN eClass = this->eClass();
  switch(eClass) {
      
    case CN_INTVAL: {
      IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
      pIntVal->vToCommand(aWorkspace);
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = reinterpret_cast<PtrVal*>(this);
      pPtrVal->vToCommand(aWorkspace);
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = reinterpret_cast<StrVal*>(this);
      pStrVal->vToCommand(aWorkspace);
      break;
    }
      
    case CN_TASKVAL: {
      TaskVal* pTaskVal = reinterpret_cast<TaskVal*>(this);
      pTaskVal->vToCommand(aWorkspace);
      break;
    }
      
    case CN_WRDVAL: {
      WrdVal* pWrdVal = reinterpret_cast<WrdVal*>(this);
      pWrdVal->vToCommand(aWorkspace);
      break;
    }
      
    default:
      ASSERT(0);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void OnStack::vToString(char* aWorkspace)
{
  eCN eClass = this->eClass();
  switch(eClass) {
      
    case CN_INTVAL: {
      IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
      pIntVal->vToString(aWorkspace);
      break;
    }
      
    case CN_IPVAL: {
      IPval* pIPval = reinterpret_cast<IPval*>(this);
      pIPval->vToString(aWorkspace);
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = reinterpret_cast<PtrVal*>(this);
      pPtrVal->vToString(aWorkspace);
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = reinterpret_cast<StrVal*>(this);
      pStrVal->vToString(aWorkspace);
      break;
    }
      
    case CN_TASKVAL: {
      TaskVal* pTaskVal = reinterpret_cast<TaskVal*>(this);
      pTaskVal->vToString(aWorkspace);
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = reinterpret_cast<ValSet*>(this);
      pValSet->vToString(aWorkspace);
      break;
    }
      
    case CN_WRDVAL: {
      WrdVal* pWrdVal = reinterpret_cast<WrdVal*>(this);
      pWrdVal->vToString(aWorkspace);
      break;
    }
      
    default:
      ASSERT(0);
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/