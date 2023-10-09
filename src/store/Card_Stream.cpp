/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"

#include "CardIter.hpp"

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
: Card_symbol(CK_STREAM, 2)
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
void Card_Stream::vFuseNext_projection(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->G.p_OP->bIsOne(CK_SCAN,CK_INDEXSCAN,CK_INDEXJOIN,CK_CROSSJOIN,CK_NONE));
  pCI->Next.m_pEmitted = pCI->Next.m_pProject->pSub_emitted();

  if ((pCI->G.p_emitted = pCI->G.p_stream->pSub_emitted()))
    pCI->G.p_stream->vDeleteChild(pCI->G.p_emitted);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no arrived columns, only injected

  if (pCI->G.p_OP->bIsOne(CK_SCAN,CK_INDEXSCAN,CK_NONE)) {
     pCI->G.p_stream->vAppendChild(pCI->Next.m_pEmitted->pClone());
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arrived and injected columns

  // index join

  pCI->G.p_CARD = pCI->Prior.m_pEmitted->pDupCardRecursive();
  pCI->G.p_CARD->vCard(CK_ARRIVING);
  if ((pCI->G.p_streamKey = pCI->G.p_CARD->pSub_streamKey()))
    pCI->G.p_CARD->vDeleteChild(pCI->G.p_streamKey);

  pCI->G.p_stream->vAppendChild(pCI->G.p_CARD);

  pCI->G.p_stream->vAppendChild(pCI->Next.m_pEmitted->pClone());
}

/*----------------------------------------------------------------------------*/
/* fuse same projection into this op                                          */
/*----------------------------------------------------------------------------*/
void Card_Stream::vFuseSame_projection(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->G.p_OP->bIsOne(CK_CROSSJOIN,CK_INDEXJOIN,CK_INDEXSCAN,CK_SCAN,CK_NONE));
  //RASSERT(! pCI->G.p_emitted);

  if (pCI->G.p_OP->bIsOne(CK_SCAN,CK_INDEXSCAN,CK_NONE)) {

    // <op>/<stream> ; <op>/<stream>/<projection>
    //pCI->G.p_stream = pCI->G.p_OP->pSub_stream();
    pCI->G.p_emitted = pCI->G.p_stream->pSub_emitted();

    // <op>/<stream>/<projection> -> <arriving>
    pCI->G.p_arriving = (Card_Arriving *) pCI->G.p_emitted->pClone();
    pCI->G.p_arriving->vCard(CK_ARRIVING);

    if ((pCI->G.p_streamKey = pCI->G.p_arriving->pSub_streamKey())) {
      pCI->G.p_arriving->vDeleteChild(pCI->G.p_streamKey);
    }

    // <project>/<stream> -> <project>/<stream>/<arriving>
    pCI->G.p_stream->vAppendChild(pCI->G.p_arriving);
  }
  else {
    pCI->G.p_CARD = pCI->Prior.m_pEmitted->pDupCardRecursive();
    pCI->G.p_CARD->vCard(CK_ARRIVING);

    if ((pCI->G.p_streamKey = pCI->G.p_CARD->pSub_streamKey())) {
      pCI->G.p_CARD->vDeleteChild(pCI->G.p_streamKey);
    }

    // <op>/<stream> <= <project>/<stream>/<projection>
    pCI->G.p_stream->vAppendChild(pCI->G.p_CARD);

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pCI->pBoard()->pState()->bHasName(aOH[OH_PROLOG]))
    vValidate_mustHave(CK_EMITTED, CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
