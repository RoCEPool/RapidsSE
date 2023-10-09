/*============================================================================*/
/* XF_Flatten_Fix_table_Pos.hpp -- Fix Table Position                         */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Fix_table_Pos : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Fix_table_Pos(CardIter* pCI);
   ~XF_Flatten_Fix_table_Pos(void) noexcept(false);

    static void vXF_Fix_table_Pos(CardIter* pCI);

    void vXF_Fix_table_Pos(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Fix_table_Pos* _pXF_Flatten_Fix_table_Pos(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
