/*============================================================================*/
/* MetaStage.hpp -- Stage Metadata                                            */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class MetaStage : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaStage(void);

    ~MetaStage(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  MetaStage* _pMetaStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
