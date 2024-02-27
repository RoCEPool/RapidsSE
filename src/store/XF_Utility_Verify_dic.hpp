/*============================================================================*/
/* XF_Utility_Verify_dic.hpp -- Validate Board                             */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Utility_Verify_dic : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Utility_Verify_dic(CardIter* pCI);
   ~XF_Utility_Verify_dic(void) noexcept(false);

    static void vVerify(CardIter* pCI);

    void vVerify(void);

  private:
    Footer* aFooter(void);
  };

  XF_Utility_Verify_dic* _pXF_Utility_Verify_dic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
