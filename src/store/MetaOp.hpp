/*============================================================================*/
/* MetaOp.hpp -- Op Metadata                                                  */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class MetaOp : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaOp(void);

    ~MetaOp(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaOp* _pMetaOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
