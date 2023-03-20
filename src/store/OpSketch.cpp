
/*============================================================================*/
/* OpSketch.cpp -- Stencil Operator Sketch                                     */
/*============================================================================*/

#include "BASE.h"

#include "OpSketch.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,OpSketch,CN_OPSKETCH)
__aNewFunction(OpSketch,CN_OPSKETCH)
__vDeleteFunction(OpSketch,CN_OPSKETCH)
__aFooterFunction(OpSketch)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSketch::OpSketch(void)
: Object(CN_OPSKETCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  // definitions
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut= nullptr;
  m_pIndexDef= nullptr;

  // stencils
  m_eStencilFrom = SR_NONE;
  m_eStencilTo   = SR_NONE;

  // expression stacks
  m_pExprStack   = nullptr;
  m_pNullStack   = nullptr;
  m_pGbyStack    = nullptr;
  m_pOrdAggStack = nullptr;
  m_pLowerStack  = nullptr;
  m_pUpperStack  = nullptr;
  m_pInitStack   = nullptr;

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = nullptr;;

  m_iLimitRows = 0;

  /////////////////////////////////
  // TO GO
  m_eStencilSlot  = LS_NONE;
  m_pStructSynthA = nullptr;
  m_pStructSynthB = nullptr;
  m_pCodeSynthA   = nullptr;
  m_pCodeSynthB   = nullptr;
  m_pCodeSynthC   = nullptr;
  /////////////////////////////////

  _checkInitialised_(CN_OPSKETCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch::~OpSketch(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  delete m_pStructSynthA;
  delete m_pStructSynthB;
  delete m_pCodeSynthA;
  delete m_pCodeSynthB;
  delete m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get OpSketch kind                                                          */
/*----------------------------------------------------------------------------*/
eLS OpSketch::eShape(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_eStencilSlot;
}

/*----------------------------------------------------------------------------*/
/* get structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pStructSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pStructSynthA;
}

/*----------------------------------------------------------------------------*/
/* set structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void OpSketch::vStructSynthA(CodeSynth_v2* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pStructSynthA = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pStructSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pStructSynthB;
}

/*----------------------------------------------------------------------------*/
/* set structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void OpSketch::vStructSynthB(CodeSynth_v2* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pStructSynthB = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pCodeSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vCodeSynthA(CodeSynth_v2* pCodeSynthA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pCodeSynthA = pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pCodeSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vCodeSynthB(CodeSynth_v2* pCodeSynthB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pCodeSynthB = pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pCodeSynthC(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vCodeSynthC(CodeSynth_v2* pCodeSynthC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pCodeSynthC = pCodeSynthC;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
