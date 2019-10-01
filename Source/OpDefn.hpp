/*============================================================================*/
/* OpDefn.hpp -- a definition of an op class                                  */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  struct OpDefn
  {
  public:
    eCN  m_eClass;     // class enum of the op
    eOK  m_eKind;      // op kind

    Stz* m_szCommand;  // first word of prompt command
    Stz* m_szPrefix;   // prefix

    bool m_bIsPartitioned; // is partitioned
    bool m_bCompilerOp;    // is compiler op

    bool m_bCrwBuffer_in;  // accepts CrwBuffer?
    bool m_bEntBuffer_in;  // accepts EntBuffer?
    bool m_bIrwBuffer_in;  // accepts IrwBuffer?
    bool m_bSrtBuffer_in;  // accepts SrtBuffer?

    bool m_bCrwBuffer_out; // outputs CrwBuffer?
    bool m_bEntBuffer_out; // outputs EntBuffer?
    bool m_bIrwBuffer_out; // outputs IrwBuffer?
    bool m_bSrtBuffer_out; // outputs SrtBuffer?
  
    eCN  m_eOutput;        // what kind of output

    Op*  (*m_fpNewOp)(OpSpec* pOpSpec, Pipeline* pPipeline);
    eOR  (*m_fpCall) (void* aThisOp);  // function which calls the op loop
    eOR  (*m_fpDrain)(void* aThisOp);  // function which calls the drain loop

    Stz* m_szHelp;     // help text
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  OpDefn* _pOpDefn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
