/*============================================================================*/
/* XF_Epilog_Column_hashing.hpp -- Column Mapping                                    */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Column_hashing : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Column_hashing(CardIter* pCI);
   ~XF_Epilog_Column_hashing(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vXF_Column_Hashing(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Column_hashing* _pXF_Epilog_Column_hashing(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
