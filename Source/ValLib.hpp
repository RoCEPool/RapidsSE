/*============================================================================*/
/* ValLib.hpp - value helper                                                  */
/*============================================================================*/
#pragma once

namespace Values
{
  class ValLib
  {
  private:

  public:
    static i8   iCompareDatum(eCN eClass, void* pArg1, void* pArg2);
    static i8   iCompareIntDatum(void* pArg1, void* pArg2);
    static i8   iCompareStrDatum(void* pArg1, void* pArg2);
  
    static i8   nCopyDatum(eCN eClass, void* pDst, void* pSrc);
    static void vCopyDatum(eCN eClass, void* pDst, void* pSrc);
  
    static i8   nCopyDteDatum(void* pDst, void* pSrc);
    static void vCopyDteDatum(void* pDst, void* pSrc);
  
    static i8   nCopyFltDatum(void* pDst, void* pSrc);
    static void vCopyFltDatum(void* pDst, void* pSrc);
  
    static i8   nCopyIntDatum(void* pDst, void* pSrc);
    static void vCopyIntDatum(void* pDst, void* pSrc);

    static i8   nCopyIvlDatum(void* pDst, void* pSrc);
    static void vCopyIvlDatum(void* pDst, void* pSrc);
  
    static i8   nCopyStrDatum(void* pDst, void* pSrc);
    static void vCopyStrDatum(void* pDst, void* pSrc);
  
    static i8   nCopyTmeDatum(void* pDst, void* pSrc);
    static void vCopyTmeDatum(void* pDst, void* pSrc);
  
    static i8   nCopyTspDatum(void* pDst, void* pSrc);
    static void vCopyTspDatum(void* pDst, void* pSrc);
  
    static i8   iSize(Object* pObject);
  
    static void vToString(Object* pObject, Str* aCharArray);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
