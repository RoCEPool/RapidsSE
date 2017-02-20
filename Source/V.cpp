/*============================================================================*/
/* V - value helper                                                           */
/*============================================================================*/

#include "Pool.h"
  
/*----------------------------------------------------------------------------*/
/* compare datum (return -ve, 0, +ve)                                         */
/*----------------------------------------------------------------------------*/
i8 V::iCompareDatum(eCN eClass, void* pArg1, void* pArg2)
{
  switch (eClass) {
      
    case CN_INTVAL: return iCompareIntDatum(pArg1,pArg2);
      
    case CN_STRVAL: return iCompareStrDatum(pArg1,pArg2);
      
    default:
      vFatal("V::iCompareDatum: Invalid compare: eClass %lld\n",
             eClass);
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* compare two int datums                                                     */
/*----------------------------------------------------------------------------*/
i8 V::iCompareIntDatum(void* pArg1, void* pArg2)
{
  i8 iCmp = *(i8*)pArg1 - *(i8*)pArg2;
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* compare two str datums                                                     */
/*----------------------------------------------------------------------------*/
i8 V::iCompareStrDatum(void* pArg1, void* pArg2)
{
  i8 iCmp = String::iCompare((char*)pArg1,(char*)pArg2);
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 V::nCopyDatum(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {
      
    case CN_INTVAL: return nCopyIntDatum(pDst,pSrc);
      
    case CN_STRVAL: return nCopyStrDatum(pDst,pSrc);
      
    default:
      vFatal("V::vCopyDatum: Invalid copy: eClass %lld\n",
             eClass);
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* copy datum                                                                 */
/*----------------------------------------------------------------------------*/
void V::vCopyDatum(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {

    case CN_INTVAL: vCopyIntDatum(pDst,pSrc); return;
      
    case CN_STRVAL: vCopyStrDatum(pDst,pSrc); return;
      
    default:
      vFatal("V::vCopyDatum: Invalid copy: eClass %lld\n",
             eClass);
  }
}

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
i8 V::nCopyIntDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
void V::vCopyIntDatum(void* pDst, void* pSrc)
{
  *(i8*)pDst = *(i8*)pSrc;
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
i8 V::nCopyStrDatum(void* pDst, void* pSrc)
{
  String::vCopy((char*)pDst,(char*)pSrc);                //! could return length
  
  return String::iLength((char*)pSrc);
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
void V::vCopyStrDatum(void* pDst, void* pSrc)
{
  String::vCopy((char*)pDst,(char*)pSrc);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
