/*============================================================================*/
/* Value.cpp --Base Class for Self-Describing Values                          */
/*============================================================================*/

#include "Values.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Value::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Value class cannot be new'd\n");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Value::Value(eCN eClass)
: Object(eClass)
{
  _checkInitialisedSize_(sizeof(Value));
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer into workspace                                      */
/*----------------------------------------------------------------------------*/
void Value::operator =(i8 iInt)
{
  vClass(CN_INTVAL);
  
  IntVal* pIntVal = _pIntVal(this);
  
  *pIntVal = iInt;
  
}

/*----------------------------------------------------------------------------*/
/* get binary object                                                          */
/*----------------------------------------------------------------------------*/
Object* Value::pObject(void)
{
  ASSERT(CN_BINVAL == eClass());
  
  BinVal* pBinVal = reinterpret_cast<BinVal*>(this);
  
  i8 iSize = pBinVal->iSize();
  Object* pObject = Heap::pAllocate(pBinVal->eObjectClass());
  Memory::vCopy(pObject, pBinVal->pData(), iSize);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get integer (ignoring type)                                                */
/*----------------------------------------------------------------------------*/
i8 Value::iAsInt(void)
{
  IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
  
  i8 i = pIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 Value::iInt(void)
{
  ASSERT(CN_INTVAL == eClass());
  
  IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
  
  i8 i = pIntVal->iInt();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get pointer                                                                */
/*----------------------------------------------------------------------------*/
Object* Value::pPtr(void)
{
  ASSERT(CN_PTRVAL == eClass());
  
  PtrVal* pPtrVal = reinterpret_cast<PtrVal*>(this);
  
  Object* p = pPtrVal->pObject();
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get instruction pointer                                                    */
/*----------------------------------------------------------------------------*/
Cell* Value::pIpPtr(void)
{
  ASSERT(CN_IPVAL == eClass());
  
  IpVal* pIpVal = reinterpret_cast<IpVal*>(this);
  
  Cell* p = pIpVal->pIpPtr();
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get string                                                                 */
/*----------------------------------------------------------------------------*/
char* Value::sStr(void)
{
  ASSERT(CN_STRVAL == eClass());
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(this);
  
  char* s = (char*)pStrVal->sStr();
  
  return s;
}

/*----------------------------------------------------------------------------*/
/* get word                                                                   */
/*----------------------------------------------------------------------------*/
Dictionaries::Word* Value::pWord(void)
{
  ASSERT(CN_PTRVAL == eClass());
  
  PtrVal* pPtrWord = reinterpret_cast<PtrVal*>(this);
  
  Dictionaries::Word* p = pPtrWord->pWord();
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 Value::iSize(void)
{
  i8 iSize = 0;
  
  eCN eClass = this->eClass();
  switch(eClass) {
    
    case CN_BINVAL: {
      BinVal* pBinVal = reinterpret_cast<BinVal*>(this);
      iSize = pBinVal->iSize();
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
      iSize = pIntVal->iSize();
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = reinterpret_cast<IpVal*>(this);
      iSize = pIpVal->iSize();
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
      
    case CN_VALSET: {
      ValSet* pValSet = reinterpret_cast<ValSet*>(this);
      iSize = pValSet->iSize();
      break;
    }
      
    default:
      THROW("Invalid eClass %lld",eClass);
      break;
  }
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void Value::vToCommand(char* aWorkspace)
{
  eCN eClass = this->eClass();
  switch(eClass) {
      
    case CN_INTVAL: {
      IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
      pIntVal->vToCommand(aWorkspace);
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = reinterpret_cast<IpVal*>(this);
      pIpVal->vToCommand(aWorkspace);
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
      
    default:
      THROW("Invalid eClass %lld",eClass);
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void Value::vToString(char* aWorkspace)
{
  eCN eClass = this->eClass();
  
  switch(eClass) {
      
    case CN_BINVAL: {
      BinVal* pBinVal = reinterpret_cast<BinVal*>(this);
      pBinVal->vToString(aWorkspace);
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = reinterpret_cast<IntVal*>(this);
      pIntVal->vToString(aWorkspace);
      break;
    }
    
    case CN_IPVAL: {
      IpVal* pIpVal = reinterpret_cast<IpVal*>(this);
      pIpVal->vToString(aWorkspace);
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
      
    case CN_VALSET: {
      ValSet* pValSet = reinterpret_cast<ValSet*>(this);
      pValSet->vToString(aWorkspace);
      break;
    }
      
    default:
      THROW("Invalid eClass %lld",eClass);
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
