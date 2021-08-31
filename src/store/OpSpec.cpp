/*============================================================================*/
/* OpSpec.cpp -- Op Specification                                             */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

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
OpSpec::OpSpec(Stz* szOpNm, i8 i_OpSpec, StageSpec* pStageSpec)
: Object(CN_OPSPEC)
, DeleteHelper_t<OpSpec>(0) // typeid(OpSpec))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sOpNm         = szOpNm;
    
  m_pStageSpec    = pStageSpec;
  m_i_OpSpec      = i_OpSpec;

  //m_oArgTags
  //m_oTopologyTags

  m_eCrossTopo    = PX_NONE;
  m_i_NextStage   = C_UNKNOWN;
  m_i_NextOp      = C_UNKNOWN;
  m_i_OtherStage  = C_UNKNOWN;
  m_i_OtherOp     = C_UNKNOWN;

  m_bAppendRight  = false;
  m_eSendKind     = XK_UNICAST;

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
/* set append right                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::vAppendRight(bool bRight)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);

  m_bAppendRight = bRight;
}

/*----------------------------------------------------------------------------*/
/* get append right                                                           */
/*----------------------------------------------------------------------------*/
bool OpSpec::bAppendRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_bAppendRight;
}

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
ePX OpSpec::eCrossTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_eCrossTopo;
}

/*----------------------------------------------------------------------------*/
/* set cross topology                                                         */
/*----------------------------------------------------------------------------*/
void OpSpec::vCrossTopo(ePX eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_eCrossTopo = eCrossTopo;
}

/*----------------------------------------------------------------------------*/
/* set cross topology                                                         */
/*----------------------------------------------------------------------------*/
void OpSpec::vCrossTopo(ePX eCrossTopo, i8 i_NextStage, i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_eCrossTopo = eCrossTopo;
  m_i_NextStage = i_NextStage;
  m_i_NextOp = i_NextOp;
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
/* set the m_i_NextOp                                                         */
/*----------------------------------------------------------------------------*/
void OpSpec:: vNextOp(i8 i_NextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_NextOp = i_NextOp;
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
/* set the m_i_NextStage                                                      */
/*----------------------------------------------------------------------------*/
void OpSpec:: vNextStage(i8 i_NextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_NextStage = i_NextStage;
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
  
  return m_i_OpSpec;
}

/*----------------------------------------------------------------------------*/
/* set the i_opspec                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::vOpSpec(i8 i_OpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_OpSpec = i_OpSpec;
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
/* set the m_i_OtherOp                                                        */
/*----------------------------------------------------------------------------*/
void OpSpec:: vOtherOp(i8 i_OtherOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_OtherOp = i_OtherOp;
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
/* set the m_i_OtherStage                                                     */
/*----------------------------------------------------------------------------*/
void OpSpec:: vOtherStage(i8 i_OtherStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_OtherStage = i_OtherStage;
}

/*----------------------------------------------------------------------------*/
/* get stage kind                                                             */
/*----------------------------------------------------------------------------*/
eXK OpSpec::eSendKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_eSendKind;
}

/*----------------------------------------------------------------------------*/
/* set stage kind                                                             */
/*----------------------------------------------------------------------------*/
void OpSpec::vSendKind(eXK eSendKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_eSendKind = eSendKind;
}

/*----------------------------------------------------------------------------*/
/* get the stage spec                                                         */
/*----------------------------------------------------------------------------*/
StageSpec* OpSpec::pStageSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_pStageSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
