/*============================================================================*/
/* PermGen.cpp -- permutation generator                                       */
/*============================================================================*/

#include "PermGen.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PermGen,CN_PERMGEN)
__aNewFunction(PermGen,CN_PERMGEN)
__vDeleteFunction(PermGen,CN_PERMGEN)
__aFooterFunction(PermGen)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PermGen::PermGen(i8 nItems)
: Object(CN_PERMGEN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  m_nItems = nItems;
  m_nPermutations = 0;

  // initialise the sequencing array
  m_iaItem[0] = -1;
  for (i8 i_Item = 1; i_Item < C_PERMGEN_ITEMS; i_Item++)
    m_iaItem[i_Item] = 0;

  _checkInitialised_(CN_PERMGEN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PermGen::~PermGen(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vTest(i8 nItems)
{
  _static_

  PermGen* pPermGen = new PermGen(nItems);

  i8 nPermutations = 0;
  while (pPermGen->bNextPermutation())
    nPermutations++;

  vTrace("%lld permutations in a set of %lld items\n"_Stz,nPermutations,nItems);
  delete pPermGen;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::iItem(i8 i_Position)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  i8 i_Item = m_nItems - 1 - i_Position;

  return m_iaItem[i_Item];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool PermGen::bNextPermutation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  for (;;) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // do ++[0] stepping, testing for a valid permutation of current subsequence

    for (;;) {
      if (++m_iaItem[0] >= m_nItems)
        break;
      if (bIsValidPermutation()) {
        m_nPermutations++;
        //vPrint("IS "_Stz);
        return true;
      }
//      else {
//        vPrint("NO "_Stz);
//      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did not find a valid permutation in current subsequence

    // ripple rightwards incrementing [i_Right] looking for next subsequence
    i8 i_Right;
    for (i_Right = 1; i_Right < m_nItems; i_Right++) {

      // continue looking for longer subsequence if subsequence at i_Right is exhausted
      if (++m_iaItem[i_Right] >= m_nItems)
        continue;

      // found next subsequence at [i_Right]

      // initialise the leading portion of subsequence [0..i_Right-1]
      // for permutation stepping:
      //   [0] = -1 ; [1] .. [i_Right-1] = 0; [i_Right] .. [nItems-1]
      m_iaItem[0] = -1;
      for (i8 i_Middle = 1; i_Middle < i_Right; i_Middle++)
        m_iaItem[i_Middle] = 0;
      break;
    }

    // have we exhausted all possible sub-sequences?
    if (i_Right >= m_nItems)
      return false;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_nPermutations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vPrint(Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  vTrace("%s: %lld items: "_Stz,szLabel,m_nItems);
  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++)
    vTrace("%s%lld"_Stz, i_Item ? "," : "", m_iaItem[i_Item]);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool PermGen::bIsValidPermutation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  for (i8 i_Item = 0; i_Item < m_nItems - 1; i_Item++) {
    i8 iItem = m_iaItem[i_Item];
    for (i8 i_Right = i_Item + 1; i_Right < m_nItems; i_Right++)
      if (iItem == m_iaItem[i_Right])
        return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
