/*============================================================================*/
/* OpSpec.cpp -- Op Specification                                             */
/*============================================================================*/

#include "Dynamics.h"


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
OpSpec::OpSpec(Stz* szOpName)
: Object(CN_OPSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  //m_sLabel
  m_sName = szOpName;
  m_bAppendRight = false;
  m_i_Slot = 0; // ALKA: C_UNKNOWN;

  //m_oEnmSet
  
  m_aOpDefn = nullptr;
  
  _checkInitialised_(CN_OPSPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSpec::OpSpec(Stz* szOpLabel, Stz* szOpName, bool bAppR, i8 i_Slot)
: Object(CN_OPSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_sLabel = szOpLabel;
  m_sName  = szOpName;
  m_bAppendRight = bAppR;
  m_i_Slot = i_Slot;

  //m_sArgs
  //m_oEnmSet
  
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
void OpSpec::vAddEnmPair(Stz* sName, Stz* sValue, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  m_oEnmSet.vAdd(sName, sValue, eToken);
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
/* get the enmSet                                                             */
/*----------------------------------------------------------------------------*/
EnmSet* OpSpec::pEnmSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return &m_oEnmSet;
}

/*----------------------------------------------------------------------------*/
/* get label                                                                  */
/*----------------------------------------------------------------------------*/
Stz* OpSpec::szLabel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_sLabel.aStz();
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* OpSpec::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSPEC,_FOOTER_);
  
  return m_sName.aStz();
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
  
  vTrace("%s "_Stz,m_sName.aStz());
    
  m_oEnmSet.vPrint();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(OpSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
