/*============================================================================*/
/* Clause_From.hpp -- From Clause operand                                     */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_From : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_From(void);
   ~Clause_From(void) noexcept(false);

    Footer* aFooter(void);
  };

  Clause_From* _pClause_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
