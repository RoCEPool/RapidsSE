/*============================================================================*/
/* OpDesc.cpp -- the class which populates the operator catalog               */
/*============================================================================*/

#include "Operators.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpDesc::OpDesc(eCN  eClass,     // class number
               eOK  eKind,      // op kind
               Stz* szCommand,  // first word of prompt command
               Stz* szPrefix,   // prefix
               
               bool bIsPartitioned, // is partitioned
               bool bCompilerOp,    // compiler op?
               
               eCN  eOutput,        // what kind of output
                                                // function which new's the operator
               Op*  (*fpNewOp)(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                               TokenVec* pTknVec, i8& i_Tkn),
               eOR  (*fpCall) (void* aThisOp),  // fun which calls op loop
               eOR  (*fpDrain)(void* aThisOp),  // fun which calls drain loop
               
               Stz* szHelp      // help text
              )
{
  i8 i_OpDefn = gxOpCatalog.m_nOpDefns++;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eClass    = eClass;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eKind     = eKind;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szCommand = szCommand;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szPrefix  = szPrefix;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bIsPartitioned = bIsPartitioned;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bCompilerOp    = bCompilerOp;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eOutput   = eOutput;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpNewOp   = fpNewOp;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpCall    = fpCall;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpDrain   = fpDrain;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szHelp    = szHelp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
