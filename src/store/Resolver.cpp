/*============================================================================*/
/* Resolver.cpp --                                                           */
/*============================================================================*/

#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Resolver,CN_RESOLVER)
__aNewFunction(Resolver,CN_RESOLVER)
__vDeleteFunction(Resolver,CN_RESOLVER)
__aFooterFunction(Resolver)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Resolver::Resolver(Bench* pBench)
: OptimiserTool(CN_RESOLVER,pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);

  m_pStream_a    = nullptr;
  m_pStream_b    = nullptr;

  _checkInitialised_(CN_RESOLVER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Resolver::~Resolver(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* resolve                                                                    */
/*----------------------------------------------------------------------------*/
void Resolver::vResolve(Card* pMainCard, Stz* szTerm,
                        Stz*& rszStreamNm, Stz*& rszStreamColNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);

  rszStreamNm    = nullptr;
  rszStreamColNm = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* locate streams                                                             */
/*----------------------------------------------------------------------------*/
void Resolver::vLocateStreams(Card* pMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_RESOLVER, _FOOTER_);

  m_pStream_a = nullptr;
  m_pStream_b = nullptr;

  //i8 nMainCards = m_pCard_SubSet->nMainCards();
  for (i8 i_Test = pMainCard->iCardNo(); i_Test >= 0; i_Test--) {
    Card *pTestCard = m_pBoard->pGetMainCard(i_Test);

    if (CK_ABSJOIN == pTestCard->eCard()) {
      vLocateStreams_two(_pCard_AbsJoin(pTestCard));
      return;
    }

    if (CK_SCAN == pTestCard->eCard()) {

    }

  }
}

/*----------------------------------------------------------------------------*/
/* locate two streams (for absJoin)                                           */
/*----------------------------------------------------------------------------*/
void Resolver::vLocateStreams_two(Card_AbsJoin* pCard_AbsJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_RESOLVER, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
