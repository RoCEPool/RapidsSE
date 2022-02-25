/*============================================================================*/
/* Action.hpp -- Optimiser Actions                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"
#include "Board.hpp"

namespace STORE
{
  class Action : public Object
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Action(void);
   ~Action(void) noexcept(false);
  
  public:
  
  private:
    Footer* aFooter(void);
  };

  Action* _pAction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
