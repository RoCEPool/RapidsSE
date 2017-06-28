/*============================================================================*/
/* ValLib.cpp - value helper                                                  */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 ValLib::iSize(Object* pObject)
{
  i8 nBytes = 0;
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
      
    case CN_BINVAL: {
      BinVal* pBinVal = _pBinVal(pObject);
      nBytes = pBinVal->nSize();
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = _pIntVal(pObject);
      nBytes = pIntVal->nSize();
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = _pIpVal(pObject);
      nBytes = pIpVal->nSize();
      break;
    }
      
    case CN_OBJVAL: {
      ObjVal* pObjVal = _pObjVal(pObject);
      nBytes = pObjVal->nSize();
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = _pPtrVal(pObject);
      nBytes = pPtrVal->nSize();
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = _pStrVal(pObject);
      nBytes = pStrVal->nSize();
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = _pValSet(pObject);
      nBytes = pValSet->nSize();
      break;
    }
      
    default:
      THROW("ValLib::iSize -- invalid eClass %lld"_Str,eClass);
      break;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ValLib::vToString(Object* pObject, Str* aString)
{
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
      
    case CN_BINVAL: {
      BinVal* pBinVal = _pBinVal(pObject);
      pBinVal->vToString(aString);
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = _pIntVal(pObject);
      pIntVal->vToString(aString);
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = _pIpVal(pObject);
      pIpVal->vToString(aString);
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = _pPtrVal(pObject);
      pPtrVal->vToString(aString);
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = _pStrVal(pObject);
      pStrVal->vToString(aString);
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = _pValSet(pObject);
      pValSet->vToString(aString);
      break;
    }
    
    case CN_VALWRK: {
      ValWrk* pValWrk = _pValWrk(pObject);
      pValWrk->vToString(aString);
      break;
    }
      
    default:
      THROW("Invalid eClass %lld"_Str,eClass);
      break;
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
