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
/* copy bol datum                                                            */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyBolDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy bol datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyBolDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy dte datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyDteDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy dte datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyDteDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy flt datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyFltDatum(void* pDst, void* pSrc)
{
  *(d8*)pDst = *(d8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy flt datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyFltDatum(void* pDst, void* pSrc)
{
  *(d8*)pDst = *(d8*)pSrc;
  
  return sizeof(d8);
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
/* copy ivt datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyIvlDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy ivl datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyIvlDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyStrDatum(void* pDst, void* pSrc)
{
  i8 nBytes = StrLib::iSize((char*)pSrc);
  
  StrLib::vCopyLen((char*)pDst,(char*)pSrc, nBytes);                //! could return length
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyStrDatum(void* pDst, void* pSrc)
{
  StrLib::vCopy((char*)pDst,(char*)pSrc);
}

/*----------------------------------------------------------------------------*/
/* copy tme datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyTmeDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy tme datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyTmeDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy tsp datum                                                             */
/*----------------------------------------------------------------------------*/
void ValLib::vCopyTspDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy tsp datum                                                             */
/*----------------------------------------------------------------------------*/
i8 ValLib::nCopyTspDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
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
