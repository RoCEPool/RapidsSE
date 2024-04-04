/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Stream, CK_s_STREAM)
__aNewFunction(Card_Stream,CN_CARD)
__vDeleteFunction(Card_Stream,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stream::Card_Stream(void)
: Card_symbol(CK_s_STREAM, 2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stream::~Card_Stream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* fuse next projection into this op                                          */
/*----------------------------------------------------------------------------*/
void Card_Stream::vFuse_project_Viable(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->E.p_Op->bEither(CK_o_SCAN, CK_o_INDEXSCAN, CK_o_INDEXJOIN, CK_o_CROSSJOIN, CK_NONE));
  pCI->Next.m_pEmitted = pCI->Next.m_pProject->pSub_emit();

  if ((pCI->E.p_emit = pCI->E.p_stream->pSub_emit()))
    pCI->E.p_stream->vDeleteChild(pCI->E.p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no arrived columns (only injected), emitted is next emitted

  // <scan> or <indexScan>

  if (pCI->E.p_Op->bEither(CK_o_SCAN, CK_o_INDEXSCAN, CK_NONE)) {
     pCI->E.p_stream->vAppendChild(pCI->Next.m_pEmitted->pClone());
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arrived (and injected) columns, emitted is next emitted

  // <indexJoin> or <crossJoin>

  //
  pCI->E.p_stream->vAppendChild(pCI->Next.m_pEmitted->pClone());

  // delete any SF_EXP_COL facet with SF_TBL_COL of this op's table
  pCI->E.p_emit = pCI->E.p_stream->pSub_emit();
  pCI->E.p_table = pCI->E.p_Op->pSub_table();
  char cTblSyn = pCI->E.p_table->pSymbol()->cTblSyn();
  pCI->E.p_emit->vDeleteFacetIf(pCI,SF_EXP_COL, SF_TBL_COL, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/* fuse same projection into this op                                          */
/*----------------------------------------------------------------------------*/
void Card_Stream::vFuse_project_NoViable(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->E.p_Op->bEither(CK_o_CROSSJOIN, CK_o_INDEXJOIN, CK_o_INDEXSCAN, CK_o_SCAN, CK_NONE));
  //RASSERT(! pCI->G.p_emit);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_o_AGGREG,
                                     CK_o_CROSSJOIN,
                                     CK_c_FOLD,
                                     CK_o_FILTER,
                                     CK_o_FROM,
                                     CK_o_HAVING,
                                     CK_o_INDEXJOIN,
                                     CK_o_INDEXSCAN,
                                     CK_o_INFLATE,
                                     CK_o_KEYEXISTS,
                                     CK_o_KEYNOTEXISTS,
                                     CK_o_LIMIT,
                                     CK_o_GBYAGG,
                                     CK_o_HASH,
                                     CK_o_PROJECT,
                                     CK_o_SCAN,
                                     CK_o_SORT,
                                     CK_o_WHERE,
                                     CK_NONE);

  if (pCI->pBoard()->pState()->bHasName(aOH[OH_PROLOG]))
    vValidate_mustHave(CK_t_EMIT, CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
