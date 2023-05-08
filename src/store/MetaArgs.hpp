/*============================================================================*/
/* MetaArgs.hpp -- Args Metadata                                              */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class MetaArgs : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaArgs(void);

    ~MetaArgs(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaArgs* _pMetaArgs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
