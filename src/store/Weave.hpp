/*============================================================================*/
/* Weave.hpp -- Weave                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "Board.hpp"

namespace STORE
{
  class Weave : public Object
  {
  protected:
  Board* m_pBoard;
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Weave(Board* pBoard);
   ~Weave(void) noexcept(false);
  
    PipelineSpec* pToPipelineSpec(void);
  
  private:
    Footer* aFooter(void);
  };

  Weave* _pWeave(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
