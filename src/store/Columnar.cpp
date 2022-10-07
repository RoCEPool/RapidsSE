/*============================================================================*/
/* Columnar.cpp --                                                           */
/*============================================================================*/

#include "Columnar.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Columnar,CN_COLUMNAR)
__aNewFunction(Columnar,CN_COLUMNAR)
__vDeleteFunction(Columnar,CN_COLUMNAR)
__aFooterFunction(Columnar)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Columnar::Columnar(Bench* pBench)
: TransformerTool(CN_COLUMNAR, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNAR,_FOOTER_);

  _checkInitialised_(CN_COLUMNAR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMNAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Columnar::~Columnar(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNAR,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add <term/<columnRef> to all ordinal <term>                                */
/*----------------------------------------------------------------------------*/
void Columnar::vAdd_columnRef_Under_term(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNAR,_FOOTER_);

  Resolver* pResolver = new Resolver(m_pBench);

  auto bfTerm_columnRef = [pResolver](Card* pCard) {
    Card_Term* p_term = _pCard_Term(pCard);

    Stz* szOrdinalTerm = p_term->sz();
    vTrace("bfTerm_columnRef: %s\n"_Stz, szOrdinalTerm);

    s32 sColumnRef;
    pResolver->vResolve_streamColOrdinal_To_tableColOrdinal(sColumnRef, szOrdinalTerm);

    Card_ColumnRef* p_columnRef = new Card_ColumnRef(sColumnRef);
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

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Columnar::vAdd_columnRef_Under_term"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
