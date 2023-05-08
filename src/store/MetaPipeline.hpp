/*============================================================================*/
/* MetaPipeline.hpp -- Pipeline Metadata                                      */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class MetaPipeline : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaPipeline(void);

    ~MetaPipeline(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaPipeline* _pMetaPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/