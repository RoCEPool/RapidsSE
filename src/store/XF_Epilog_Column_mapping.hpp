/*============================================================================*/
/* XF_Epilog_Column_mapping.hpp -- Column Mapping                                    */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Column_mapping : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Column_mapping(CardIter* pCI);
   ~XF_Epilog_Column_mapping(void) noexcept(false);

    static void vXF_Column_Mapping(CardIter* pCI);

    void vXF_Column_Mapping(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Column_mapping* _pXF_Epilog_Column_mapping(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
