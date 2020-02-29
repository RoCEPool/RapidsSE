/*============================================================================*/
/* OpSpec.cpp -- Op Specification                                             */
/*============================================================================*/

#include "PipelineSpecs.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* OpSpec::operator new(size_t)
{
  OpSpec* pOpSpec = reinterpret_cast<OpSpec*>(HeapMem::pAllocate(CN_OPSPEC));
  
  return pOpSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void OpSpec::operator delete(void* aAddr)
{
  OpSpec* pOpSpec = reinterpret_cast<OpSpec*>(aAddr);

  HeapMem::vFree(pOpSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSpec::OpSpec(Stz* szOpNm, StageSpec* pStageSpec)
: Object(CN_OPSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sOpNm        = szOpNm;
  m_pStageSpec   = pStageSpec;
  
  m_bAppendRight = false;
  m_i_OpSpec     = C_UNKNOWN;
  m_eXmitKind    = XK_NORMAL;

  //m_oEnameSet
  //m_oTopologyEnameSet
  
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
void OpSpec::vAddEnamePair(Stz* sName, Stz* sValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oEnameSet.vAdd(sName, sValue);
}

/*----------------------------------------------------------------------------*/
/*add an ename pair                                                            */
/*----------------------------------------------------------------------------*/
void OpSpec::vAddEnamePair(Stz* sName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oEnameSet.vAdd(sName, iValue);
}

/*----------------------------------------------------------------------------*/
/* add an ename pair to hidden ename set                                      */
/*----------------------------------------------------------------------------*/
void OpSpec::vAddTopologyEnamePair(Stz* sName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oTopologyEnameSet.vAdd(sName, iValue);
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
/* get the EnameSet                                                           */
/*----------------------------------------------------------------------------*/
EnameSet* OpSpec::pEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return &m_oEnameSet;
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
/* replace an ename value                                                     */
/*----------------------------------------------------------------------------*/
void OpSpec::vReplaceTopologyEnamePair(Stz* szName, Stz* szValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oEnameSet.vReplace(szName, szValue);
}

/*----------------------------------------------------------------------------*/
/* replace an ename value                                                     */
/*----------------------------------------------------------------------------*/
void OpSpec::vReplaceTopologyEnamePair(Stz* szName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oTopologyEnameSet.vReplace(szName, iValue);
}

/*----------------------------------------------------------------------------*/
/* get the topology EnameSet                                                  */
/*----------------------------------------------------------------------------*/
EnameSet* OpSpec::pTopologyEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return &m_oTopologyEnameSet;
}

/*----------------------------------------------------------------------------*/
/* get stage kind                                                             */
/*----------------------------------------------------------------------------*/
eXK OpSpec::eXmitKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_eXmitKind;
}

/*----------------------------------------------------------------------------*/
/* set stage kind                                                             */
/*----------------------------------------------------------------------------*/
void OpSpec::vXmitKind(eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_eXmitKind = eXmitKind;
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
    
  m_oEnameSet.vPrint();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(OpSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
