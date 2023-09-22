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

  RASSERT(pCI->G.p_OP->bIsOne(CK_SCAN,CK_INDEXSCAN,CK_INDEXJOIN,CK_NONE));
  pCI->G.p_projection = pCI->G.p_project->pSub_projection();

#ifdef Z_FUSE

  if (pCI->G.p_OP->bIsOne(CK_SCAN,CK_INDEXSCAN,CK_NONE)) {
    // <op>/<stream> ; <op>/<stream>/<projection>
    //pCI->G.p_stream = pCI->G.p_OP->pSub_stream();
    pCI->G.p_arriving = (Card_Arriving *) pCI->G.p_stream->pSub_projection();
    pCI->G.p_arriving->vCard(CK_ARRIVING);

    if ((pCI->G.p_streamKey = pCI->G.p_arriving->pSub_streamKey())) {
      pCI->G.p_arriving->vDeleteChild(pCI->G.p_streamKey);
    }

    // <op>/<stream>/<projection> -> <op>/<stream>/<arriving>

    // <op>/<stream> <= <project>/<stream>/<projection>
    pCI->G.p_stream->vAppendChild(pCI->G.p_projection->pClone());
  }
  else {
    // index join

    pCI->G.p_arriving = (Card_Arriving *) pCI->G.p_stream->pSub_projection();
    pCI->G.p_arriving->vCard(CK_ARRIVING);

    if ((pCI->G.p_streamKey = pCI->G.p_arriving->pSub_streamKey())) {
      pCI->G.p_arriving->vDeleteChild(pCI->G.p_streamKey);
    }

    // <op>/<stream> <= <project>/<stream>/<projection>
    pCI->G.p_stream->vAppendChild(pCI->G.p_projection->pClone());
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* fuse same projection into this op                                          */
/*----------------------------------------------------------------------------*/
void Card_Stream::vFuseSame_projection(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->G.p_OP->bIsOne(CK_INDEXJOIN,CK_INDEXSCAN,CK_SCAN,CK_NONE));
  RASSERT(! pCI->G.p_projection);

#ifdef Z_FUSE

  // <op>/<stream> ; <op>/<stream>/<projection>
  //pCI->G.p_stream = pCI->G.p_OP->pSub_stream();
  pCI->G.p_projection = pCI->G.p_stream->pSub_projection();

  // <op>/<stream>/<projection> -> <arriving>
  pCI->G.p_arriving = (Card_Arriving*)pCI->G.p_projection->pClone();
  pCI->G.p_arriving->vCard(CK_ARRIVING);

  if ((pCI->G.p_streamKey = pCI->G.p_arriving->pSub_streamKey())) {
    pCI->G.p_arriving->vDeleteChild(pCI->G.p_streamKey);
  }

  // <project>/<stream> -> <project>/<stream>/<arriving>
  pCI->G.p_stream->vAppendChild(pCI->G.p_arriving);
#endif
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Stream::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pCI->pBoard()->pState()->bHasName(aOH[OH_PROLOG]))
    vValidate_mustHave(CK_PROJECTION,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
