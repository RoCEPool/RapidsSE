/*============================================================================*/
/* XF_Morph_table_To_EEop.hpp -- Morph table to EE op                         */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Morph_table_To_EEop : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Morph_table_To_EEop(CardIter* pCI);
   ~XF_Morph_table_To_EEop(void) noexcept(false);

    static void vXF_Morph_table_To_EEop(CardIter* pCI);

    void vXF_Morph_table_To_EEop(void);

  private:
    Footer* aFooter(void);
  };

  XF_Morph_table_To_EEop* _pXF_Morph_table_To_EEop(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
