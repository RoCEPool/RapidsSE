/*============================================================================*/
/* Transformer.cpp --                                                           */
/*============================================================================*/

#include "Transformer.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Transformer,CN_TRANSFORMER)
__aNewFunction(Transformer,CN_TRANSFORMER)
__vDeleteFunction(Transformer,CN_TRANSFORMER)
__aFooterFunction(Transformer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Transformer::Transformer(Optimiser* pOptimiser)
: Object(CN_TRANSFORMER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  m_pOptimiser   = pOptimiser;

  m_pBoard       = nullptr;
  m_pCard_SubSet = nullptr;

  m_pSimplifier  = nullptr;

  _checkInitialised_(CN_TRANSFORMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transformer::~Transformer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform                                                                  */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  m_pBoard = m_pOptimiser->pHeadBoard();
  m_pCard_SubSet = m_pBoard->pCard_SubSet();

  m_pSimplifier  = new Simplifier(m_pCard_SubSet);

  m_pSimplifier->vSimplify();

  delete m_pSimplifier;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
