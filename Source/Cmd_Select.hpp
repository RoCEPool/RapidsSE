/*============================================================================*/
/* Cmd_Select.hpp -- Select Request operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Cmd_Select : public Cmd
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Cmd_Select(Stz* szCmd);
   ~Cmd_Select(void) noexcept(false);
  
    Footer* aFooter(void);
  };

  Cmd_Select* _pCmd_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
