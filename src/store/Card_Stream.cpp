/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"

#include "CardIter.hpp"
#include "Card_AggElist.hpp"
//#include "Card_ELIST.hpp"
#include "Card_ProjElist.hpp"
#include "Card_Expr.hpp"
#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_StmColNm.hpp"
#include "Table.hpp"
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stream,CK_STREAM)
__aNewFunction(Card_Stream,CN_CARD)
__vDeleteFunction(Card_Stream,CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stream::Card_Stream(void)
: Card(CK_STREAM)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* produce composite <stream> of <aggreg>                                     */
/*----------------------------------------------------------------------------*/
Card_Stream* Card_Stream::pDerive_Of_aggreg(Card_AggElist* p_aggElist)
{
  _static_

  Card_Stream *p_stream = new Card_Stream;
  Card_StmColList *p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 i_Expr = 0;
  CardIter xCI_elist(p_aggElist);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {
    Card_As*   p_expr_as   = nullptr;
    Card_Type* p_expr_type = nullptr;

    i_Expr++;
    CardIter xCI_expr(p_expr);
    while (Card* p_expr_sub = xCI_expr.pEachChild()) {

      eCK e_expr_sub = p_expr_sub->eCard();

      if (CK_AS == e_expr_sub)
        p_expr_as = _pCard_As(p_expr_sub);
      else
      if (CK_TYPE == e_expr_sub)
        p_expr_type = _pCard_Type(p_expr_sub);

    }

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    if (p_expr_as)
      p_stmColNm->vValue(p_expr_as->szValue());
    else
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr);;
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* produce composite <stream> of <project>                                    */
/*----------------------------------------------------------------------------*/
Card_Stream* Card_Stream::pDerive_Of_project(Card_ProjElist* p_projElist)
{
  _static_

  Card_Stream *p_stream = new Card_Stream;
  Card_StmColList *p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 i_Expr = 0;
  CardIter xCI_elist(p_projElist);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {
    Card_As*   p_expr_as   = nullptr;
    Card_Type* p_expr_type = nullptr;

    i_Expr++;
    CardIter xCI_expr(p_expr);
    while (Card* p_expr_sub = xCI_expr.pEachChild()) {

      eCK e_expr_sub = p_expr_sub->eCard();

      if (CK_AS == e_expr_sub)
        p_expr_as = _pCard_As(p_expr_sub);
      else
      if (CK_TYPE == e_expr_sub)
        p_expr_type = _pCard_Type(p_expr_sub);

    }

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    if (p_expr_as)
      p_stmColNm->vValue(p_expr_as->szValue());
    else
      p_stmColNm->vValueSprint("__%lld"_Stz,i_Expr-1);
    p_stmCol->vAppend(p_stmColNm);

    Card_Type* p_type = _pCard_Type(p_expr_type->pDupCardAndSubCards());
    p_stmCol->vAppend(p_type);
  }

  return p_stream;
}


/*----------------------------------------------------------------------------*/
/* produce composite <stream> of table                                        */
/*----------------------------------------------------------------------------*/
Card_Stream* Card_Stream::pDerive_Of_table(Table* pTable)
{
  _static_

  if (!pTable)
    return nullptr;

  StreamDef* pStreamDef = pTable->pStreamDef();

  Card_Stream* p_stream = new Card_Stream;
  Card_StmColList* p_stmColList = new Card_StmColList;
  p_stream->vAppend(p_stmColList);

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_StmCol* p_stmCol = new Card_StmCol();
    p_stmCol->vValue(pColumn->szColNm());
    p_stmColList->vAppend(p_stmCol);

    Card_StmColNm* p_stmColNm = new Card_StmColNm();
    p_stmColNm->vValue(pColumn->szColNm());
    p_stmCol->vAppend(p_stmColNm);

    Stz* szType = aCT[pColumn->eColumnType()];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_stmCol->vAppend(pCard_Type);
  }

  return p_stream;
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal low                                                     */
/*----------------------------------------------------------------------------*/
i8 Card_Stream::iGetOdnLow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iOdnLo = Y_iGet(P_OdnLo);

  return iOdnLo;
}

/*----------------------------------------------------------------------------*/
/* get stream ordinal high                                                    */
/*----------------------------------------------------------------------------*/
i8 Card_Stream::iGetOdnHigh(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iOdnHi = Y_iGet(P_OdnHi);

  return iOdnHi;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
