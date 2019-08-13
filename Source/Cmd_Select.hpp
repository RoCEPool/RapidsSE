/*============================================================================*/
/* Cmd_Select.hpp -- Select Command operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Cmd_Select : public Cmd
  {
  private:
  
    TokenVec* m_pTokenVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Cmd_Select(void);
   ~Cmd_Select(void) noexcept(false);
  
    TokenVec* pTokenVec(void);

    Footer* aFooter(void);
  };

  Cmd_Select* _pCmd_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
