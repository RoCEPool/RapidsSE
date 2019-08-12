/*============================================================================*/
/* ForthClump.hpp -- Forth Clump                                              */
/*============================================================================*/
#pragma once

namespace Forth
{
  class ForthClump : public Object
  {
  
  private:
  
    Cell*     m_aFipCell;     // points to cell inside a word
    LocalSet* m_pLocalsFrame; // local register frame chain
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
    Stack*    m_pAuxStack;    // auxiliary stack

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ForthClump(void);
   ~ForthClump(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  ForthClump* _pForthClump(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
