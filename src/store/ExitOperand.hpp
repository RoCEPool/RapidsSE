/*============================================================================*/
/* ExitOperand.hpp -- Exit Operand                                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ExitOperand : public Operand
  {
  protected:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);

    ExitOperand(void);
   ~ExitOperand(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ExitOperand* _pExitOperand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
