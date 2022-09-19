/*============================================================================*/
/* Streamer.cpp --                                                           */
/*============================================================================*/

#include "Streamer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Streamer,CN_STREAMER)
__aNewFunction(Streamer,CN_STREAMER)
__vDeleteFunction(Streamer,CN_STREAMER)
__aFooterFunction(Streamer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Streamer::Streamer(Bench* pBench)
: TransformerTool(CN_STREAMER, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  _checkInitialised_(CN_STREAMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Streamer::~Streamer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* stream                                                                     */
/*----------------------------------------------------------------------------*/
void Streamer::vStream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  i8 nMainCards = m_pBoard->nMainCards();
  for (i8 i_MainCard = 0; i_MainCard < nMainCards; i_MainCard++) {
    Card* pMainCard = m_pBoard->pGetMainCard(i_MainCard);

    vTrace("Resolving MainCard %lld\n"_Stz,i_MainCard);
    vResolveRecursive(pMainCard);
  }
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Streamer::vResolveRecursive(Card* pAnyCard) // recursive
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STREAMER, _FOOTER_);

  eCK eCard = pAnyCard->eCard();

  //
  // subcards first
  //

  //pCard->vDump();

  i8 nSubCards = pAnyCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card *pSubCard = pAnyCard->pGetSubCard(i_SubCard);
    vResolveRecursive(pSubCard);
  }

  //
  // this card
  //

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scan

  if (CK_SCAN == eCard) {
    Card_Table *pCard_Table = _pCard_Table(pAnyCard->pFindChildCard(CK_TABLE));
    if (pCard_Table) {

      StreamDef *pStreamDef = pCard_Table->pTable()->pStreamDef();

      Card_Stream *pCard_Stream = new Card_Stream;
      pAnyCard->vAppend(pCard_Stream);

      Card_StreamColList *pCard_StreamColList = new Card_StreamColList(pStreamDef);
      pCard_Stream->vAppend(pCard_StreamColList);
    }
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
