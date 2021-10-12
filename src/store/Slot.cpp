
/*============================================================================*/
/* Slot.cpp -- Stencil Slot                                                   */
/*============================================================================*/

#include "BASE.h"

#include "Slot.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Slot,CN_SLOT)
__aNewFunction(Slot,CN_SLOT)
__vDeleteFunction(Slot,CN_SLOT)
__aFooterFunction(Slot)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Slot::Slot(void)
: Object(CN_SLOT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);
  
  m_eSlot        = LS_NONE;
  m_pStructSynth = nullptr;
  m_pCodeSynthA  = nullptr;
  m_pCodeSynthB  = nullptr;
  m_pCodeSynthC  = nullptr;

  _checkInitialised_(CN_SLOT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Slot::~Slot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get slot kind                                                              */
/*----------------------------------------------------------------------------*/
eLS Slot::eSlot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  return m_eSlot;
}

/*----------------------------------------------------------------------------*/
/* get struct synthesiser                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Slot::pStructSynth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  return m_pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* set struct synthesiser                                                     */
/*----------------------------------------------------------------------------*/
void Slot::vStructSynth(CodeSynth* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  m_pStructSynth = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Slot::pCodeSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  return m_pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
void Slot::vCodeSynthA(CodeSynth* pCodeSynthA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  m_pCodeSynthA = pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Slot::pCodeSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  return m_pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
void Slot::vCodeSynthB(CodeSynth* pCodeSynthB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  m_pCodeSynthB = pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Slot::pCodeSynthC(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  return m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
void Slot::vCodeSynthC(CodeSynth* pCodeSynthC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLOT,_FOOTER_);

  m_pCodeSynthC = pCodeSynthC;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
