
/*============================================================================*/
/* Shape_v2.cpp -- Stencil Shape_v2                                                 */
/*============================================================================*/

#include "BASE.h"

#include "Shape_v2.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Shape_v2,CN_SHAPE_V2)
__aNewFunction(Shape_v2,CN_SHAPE_V2)
__vDeleteFunction(Shape_v2,CN_SHAPE_V2)
__aFooterFunction(Shape_v2)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Shape_v2::Shape_v2(void)
: Object(CN_SHAPE_V2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);
  
  m_eShape        = LS_NONE;
  m_pStructSynthA = nullptr;
  m_pStructSynthB = nullptr;
  m_pCodeSynthA   = nullptr;
  m_pCodeSynthB   = nullptr;
  m_pCodeSynthC   = nullptr;

  _checkInitialised_(CN_SHAPE_V2);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Shape_v2::~Shape_v2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

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
/* get Shape_v2 kind                                                          */
/*----------------------------------------------------------------------------*/
eLS Shape_v2::eShape(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  return m_eShape;
}

/*----------------------------------------------------------------------------*/
/* get structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* Shape_v2::pStructSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  return m_pStructSynthA;
}

/*----------------------------------------------------------------------------*/
/* set structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void Shape_v2::vStructSynthA(CodeSynth_v2* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  m_pStructSynthA = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* Shape_v2::pStructSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  return m_pStructSynthB;
}

/*----------------------------------------------------------------------------*/
/* set structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void Shape_v2::vStructSynthB(CodeSynth_v2* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  m_pStructSynthB = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* Shape_v2::pCodeSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  return m_pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
void Shape_v2::vCodeSynthA(CodeSynth_v2* pCodeSynthA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  m_pCodeSynthA = pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* Shape_v2::pCodeSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  return m_pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
void Shape_v2::vCodeSynthB(CodeSynth_v2* pCodeSynthB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  m_pCodeSynthB = pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* Shape_v2::pCodeSynthC(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  return m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
void Shape_v2::vCodeSynthC(CodeSynth_v2* pCodeSynthC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE_V2,_FOOTER_);

  m_pCodeSynthC = pCodeSynthC;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
