/*============================================================================*/
/* Tools.hpp -- Drafting Tools                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

namespace STORE
{
  class Tools : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Tools(void);
   ~Tools(void) noexcept(false);
  
    void vExpandStreamDefs(Bench* pBench);

  private:

    Footer* aFooter(void);
  };

  Tools* _pTools(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
