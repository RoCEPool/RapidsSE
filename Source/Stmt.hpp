/*============================================================================*/
/* Stmt.hpp -- Statement                                                      */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Stmt : public Operand
  {
  protected:
  
  public:
    void operator delete(void*);
  
    Stmt(eCN eClass);
   ~Stmt(void) noexcept(false);
  };

  Stmt* _pStmt(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
