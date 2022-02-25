/*============================================================================*/
/* Card_Project.cpp -- Project                                                */
/*============================================================================*/

#include "Card_Project.hpp"

#include "Card_AggElist.hpp"
#include "Card_As.hpp"
#include "Card_CalcExpr.hpp"
#include "Card_ProjElist.hpp"
#include "Card_StreamDef.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Project,CK_PROJECT)
__aNewFunction(Card_Project,CN_CARD)
__vDeleteFunction(Card_Project,CN_CARD)

__bIsCard(STORE,Card_Project,CK_PROJECT)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Project::Card_Project(eCK eCard)
: Card_MAIN(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Project::~Card_Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce the streamdef of a projective work unit                            */
/*----------------------------------------------------------------------------*/
StreamDef* Card_Project::pProduceStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  /*

    A projection pile can be one of the following:
   
      AggregList .7    //  OrdAggList .4    //  ProjectList .6
              AL_NODE  //          AL_NONE  //  .0)Project
      .0)Project       //  .0)Project       //  ..0)Type
      ..0)Type         //  ..0)Type         //          INTEGER
              INTEGER  //          INTEGER  //  ..1)As
      ..1)As           //  ..1)PrjPred      //          CUSTKEY
              CUSTKEY  //       L_ORDERKEY  //  ..2)PrjPred
      ..2)PrjPred      //                   //     L_LINENUMBER
           L_ORDERKEY  //                   //
                       //                   //
      .4)Aggreg        //  .1)Aggreg        //
              AL_NONE  //          AL_NONE  //
      ..0)Type         //  ..0)Type         //
              DECIMAL  //          DECIMAL  //
      ..1)As           //  ..1)AggPred      //
              SUM_QTY  //                )  //
      ..2)AggPred      //       L_QUANTITY  //
                    )  //              sum  //
           L_QUANTITY  //                   //
                  sum  //                   //
      0123456789ABCDE  //  0123456789ABCDE  //  0123456789ABCDE
  
  */
  
  StreamDef* pStreamDef_projected = new StreamDef;

  Card_StreamDef* pCard_StreamDef_projected
    = new Card_StreamDef(pStreamDef_projected);
  vAppend(pCard_StreamDef_projected);
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given projective part and add its projected column to streamDef*/
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vAddColumn = [&pStreamDef_projected](Card* pProjective) mutable
  {
    // a projection card will either be an aggreg or a project
//    ASSERT(_bIsCard_Aggreg(pItem_Projection) ||
//           _bIsCard_Project(pItem_Projection));
  
    //vTrace("vAddColumn: %s\n"_Stz,pProjectivePart->szCardNm());
    //pProjectivePart->vDump();
  
    s32 sColNm  = "(unknown)"_Stz;
    eCT eColumn = CT_NONE;;
    i8  iLength = 0;
    i8  iScale  = 0;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // TYPE (mandatory)
    Card_Type* pElement_Type
      = _pCard_Type(pProjective->pFindSubCard(CK_TYPE));
    if (pElement_Type) {
      eColumn = Column::eColumn(pElement_Type->szType());
      //vTrace("vAddColumn: Type %s\n"_Stz,pElement_Type->szType());
    }
  
    // AS (optional)
    Card_As* pElement_As
      = _pCard_As(pProjective->pFindSubCard(CK_AS));
    if (pElement_As) {
      sColNm = pElement_As->szAs();
      //vTrace("vAddColumn: As %s\n"_Stz,sColNm.azChar());
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a projective part with have an AGGREGPRED element
  
//    Card_AggPred* pElement_AggregPred
//      = _pCard_AggPred(pProjectivePart->pFindSubCard(CK_AGGPRED));
//
//    if (pElement_AggregPred) {
//      Card_Term* pCard_Term = _pCard_Term(pElement_AggregPred->pGet(0));
//      sColNm = pCard_Term->szGetProperty("szTerm"_Stz);
//      vTrace("AGGREGPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);
//
//      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);
//
//      return;
//    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or a PROJECTPRED element
  
    Card_CalcExpr* pElement_ProjectPred
      = _pCard_CalcExpr(pProjective->pFindSubCard(CK_CALCEXPR));

    if (pElement_ProjectPred) {
      Card_Term* pCard_Term = _pCard_Term(pElement_ProjectPred->pGet(0));
      sColNm = pCard_Term->szGetProperty("szTerm"_Stz);
      //vTrace("PROJECTPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);

      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);
      
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ??
    NEVER_REACH
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // add the columns from all
  //   aggreg, ordagg and project cards of the given pile
  //   to the streamdef
  //
  
  
  Card_ELIST* pCard_ELIST;
  
  pCard_ELIST = _pCard_AggElist(pFindSubCard(CK_AGGELIST));
  if (! pCard_ELIST)
    pCard_ELIST = _pCard_ProjElist(pFindSubCard(CK_PROJELIST));

  ListExoIter2_t<Card>* pSubCard_I = pCard_ELIST->pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pUnit = pSubCard_I->pNext();
    
    eCK eCard = pUnit->eCard();
    if (CK_TYPEDEXPR == eCard)
      vAddColumn(pUnit);
  }
  
  delete pSubCard_I;

  return pStreamDef_projected;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
