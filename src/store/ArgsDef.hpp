/*============================================================================*/
/* ArgsDef.hpp -- Args Metadata                                               */
/*============================================================================*/

#include "Meta.hpp"

namespace STORE
{
  class ArgsDef : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ArgsDef(void);

    ~ArgsDef(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  ArgsDef* _pArgsDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
