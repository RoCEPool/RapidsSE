/*============================================================================*/
/* Fragments.hpp -- Code fragments utility                                    */
/*============================================================================*/
#pragma once

namespace STORE
{

  //----------------------------------------------------------------------------

  class Fragments
  {
  private:

  public:

    static void vAppendBannerHead           (char *azStr);
    static void vAppendBannerTail           (char *azStr);
    static void vAppendConcludingFunc0      (char *azStr);
    static void vAppendConcludingFunc0_Agg  (char *azStr);
    static void vAppendConcludingFunc1      (char *azStr);
    static void vAppendConcludingFunc1_Agg  (char *azStr);
    static void vAppendConcludingFunc2  (char *azStr);
    static void vAppendConcludingFunc2_ (char *azStr);
    static void vAppendConcludingFunc3  (char *azStr);
    static void vAppendEmptyFunc        (char *azStr, Stz* szFunc);
    static void vAppendInflate          (char *azStr);
    static void vAppendPreliminaryFunc0 (char *azStr, Stz* szStageNm);
    static void vAppendPreliminaryFunc1 (char *azStr, Stz* szStageNm);
    static void vAppendPreliminaryFunc2 (char *azStr, Stz* szStageNm);
    static void vAppendPreliminaryFunc3 (char *azStr, Stz* szStageNm);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
