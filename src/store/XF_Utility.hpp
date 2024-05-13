/*============================================================================*/
/* XF_Utility.hpp -- Verify dic                                    */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Utility : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Utility(CardIter* pCI);
   ~XF_Utility(void) noexcept(false);

    // - - - - - - - - - - - - - - - - - - - - - -

    static void vRemove_LeftMost(CardIter* pCI);
    void vRemove_LeftMost(void);

    // - - - - - - - - - - - - - - - - - - - - - -

    static void vValidate_board(CardIter* pCI);
    void vValidate_board(void);

    // - - - - - - - - - - - - - - - - - - - - - -

    static void vVerify_dic(CardIter* pCI);
    void vVerify_dic_loop(void);
    void vVerify_dic_OP(void);

  private:
    Footer* aFooter(void);
  };

  XF_Utility* _pXF_Utility(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
