/*============================================================================*/
/* ValLib.cpp - value helper                                                  */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyDatum(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {
      
    case CN_INTVAL: return Int::nCopy(pDst,pSrc);
      
    case CN_STRVAL: return Vch::nCopy(pDst,pSrc);
      
    default:
      vFatal("ValLib::vCopyDatum: Invalid copy: eClass %lld\n"_Str,
             eClass);
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* copy datum                                                                 */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyDatum(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {

    case CN_INTVAL: Int::vCopy(pDst,pSrc); return;
      
    case CN_STRVAL: Vch::vCopy(pDst,pSrc); return;
      
    default:
      vFatal("ValLib::vCopyDatum: Invalid copy: eClass %lld\n"_Str,
             eClass);
  }
}

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
