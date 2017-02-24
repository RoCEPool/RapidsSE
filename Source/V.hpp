/*============================================================================*/
/* V - value helper                                                           */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Values
{
  class V
  {
  private:

  public:
    static i8   iCompareDatum(eCN eClass, void* pArg1, void* pArg2);
    static i8   iCompareIntDatum(void* pArg1, void* pArg2);
    static i8   iCompareStrDatum(void* pArg1, void* pArg2);
  
    static i8   nCopyDatum(eCN eClass, void* pDst, void* pSrc);
    static void vCopyDatum(eCN eClass, void* pDst, void* pSrc);
  
    static i8   nCopyIntDatum(void* pDst, void* pSrc);
    static void vCopyIntDatum(void* pDst, void* pSrc);
  
    static i8   nCopyStrDatum(void* pDst, void* pSrc);
    static void vCopyStrDatum(void* pDst, void* pSrc);
  
    static void vToString(Object* pObject, char* aCharArray);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
