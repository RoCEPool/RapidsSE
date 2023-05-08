/*============================================================================*/
/* ArgDef.hpp -- Args Metadata                                               */
/*============================================================================*/
#pragma once

#include "Meta.hpp"

namespace STORE
{
  class ArgDef : public Meta
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ArgDef(void);

    ~ArgDef(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  ArgDef* _pArgDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
