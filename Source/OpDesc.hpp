/*============================================================================*/
/* OpDesc.hpp -- the class which populates the op catalog                     */
/*============================================================================*/
#pragma once

namespace Operators
{
  class OpDesc
  {
  public:

  public:
    OpDesc(eCN  eClass,     // class number
           eOK  eKind,      // op kind
           Stz* szCommand,  // first word of prompt command
           Stz* szPrefix,   // prefix
           
           bool bIsPartitioned, // is operator partitioned
           bool m_bCompilerOp,  // is compiler op

           Op*  (*fpNewOp)(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
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
