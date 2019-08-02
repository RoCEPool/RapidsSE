/*============================================================================*/
/* OpDesc.hpp -- the class which populates the op catalog                     */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OpDesc
  {
  public:

  public:
    OpDesc(eCN  eClass,     // class number
           eOK  eKind,      // op kind
           Stz* szCommand,  // first word of prompt command
           Stz* szPrefix,   // prefix

           Op*  (*fpNew)(Job_Dynamic* pJob, Stz* szLabel,
                         TokenVec* pTknVec, i8& i_Tkn_),
           
           eOR  (*fpCall)(void* aThis),   // function which calls the op
           void (*fpDrain)(void* aThis),  // function which drains the operand queue
           Stz* szHelp      // help text
          );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
