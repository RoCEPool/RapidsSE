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

  m_nElements = nItems;
  m_nPermutations = 0;

  // initialise the sequencing array
  m_iaItem[0] = -1;
  for (i8 i_Item = 1; i_Item < C_PERMGEN_ELEMENTS; i_Item++)
    m_iaItem[i_Item] = 0;

  for (i8 i_Position = 0; i_Position < C_PERMGEN_ELEMENTS; i_Position++)
    m_iaElements[i_Position] = C_UNKNOWN;

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
  while (pPermGen->bNextPermutation()) {

    vTrace("%ll3d): "_Stz,nPermutations);
    for (i8 i_Item = 0; i_Item < nItems; i_Item++)
      vTrace ("%s%lld"_Stz,
              !i_Item?" ":", ",pPermGen->iItem(i_Item));
    vTrace("\n"_Stz);

    nPermutations++;
  }

  vTrace("%lld permutations in a set of %lld items\n"_Stz,nPermutations,nItems);
  delete pPermGen;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vTest_Permutation(i8 nItems)
{
  _static_

  PermGen* pPermGen = new PermGen(nItems);
  //pPermGen->vInitSample();

  i8 i_Permutation = 0;
  for (; pPermGen->bNextPermutation(); i_Permutation++) {

    pPermGen->vCalcPermutation(i_Permutation);

    vTrace("%ll3d): "_Stz, i_Permutation);

    for (i8 i_Item = 0; i_Item < nItems; i_Item++)
      vTrace ("%s%lld"_Stz,
              !i_Item?" ":", ",pPermGen->iItem(i_Item));

    vTrace(" :: "_Stz);
    for (i8 i_Position = 0; i_Position < nItems; i_Position++)
      vTrace ("%s%lld"_Stz,
              !i_Position ? " " : ", ", pPermGen->iElement(i_Position));

    vTrace("\n"_Stz);
  }

  vTrace("%lld permutations in a set of %lld items\n"_Stz, i_Permutation, nItems);
  delete pPermGen;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vTest_Permutation_v2(i8 nItems)
{
  _static_

  PermGen* pPermGen = new PermGen(nItems);
  //pPermGen->vInitSample();

  i8 nPermutations = IntLib::iFactorial(nItems);
  i8 i_Permutation = 0;
  for (i_Permutation = 0; i_Permutation < nPermutations; i_Permutation++) {

    pPermGen->vCalcPermutation(i_Permutation);

    vTrace("%ll3d): "_Stz, i_Permutation);

    for (i8 i_Position = 0; i_Position < nItems; i_Position++)
      vTrace ("%s%lld"_Stz,
              !i_Position ? " " : ", ", pPermGen->iElement(i_Position));

    vTrace("\n"_Stz);
  }

  vTrace("%lld permutations in a set of %lld items\n"_Stz, nPermutations, nItems);
  delete pPermGen;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vCalcPermutation(i8 i_Permutation)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMGEN, _FOOTER_);

  for (i8 i_Position = 0; i_Position < m_nElements; i_Position++) {
    i8 i_Element = i_Position;
    m_iaElements[i_Position] = i_Element;
  }

  for (i8 i_Position = 0; i_Position < m_nElements - 1; i_Position++) {
    i8 nTimesPositionRepeats = IntLib::iFactorial(m_nElements - i_Position - 1);
    i8 i_NextElementOfPerm = (i_Permutation / nTimesPositionRepeats)
                             % (m_nElements - i_Position);

    IntLib::vRotate(m_iaElements, m_nElements, i_NextElementOfPerm);
  }

  IntLib::vRotate(m_iaElements, m_nElements, 0);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::iItem(i8 i_Position)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  i8 i_Item = m_nElements - 1 - i_Position;

  return m_iaItem[i_Item];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_nElements;
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
      if (++m_iaItem[0] >= m_nElements)
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
    for (i_Right = 1; i_Right < m_nElements; i_Right++) {

      // continue looking for longer subsequence if subsequence at i_Right is exhausted
      if (++m_iaItem[i_Right] >= m_nElements)
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
    if (i_Right >= m_nElements)
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

  vTrace("%s: %lld items: "_Stz, szLabel, m_nElements);
  for (i8 i_Item = 0; i_Item < m_nElements; i_Item++)
    vTrace("%s%lld"_Stz, i_Item ? "," : "", m_iaItem[i_Item]);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::iElement(i8 i_Position)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_iaElements[i_Position];
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vCalcPermutation_showWorking(i8 i_Permutation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  i8 m_iaSequence[C_PERMGEN_ELEMENTS]; // sequencing array of items

  i8 i5Fac = IntLib::iFactorial(5); // 120;
  i8 i4Fac = IntLib::iFactorial(4); // 24;
  i8 i3Fac = IntLib::iFactorial(3); // 6;
  i8 i2Fac = IntLib::iFactorial(2); // 2;
  i8 i1Fac = IntLib::iFactorial(1); // 1;

  i8 n0Rep = i4Fac;
  i8 n1Rep = i3Fac;
  i8 n2Rep = i2Fac;
  i8 n3Rep = i1Fac;

  i8 i_Col0Mod = 5;
  i8 i_Col1Mod = 4;
  i8 i_Col2Mod = 3;
  i8 i_Col3Mod = 2;

  i8 iSequence0 = (i_Permutation / n0Rep) % i_Col0Mod;
  i8 iSequence1 = (i_Permutation / n1Rep) % i_Col1Mod;
  i8 iSequence2 = (i_Permutation / n2Rep) % i_Col2Mod;
  i8 iSequence3 = (i_Permutation / n3Rep) % i_Col3Mod;
  i8 iSequence4 = 0;

  for (i8 i_Position = 0; i_Position < m_nElements; i_Position++)
    m_iaSequence[i_Position] = i_Position;
  for (i8 i_Position = 0; i_Position < m_nElements; i_Position++)
    m_iaElements[i_Position] = i_Position;

  m_iaSequence[0] = iSequence0; IntLib::vRotate(m_iaElements, m_nElements, m_iaSequence[0]);
  m_iaSequence[1] = iSequence1; IntLib::vRotate(m_iaElements, m_nElements, m_iaSequence[1]);
  m_iaSequence[2] = iSequence2; IntLib::vRotate(m_iaElements, m_nElements, m_iaSequence[2]);
  m_iaSequence[3] = iSequence3; IntLib::vRotate(m_iaElements, m_nElements, m_iaSequence[3]);
  m_iaSequence[4] = iSequence4; IntLib::vRotate(m_iaElements, m_nElements, m_iaSequence[4]);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool PermGen::bIsValidPermutation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  for (i8 i_Item = 0; i_Item < m_nElements - 1; i_Item++) {
    i8 iItem = m_iaItem[i_Item];
    for (i8 i_Right = i_Item + 1; i_Right < m_nElements; i_Right++)
      if (iItem == m_iaItem[i_Right])
        return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
