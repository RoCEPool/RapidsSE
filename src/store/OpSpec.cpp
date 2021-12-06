/*============================================================================*/
/* OpSpec.cpp -- Op Specification                                             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "OpSpec.hpp"
#include "PipelineSpec.hpp"
#include "StageSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,OpSpec,CN_OPSPEC)
__aNewFunction(OpSpec,CN_OPSPEC)
__vDeleteFunction(OpSpec,CN_OPSPEC)
__aFooterFunction(OpSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSpec::OpSpec(Stz* szOpNm, StageSpec* pStageSpec)
: Object(CN_OPSPEC)
, LifeGuard_t<OpSpec>(this,CN_OPSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sOpNm         = szOpNm;
    
  m_pStageSpec    = pStageSpec;

  //m_oArgTags
  //m_oTopologyTags

  m_eStreamTopology_op = YS_NONE;
  m_eParcelForward     = YF_NONE;

  m_eCrossTopology    = YX_NONE;

  m_i_NextStage   = C_UNKNOWN;
  m_i_NextOp      = C_UNKNOWN;
  
  m_i_OtherStage  = C_UNKNOWN;
  m_i_OtherOp     = C_UNKNOWN;

  m_eSendCast     = XK_UNICAST;

  _checkInitialised_(CN_OPSPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OpSpec::~OpSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an ename pair                                                          */
/*----------------------------------------------------------------------------*/
void OpSpec::vArgTagAdd(Stz* sName, Stz* sValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  UNUSED(eToken);
  
  m_oArgTags.vInsert(sName, sValue);
}

/*----------------------------------------------------------------------------*/
/*add an ename pair                                                            */
/*----------------------------------------------------------------------------*/
void OpSpec::vArgTagAdd(Stz* sName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  UNUSED(eToken);

  m_oArgTags.vInsert(sName, iValue);
}

/*----------------------------------------------------------------------------*/
/* get the argument tag set                                                   */
/*----------------------------------------------------------------------------*/
TagSet* OpSpec::pArgTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return &m_oArgTags;
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
eYX OpSpec::eCrossTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  //eYX eCheck = m_pStageSpec->eCrossTopology();
  //RASSERT(eCheck == m_eCrossTopology);
  
  return m_eCrossTopology;
}

/*----------------------------------------------------------------------------*/
/* set cross topology                                                         */
/*----------------------------------------------------------------------------*/
void OpSpec::vCrossTopology(eYX eCrossTopology,
                        Stz* szNextStageNm, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
    
  RASSERT(szNextStageNm);
  
  m_eCrossTopology   = eCrossTopology;

  m_sNextStageNm = szNextStageNm;
  m_i_NextStage  = i_NextStage;
  m_i_NextOp     = i_NextOp;
}

/*----------------------------------------------------------------------------*/
/* get m_i_NextOp                                                             */
/*----------------------------------------------------------------------------*/
i8 OpSpec::iNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_i_NextOp;
}

/*----------------------------------------------------------------------------*/
/* get m_i_NextStage                                                          */
/*----------------------------------------------------------------------------*/
i8 OpSpec::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_i_NextStage;
}

/*----------------------------------------------------------------------------*/
/* get next stage name                                                        */
/*----------------------------------------------------------------------------*/
Stz* OpSpec::szNextStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_sNextStageNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set the next stage                                                         */
/*----------------------------------------------------------------------------*/
void OpSpec::vNextStageOp(i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_NextStage  = i_NextStage;
  m_i_NextOp     = i_NextOp;
}

/*----------------------------------------------------------------------------*/
/* set the next stage                                                         */
/*----------------------------------------------------------------------------*/
void OpSpec::vNextStageOp(Stz* szNextStageNm, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sNextStageNm = szNextStageNm;
  m_i_NextStage  = i_NextStage;
  m_i_NextOp     = i_NextOp;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* OpSpec::szOpNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_sOpNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set name                                                                   */
/*----------------------------------------------------------------------------*/
void OpSpec::vOpNm(Stz* szOpNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);

  m_sOpNm = szOpNm;
}

/*----------------------------------------------------------------------------*/
/* get i_OpSpec                                                               */
/*----------------------------------------------------------------------------*/
i8 OpSpec::iOpSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  i8 i_OpSpec = m_pStageSpec->iFind(this);

  return i_OpSpec;
}

/*----------------------------------------------------------------------------*/
/* get m_i_OtherOp                                                            */
/*----------------------------------------------------------------------------*/
i8 OpSpec::iOtherOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_i_OtherOp;
}

/*----------------------------------------------------------------------------*/
/* get m_i_OtherStage                                                         */
/*----------------------------------------------------------------------------*/
i8 OpSpec::iOtherStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_i_OtherStage;
}

/*----------------------------------------------------------------------------*/
/* get other stage name                                                       */
/*----------------------------------------------------------------------------*/
Stz* OpSpec::szOtherStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_sOtherStageNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set the m_i_OtherStage                                                     */
/*----------------------------------------------------------------------------*/
void OpSpec::vOtherStageOp(Stz* szOtherStageNm,
                            i8 i_OtherStage,
                            i8 i_OtherOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sOtherStageNm = szOtherStageNm;
  m_i_OtherStage  = i_OtherStage;
  m_i_OtherOp     = i_OtherOp;
}

/*----------------------------------------------------------------------------*/
/* get m_eParcelForward                                                       */
/*----------------------------------------------------------------------------*/
eYF OpSpec::eParcelForward(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_eParcelForward;
}

/*----------------------------------------------------------------------------*/
/* get send cast kind (if send op)                                            */
/*----------------------------------------------------------------------------*/
eXK OpSpec::eSendCast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_eSendCast;
}

/*----------------------------------------------------------------------------*/
/* set send cast kind (if send op)                                            */
/*----------------------------------------------------------------------------*/
void OpSpec::vSendCast(eXK eCastKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_eSendCast = eCastKind;
}

/*----------------------------------------------------------------------------*/
/* get the stage spec                                                         */
/*----------------------------------------------------------------------------*/
StageSpec* OpSpec::pStageSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get stream topology                                                        */
/*----------------------------------------------------------------------------*/
eYS OpSpec::eStreamTopology_op(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_eStreamTopology_op;
}

/*----------------------------------------------------------------------------*/
/* set stream topology                                                        */
/*----------------------------------------------------------------------------*/
void OpSpec::vStreamTopology_op(eYS eStreamTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_eStreamTopology_op = eStreamTopology;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
