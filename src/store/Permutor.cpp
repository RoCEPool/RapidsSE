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

  m_pBoard_permute->vAppendMain(new Card_Comment("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  m_p_subSet = m_p_ptndJoinSet->pDupSubCards();
  while ((m_pPoppedCard = m_p_subSet->pPop())) {
    //m_pPoppedCard->vAddProperty_int("i_PtndJoin"_Stz,i_PtdnJoin++);
    m_pBoard_permute->vAppendMain(m_pPoppedCard);
  }
  DELETE_CARD(m_p_subSet);

  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard("Permutor::vCopy_ptndJoinSet"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vGenerate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

//  PermGen::vTest(4);

  //--nn--------------==
  // x


  m_pPermGen = new PermGen(m_p_ptndJoinSet->nChildCards());

  m_p_subSet = new Card_SubSet;

  while (m_pPermGen->bNextPermutation()) {
    m_pBoard_permute->vAppendMain((m_p_permutation = new Card_Permutation));
    m_p_permutation->vAddProperty_int("i_Permutation"_Stz,m_pPermGen->nPermutations()-1);

    RASSERT(0 == m_p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < m_pPermGen->nItems(); i_Item++) {
      i8 i_Child = m_pPermGen->iItem(i_Item);
      m_pChildCard = m_p_ptndJoinSet->pGetChild(i_Child);
      m_pBoard_permute->vAppendMain(m_pChildCard->pDupCardAndSubCards());

      m_p_ptndJoin = _pCard_PtndJoin(m_pChildCard);
      if (! m_p_ptndJoin->bMayFollowAfter(m_p_subSet)) {
        m_pBoard_permute->vAppendMain(new Card_Comment("**DOESN'T FOLLOW**"_Stz));
        m_p_permutation->vAddProperty_nameOnly("bInvalid"_Stz);
        break;
      }
      m_p_subSet->vAppend(m_pChildCard);
    }
    m_p_subSet->vRemoveAll();
  }

  DELETE_CARD(m_p_subSet);

  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard("Permutor::vGenerate_ptndPermutations"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Permutor::vRemove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMUTOR, _FOOTER_);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < m_pBoard_permute->nMainCards()) {
    m_pMainCard = m_pBoard_permute->pGetMainCard(i_Main);

    if (CK_PERMUTATION != m_pMainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! m_pMainCard->bHasProperty("bInvalid"_Stz)) {
      m_pMainCard->vAddProperty_int("i_Permutation"_Stz,i_Permutation++);
      i_Main++;
      continue;
    }

    m_pBoard_permute->vRemoveMainCard(m_pMainCard);
    while (i_Main < m_pBoard_permute->nMainCards()) {
      m_pMainCard = m_pBoard_permute->pGetMainCard(i_Main);
      if (CK_PTNDJOIN != m_pMainCard->eCard() && CK_COMMENT != m_pMainCard->eCard())
        break;
      m_pBoard_permute->vRemoveMainCard(m_pMainCard);
      delete m_pMainCard;
    }
  }

  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard("Permutor::vRemove_invalidPermutations"_Stz);


}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
