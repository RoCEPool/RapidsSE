
/*============================================================================*/
/* Shape.cpp -- Stencil Shape                                                 */
/*============================================================================*/

#include "BASE.h"

#include "Shape.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Shape,CN_SHAPE)
__aNewFunction(Shape,CN_SHAPE)
__vDeleteFunction(Shape,CN_SHAPE)
__aFooterFunction(Shape)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Shape::Shape(void)
: Object(CN_SHAPE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);
  
  m_eShape        = LS_NONE;
  m_pStructSynthA = nullptr;
  m_pStructSynthB = nullptr;
  m_pCodeSynthA   = nullptr;
  m_pCodeSynthB   = nullptr;
  m_pCodeSynthC   = nullptr;

  _checkInitialised_(CN_SHAPE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Shape::~Shape(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

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
/* get Shape kind                                                              */
/*----------------------------------------------------------------------------*/
eLS Shape::eShape(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  return m_eShape;
}

/*----------------------------------------------------------------------------*/
/* get structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth* Shape::pStructSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  return m_pStructSynthA;
}

/*----------------------------------------------------------------------------*/
/* set structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void Shape::vStructSynthA(CodeSynth* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  m_pStructSynthA = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth* Shape::pStructSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  return m_pStructSynthB;
}

/*----------------------------------------------------------------------------*/
/* set structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void Shape::vStructSynthB(CodeSynth* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  m_pStructSynthB = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Shape::pCodeSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  return m_pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
void Shape::vCodeSynthA(CodeSynth* pCodeSynthA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  m_pCodeSynthA = pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Shape::pCodeSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  return m_pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
void Shape::vCodeSynthB(CodeSynth* pCodeSynthB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  m_pCodeSynthB = pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth* Shape::pCodeSynthC(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  return m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
void Shape::vCodeSynthC(CodeSynth* pCodeSynthC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SHAPE,_FOOTER_);

  m_pCodeSynthC = pCodeSynthC;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
