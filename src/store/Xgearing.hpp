/*============================================================================*/
/* Xgearing.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"
#include "Bench.hpp"

#include "PermGen.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

namespace STORE
{
  class Xgearing : public CardIter
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xgearing(eCN eClass, Bench* pBench);
   ~Xgearing(void) noexcept(false);

  protected:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
