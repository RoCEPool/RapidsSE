/*============================================================================*/
/* ValLib.cpp - value helper                                                  */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 ValLib::iSize(Value* aValue)
{
  i8 nBytes = 0;
  
  eCN eClass = aValue->eClass();
  
  switch (eClass) {
      
    case CN_BINVAL: {
      BinVal* pBinVal = _pBinVal(aValue);
      nBytes = pBinVal->nSize();
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = _pIntVal(aValue);
      nBytes = pIntVal->nSize();
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = _pIpVal(aValue);
      nBytes = pIpVal->nSize();
      break;
    }
            
    case CN_PTRVAL: {
      PtrVal* pPtrVal = _pPtrVal(aValue);
      nBytes = pPtrVal->nSize();
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = _pStrVal(aValue);
      nBytes = pStrVal->nSize();
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = _pValSet(aValue);
      nBytes = pValSet->nSize();
      break;
    }
      
    default:
      THROW("ValLib::iSize -- invalid eClass %lld"_Stz,eClass);
      break;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ValLib::vToString(Value* aValue, Stz* aString)
{
  eCN eClass = aValue->eClass();
  
  switch (eClass) {
      
    case CN_BINVAL: {
      BinVal* pBinVal = _pBinVal(aValue);
      pBinVal->vToString(aString);
      break;
    }
      
    case CN_INTVAL: {
      IntVal* pIntVal = _pIntVal(aValue);
      pIntVal->vToString(aString);
      break;
    }
      
    case CN_IPVAL: {
      IpVal* pIpVal = _pIpVal(aValue);
      pIpVal->vToString(aString);
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* pPtrVal = _pPtrVal(aValue);
      pPtrVal->vToString(aString);
      break;
    }
      
    case CN_STRVAL: {
      StrVal* pStrVal = _pStrVal(aValue);
      pStrVal->vToString(aString);
      break;
    }
      
    case CN_VALSET: {
      ValSet* pValSet = _pValSet(aValue);
      pValSet->vToString(aString);
      break;
    }
    
    case CN_VALWRK: {
      ValWrk* pValWrk = _pValWrk(aValue);
      pValWrk->vToString(aString);
      break;
    }
      
    default:
      THROW("Invalid eClass %lld"_Stz,eClass);
      break;
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
