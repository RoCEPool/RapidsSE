/*============================================================================*/
/* ExitOperand.hpp -- Exit Operand                                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ExitOperand : public Operand
  {
  protected:
    Object* m_pObject;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);

    ExitOperand(Object* pObject);
   ~ExitOperand(void) noexcept(false);

    Object* pObject(void);
  
  private:
    Footer* aFooter(void);
  };

  ExitOperand* _pExitOperand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
