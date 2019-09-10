/*============================================================================*/
/* OpDesc.hpp -- the class which populates the operator catalog               */
/*============================================================================*/
#pragma once

namespace Operators
{
  class OpDesc
  {
  public:
    OpDesc(eCN  eClass,     // class number
           eOK  eKind,      // op kind
           Stz* szCommand,  // first word of prompt command
           Stz* szPrefix,   // prefix
           
           bool bIsPartitioned, // is operator partitioned
           bool m_bCompilerOp,  // is compiler op
           
           eCN  eOutput,        // what kind of output
           
                                           // function which new's the operator
           Op*  (*fpNewOp)(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                           TokenVec* pTknVec, i8& i_Tkn_),
           
           eOR  (*fpCall)(void* aThisOp),  // function which calls op loop
           eOR  (*fpDrain)(void* aThisOp), // function which drains drain loop
           
           Stz* szHelp      // help text
          );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
