/*============================================================================*/
/* Cmd.hpp -- Command                                                         */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Cmd : public Operand
  {
  protected:
  
  public:
    void operator delete(void*);
  
    Cmd(eCN eClass);
   ~Cmd(void) noexcept(false);
  };

  Cmd* _pCmd(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
