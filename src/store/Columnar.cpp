/*============================================================================*/
/* Columnar.cpp --                                                           */
/*============================================================================*/

#include "Columnar.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Columnar,CN_TABULAR)
__aNewFunction(Columnar,CN_TABULAR)
__vDeleteFunction(Columnar,CN_TABULAR)
__aFooterFunction(Columnar)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Columnar::Columnar(Bench* pBench)
: TransformerTool(CN_TABULAR, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  _checkInitialised_(CN_TABULAR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Columnar::~Columnar(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add colummn ref's                                                          */
/*----------------------------------------------------------------------------*/
void Columnar::vAdd_ColumnRef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABULAR,_FOOTER_);

  Resolver* pResolver = new Resolver(m_pBench);

  auto bfTerm_columnRef = [pResolver](Card* pCard) {
    Card_Term* p_term = _pCard_Term(pCard);

    Stz* szTerm = p_term->szTerm();
    vTrace("bfTerm_columnRef: %s\n"_Stz,szTerm);

    s32 sColumnRef;
    pResolver->vResolve_columnRef(sColumnRef,szTerm);

    Card_ColumnRef* p_columnRef = new Card_ColumnRef;
    p_columnRef->vColumnRef(sColumnRef);
    p_term->vAppend(p_columnRef);

    return true;
  };

  // - - - - - - - - -

  m_pBoard = m_pBench->pGetBoard(BK_QUERY);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain()) ) {
    if (! (m_p_calcBool = _pGetSub_CalcBool(m_pMainCard)) )
      continue;

    m_p_calcBool->vVisitAllSub(CK_TERM,bfTerm_columnRef);
  }

  delete pResolver;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
