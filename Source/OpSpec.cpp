/*============================================================================*/
/* OpSpec.cpp -- Op Specification                                             */
/*============================================================================*/

#include "Pipelines.h"


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
OpSpec::OpSpec(Stz* szOpName, StageSpec* pStageSpec)
: Object(CN_OPSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sName = szOpName;
  m_bAppendRight = false;
  m_pStageSpec   = pStageSpec;
  m_i_OpSpec     = C_UNKNOWN;
  m_i_Slot = 0; // ALKA: C_UNKNOWN;

  //m_oEnameSet
  
  m_aOpDefn = nullptr;
  
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
/* get append right                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::vAddEnamePair(Stz* sName, Stz* sValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oEnameSet.vAdd(sName, sValue);
}

/*----------------------------------------------------------------------------*/
/* get append right                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::vAddEnamePair(Stz* sName, i8 iValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oEnameSet.vAdd(sName, iValue);
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
/* get the EnameSet                                                             */
/*----------------------------------------------------------------------------*/
EnameSet* OpSpec::pEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return &m_oEnameSet;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* OpSpec::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_sName.szStz();
}

/*----------------------------------------------------------------------------*/
/* set name                                                                   */
/*----------------------------------------------------------------------------*/
void OpSpec::vName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sName = szName;
}

/*----------------------------------------------------------------------------*/
/* get the op defn                                                            */
/*----------------------------------------------------------------------------*/
OpDefn* OpSpec::aOpDefn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_aOpDefn;
}

/*----------------------------------------------------------------------------*/
/* set the op defn                                                            */
/*----------------------------------------------------------------------------*/
void OpSpec::vOpDefn(OpDefn* aOpDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_aOpDefn = aOpDefn;
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
/* get i_Slot                                                                 */
/*----------------------------------------------------------------------------*/
i8 OpSpec::i_Slot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_i_Slot;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void OpSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  vTrace("[%lld] %s opDefn=%p "_Stz,m_i_OpSpec,m_sName.szStz(),m_aOpDefn);
    
  m_oEnameSet.vPrint();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(OpSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
