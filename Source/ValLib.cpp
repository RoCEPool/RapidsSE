/*============================================================================*/
/* ValLib.cpp - value helper                                                  */
/*============================================================================*/

#include "Pool.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* compare datum (return -ve, 0, +ve)                                         */
/*----------------------------------------------------------------------------*/
i8 ValLib::iCompareDatum(eCN eClass, void* pArg1, void* pArg2)
{
  switch (eClass) {
      
    case CN_INTVAL: return iCompareIntDatum(pArg1,pArg2);
      
    case CN_STRVAL: return iCompareStrDatum(pArg1,pArg2);
      
    default:
      vFatal("ValLib::iCompareDatum: Invalid compare: eClass %lld\n"_Str,
             eClass);
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* compare two int datums                                                     */
/*----------------------------------------------------------------------------*/
i8 ValLib::iCompareIntDatum(void* pArg1, void* pArg2)
{
  i8 iCmp = *(i8*)pArg1 - *(i8*)pArg2;
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* compare two str datums                                                     */
/*----------------------------------------------------------------------------*/
i8 ValLib::iCompareStrDatum(void* pArg1, void* pArg2)
{
  i8 iCmp = StrLib::iCompare((char*)pArg1,(char*)pArg2);
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyDatum(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {
      
    case CN_INTVAL: return nCopyIntDatum(pDst,pSrc);
      
    case CN_STRVAL: return nCopyStrDatum(pDst,pSrc);
      
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

    case CN_INTVAL: vCopyIntDatum(pDst,pSrc); return;
      
    case CN_STRVAL: vCopyStrDatum(pDst,pSrc); return;
      
    default:
      vFatal("ValLib::vCopyDatum: Invalid copy: eClass %lld\n"_Str,
             eClass);
  }
}

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyIntDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyIntDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyStrDatum(void* pDst, void* pSrc)
{
  StrLib::vCopyLen((char*)pDst,(char*)pSrc, 8);                //! could return length
  
  vTrace("%s %s \n"_Str, (char*)pSrc, (char*)pDst);
  //return StrLib::iLength((char*)pSrc);
  return 8;
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyStrDatum(void* pDst, void* pSrc)
{
  StrLib::vCopy((char*)pDst,(char*)pSrc);
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
      
    default:
      THROW("Invalid eClass %lld"_Str,eClass);
      break;
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
