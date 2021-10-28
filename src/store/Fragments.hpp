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

    static void vAppendBannerHead       (char *azStr, Stz* szFunc);
    static void vAppendBannerTail       (char *azStr);
    static void vAppendConcludingFunc0  (char *azStr);
    static void vAppendConcludingFunc1  (char *azStr);
    static void vAppendInflate          (char *azStr);
    static void vAppendPreliminaryFunc0 (char *azStr);
    static void vAppendPreliminaryFunc1 (char *azStr);
    static void vAppendPreliminaryFunc2 (char *azStr);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
