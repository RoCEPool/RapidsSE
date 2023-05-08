/*============================================================================*/
/* OpDef.hpp -- Op Metadata                                                  */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class OpDef : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpDef(void);

    ~OpDef(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  OpDef* _pOpDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
