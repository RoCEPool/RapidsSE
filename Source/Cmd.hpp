/*============================================================================*/
/* Cmd.hpp -- Command                                                         */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Cmd : public Operand
  {
  protected:
    s4K m_sCmd;
  
  public:
    void operator delete(void*);
  
    Cmd(eCN eClass, Stz* szCmd);
   ~Cmd(void) noexcept(false);
  
    Stz* szCmd(void);
  };

  Cmd* _pCmd(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
