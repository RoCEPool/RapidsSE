/*============================================================================*/
/* V - value helper                                                           */
/*============================================================================*/

#include "Pool.h"
  
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
