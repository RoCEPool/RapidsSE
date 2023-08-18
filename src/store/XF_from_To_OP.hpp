/*============================================================================*/
/* XF_from_To_OP.hpp -- Morph table to EE op                         */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_from_To_OP : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_from_To_OP(CardIter* pCI);
   ~XF_from_To_OP(void) noexcept(false);

    static void vXF_Morph_table_To_EEop(CardIter* pCI);

    void vXF_Morph_table_To_EEop(void);

  private:
    Footer* aFooter(void);
  };

  XF_from_To_OP* _pXF_from_To_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
