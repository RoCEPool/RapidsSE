/*============================================================================*/
/* Card_StmColOdn.cpp -- Stream Column Name                                    */
/*============================================================================*/

#include "Card_StmColOdn.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_StmColOdn, CK_STMCOLODN)
__aNewFunction(Card_StmColOdn, CN_CARD)
__vDeleteFunction(Card_StmColOdn, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StmColOdn::Card_StmColOdn(void)
: Card(CK_STMCOLODN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StmColOdn::~Card_StmColOdn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColOdn* Card_StmColOdn::
pResolve_tblColOdn_Into_stmColOdn(CardIter* pCI, Card_Term* p_term_tblColOdn, i8 i_Before)
{
  _static_

  Card_StmColOdn* p_term_stmColOdn = nullptr;

  pCI->G_vPushIter();

  Stz* szTblColOdn = p_term_tblColOdn->szValue();
  if (pCI->G_bResolveSubCard(CK_STMCOL,CK_TBLCOLODN,szTblColOdn,i_Before)) {
    RASSERT(pCI->m_pCard_resolved->bIsCard(CK_STMCOL));

    p_term_stmColOdn = pCI->m_pCard_resolved->pSub_stmColOdn();
  }

  pCI->G_vPopIter();

  return p_term_stmColOdn;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
