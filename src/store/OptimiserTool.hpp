/*============================================================================*/
/* OptimiserTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"
#include "Optimiser.hpp"

namespace STORE
{
  class OptimiserTool : public Object
  {
  protected:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OptimiserTool(eCN eClass);
   ~OptimiserTool(void) noexcept(false);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
