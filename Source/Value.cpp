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
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 Value::iSize(void)
{
  i8 nBytes = 0;
  
  eCN eClass = this->eClass();
  
  switch (eClass) {
    
    case CN_BINVAL: {
      BinVal* pBinVal = _pBinVal(this);
      nBytes = pBinVal->iSize();
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = _pIntVal(this);
      nBytes = pIntVal->iSize();
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = _pIpVal(this);
      nBytes = pIpVal->iSize();
      break;
    }
      
    case CN_OBJVAL: {
      ObjVal* pObjVal = _pObjVal(this);
      nBytes = pObjVal->iSize();
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = _pPtrVal(this);
      nBytes = pPtrVal->iSize();
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = _pStrVal(this);
      nBytes = pStrVal->iSize();
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = _pValSet(this);
      nBytes = pValSet->iSize();
      break;
    }
      
    default:
      THROW("Invalid eClass %lld",eClass);
      break;
  }
  
  return nBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
