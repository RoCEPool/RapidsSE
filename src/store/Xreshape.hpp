/*============================================================================*/
/* Xreshape.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xderive.hpp"


namespace STORE
{
  class Table;

  class Xreshape : public Xderive
  {
  public:

    // must be last
    Footer m_xFooter;


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xreshape(eCN eClass);
   ~Xreshape(void) noexcept(false);

    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
