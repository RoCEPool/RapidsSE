/*============================================================================*/
/* ColsReq.hpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class ColsReq : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColsReq(void);

    ~ColsReq(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  ColsReq* _pColsReq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/