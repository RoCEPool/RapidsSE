/*============================================================================*/
/* OpSpec.cpp -- Op Specification                                             */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "OpSpec.hpp"
#include "PipelineSpec.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* OpSpec::operator new(size_t)
{
  OpSpec* pOpSpec = reinterpret_cast<OpSpec*>(Heaps::pAllocate(CN_OPSPEC));
  
  return pOpSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void OpSpec::operator delete(void* aAddr)
{
  OpSpec* pOpSpec = reinterpret_cast<OpSpec*>(aAddr);

  Heaps::vFree(pOpSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSpec::OpSpec(Stz* szOpNm, i8 i_OpSpec, StageSpec* pStageSpec)
: Object(CN_OPSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sPipelineNm   = pStageSpec->pPipelineSpec()->szPipelineNm();
  m_sStageNm      = pStageSpec->szStageNm();
  m_sOpNm         = szOpNm;
    
  m_pPipelineSpec = pStageSpec->pPipelineSpec();
  m_pStageSpec    = pStageSpec;
  m_i_OpSpec      = i_OpSpec;

  //m_oArgTags
  //m_oTopologyTags

  m_bAppendRight  = false;
  m_eSendKind     = XK_NORMAL;

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
i8 OpSpec::i_OpSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_i_OpSpec;
}

/*----------------------------------------------------------------------------*/
/* set the i_opspec                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::v_OpSpec(i8 i_OpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_i_OpSpec = i_OpSpec;
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

/*----------------------------------------------------------------------------*/
/* add an ename pair to hidden ename set                                      */
/*----------------------------------------------------------------------------*/
void OpSpec::vTopoTagAdd(Stz* sName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  UNUSED(eToken);

  m_oTopologyTags.vInsert(sName, iValue);
}

/*----------------------------------------------------------------------------*/
/* replace an ename value                                                     */
/*----------------------------------------------------------------------------*/
void OpSpec::vTopoTagReplace(Stz* szName, Stz* szValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  UNUSED(eToken);

  m_oArgTags.vReplace(szName, szValue);
}

/*----------------------------------------------------------------------------*/
/* replace an ename value                                                     */
/*----------------------------------------------------------------------------*/
void OpSpec::vTopoTagReplace(Stz* szName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  UNUSED(eToken);

  m_oTopologyTags.vReplace(szName, iValue);
}

/*----------------------------------------------------------------------------*/
/* get the topology tag set                                                   */
/*----------------------------------------------------------------------------*/
TagSet* OpSpec::pTopoTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return & m_oTopologyTags;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  vTrace("[%lld] %s "_Stz,m_i_OpSpec,m_sOpNm.szStz());
    
  m_oArgTags.vDumpSet();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(OpSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
