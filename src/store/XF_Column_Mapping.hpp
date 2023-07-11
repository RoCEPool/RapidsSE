/*============================================================================*/
/* XF_Column_Mapping.hpp -- Column Mapping                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Column_Mapping : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Column_Mapping(CardIter* pCI);
   ~XF_Column_Mapping(void) noexcept(false);

    static void vXF_Column_Mapping(CardIter* pCI);

    void vXF_Column_Mapping(void);

  private:
    Footer* aFooter(void);
  };

  XF_Column_Mapping* _pXF_Column_Mapping(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
