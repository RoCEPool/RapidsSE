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

    static void vAppendBannerHead  (char *azStr);
    static void vAppendBannerTail  (char *azStr);
    static void vAppendConcluding  (char *azStr);
    static void vAppendInflate     (char *azStr);
    static void vAppendPreliminary (char *azStr);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
