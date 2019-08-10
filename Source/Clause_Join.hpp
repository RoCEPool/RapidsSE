/*============================================================================*/
/* Clause_Join.hpp -- Join Clause operand                                     */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Join : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Join(void);
   ~Clause_Join(void) noexcept(false);

    Footer* aFooter(void);
  };

  Clause_Join* _pClause_Join(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
