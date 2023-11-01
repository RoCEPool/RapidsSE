/*============================================================================*/
/* XF_Flatten_Anchor_table.hpp -- Fix Table Position                         */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Anchor_table : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Anchor_table(CardIter* pCI);
   ~XF_Flatten_Anchor_table(void) noexcept(false);

    static void vXF_Fix_table_Pos(CardIter* pCI);

    void vAnchor(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Anchor_table* _pXF_Flatten_Anchor_table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
