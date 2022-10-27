/*============================================================================*/
/* TransformIterList.hpp --                                                    */
/*============================================================================*/
#pragma once

#include "Xsentinals.hpp"

namespace STORE
{
  class TransformIterList : public ListNL_t<Xsentinals>
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TransformIterList(void);
   ~TransformIterList(void) noexcept(false);

    Footer* aFooter(void);
  };

  TransformIterList* _pTransformIterList(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
