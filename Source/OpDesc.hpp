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

           Op*  (*fpNew)(Stz* szLabel, Digest* pDigest, OpDefn* aOpDefn,
                         TokenVec* pTknVec, i8& i_Tkn_),
           
           eOR  (*fpCall)(void* aThisOp),  // function which calls the op loop
           eOR  (*fpDrain)(void* aThisOp), // function which drains the drain loop
           Stz* szHelp      // help text
          );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
