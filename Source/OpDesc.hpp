/*============================================================================*/
/* OpDesc.hpp -- the class which populates the op catalog                     */
/*============================================================================*/
#pragma once

namespace PushDown
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class OpDesc
  {
  public:

  public:
    OpDesc(eCN  eClass,     // class number
           eOK  eKind,      // op kind
           Stz* szCommand,  // first word of prompt command
           eOR  (*fpOpFun)(void* aThis),  // function which does the op
           void (*fpDrain)(void* aThis)   // function which drains the operand queue
          );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
