/*============================================================================*/
/* OpDesc.cpp -- the class which populates the operator catalog               */
/*============================================================================*/

#include "Pipelines.h"

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
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bCrwBuffer_in  = bCrwBuffer_in;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bEntBuffer_in  = bEntBuffer_in;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bIrwBuffer_in  = bIrwBuffer_in;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bSrtBuffer_in  = bSrtBuffer_in;

  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bCrwBuffer_out = bCrwBuffer_out;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bEntBuffer_out = bEntBuffer_out;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bIrwBuffer_out = bIrwBuffer_out;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_bSrtBuffer_out = bSrtBuffer_out;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eOutput   = eOutput;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpNewOp   = fpNewOp;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpCall    = fpCall;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpDrain   = fpDrain;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szHelp    = szHelp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
