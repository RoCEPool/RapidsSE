/*============================================================================*/
/* OpDesc.hpp -- the class which populates the operator catalog               */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class OpDesc
  {
  public:
    OpDesc(eCN  eClass,     // class number
           eOK  eKind,      // op kind
           Stz* szCommand,  // first word of prompt command
           Stz* szPrefix,   // prefix
           
           bool bIsPartitioned, // is operator partitioned
           bool bCompilerOp,    // is compiler op
           
           bool bCrwBuffer_in,  // accepts CrwBuffer?
           bool bEntBuffer_in,  // accepts EntBuffer?
           bool bIrwBuffer_in,  // accepts IrwBuffer?
           bool bSrtBuffer_in,  // accepts SrtBuffer?
           
           bool bCrwBuffer_out, // outputs CrwBuffer?
           bool bEntBuffer_out, // outputs EntBuffer?
           bool bIrwBuffer_out, // outputs IrwBuffer?
           bool bSrtBuffer_out, // outputs SrtBuffer?
           
           eCN  eOutput,        // what kind of output
           
                                           // function which new's the operator
           Op*  (*fpNewOp)(OpSpec* pOpSpec, Pipeline* pPipeline),
           
           eOR  (*fpCall)(void* aThisOp),  // function which calls op loop
           eOR  (*fpDrain)(void* aThisOp), // function which drains drain loop
           
           Stz* szHelp      // help text
          );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
