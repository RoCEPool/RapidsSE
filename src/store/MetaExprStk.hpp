/*============================================================================*/
/* MetaExprStk.hpp -- Expression Stack Metadata                               */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class MetaExprStk : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaExprStk(void);

    ~MetaExprStk(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaExprStk* _pMetaExprStk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

