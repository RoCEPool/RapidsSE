/*============================================================================*/
/* Permutor.cpp -- scan tool                                                    */
/*============================================================================*/

#include "Permutor.hpp"
#include "Resolver.hpp"

#include "PermGen.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Permutor, CN_PERMUTOR)
__aNewFunction(Permutor, CN_PERMUTOR)
__vDeleteFunction(Permutor, CN_PERMUTOR)
__aFooterFunction(Permutor)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Permutor::Permutor(Bench* pBench)
: TransformerTool(CN_PERMUTOR, pBench)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  _checkInitialised_(CN_PERMUTOR);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Permutor::~Permutor(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vCopy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  vSetupPermuteBoard();

  m_pBench->pCardCtl()->vNextHorizon(CK_PERMUTATION, true);

  m_p_ptndJoinSet =
      _pCard_PtndJoinSet(m_pBoard_partition->pFindMain(CK_PTNDJOINSET));

  m_p_comment = new Card_Comment;
  m_p_comment->vValue("PERMUTATIONS"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);

  m_p_subSet = m_p_ptndJoinSet->pDupSubCards();
  while ((m_pPoppedCard = m_p_subSet->pPop()))
    m_pBoard_permute->vAppendMainCard(m_pPoppedCard);
  DELETE_CARD(m_p_subSet);

  //--nn--------------==
  // x


  m_p_permutation = new Card_Permutation;
  m_pBoard_permute->vAppendMainCard(m_p_permutation);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);

  m_p_permutation = new Card_Permutation;
  m_pBoard_permute->vAppendMainCard(m_p_permutation);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);

  m_p_permutation = new Card_Permutation;
  m_pBoard_permute->vAppendMainCard(m_p_permutation);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);
  m_p_comment = new Card_Comment;
  m_p_comment->vValue("((to be coded))"_Stz);
  m_pBoard_permute->vAppendMainCard(m_p_comment);


  PermGen::vTest(1);
  PermGen::vTest(2);
  PermGen::vTest(3);
  PermGen::vTest(4);
  PermGen::vTest(5);
  PermGen::vTest(6);
  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard("Permutor::vCopy_ptndJoinSet"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
