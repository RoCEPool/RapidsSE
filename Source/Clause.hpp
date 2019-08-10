/*============================================================================*/
/* Clause.hpp -- Clause                                                       */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause : public Operand
  {
  protected:
  
  public:
    void operator delete(void*);
  
    Clause(eCN eClass);
   ~Clause(void) noexcept(false);
  };

  Clause* _pClause(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
