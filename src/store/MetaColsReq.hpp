/*============================================================================*/
/* MetaColsReq.hpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class MetaColsReq : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaColsReq(void);

    ~MetaColsReq(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaColsReq* _pMetaColsReq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/