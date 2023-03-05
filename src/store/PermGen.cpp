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

  for (i8 i_Rotate = 0; i_Rotate < C_PERMGEN_ITEMS; i_Rotate++)
    m_naRotated[i_Rotate] = -1;

  for (i8 i_Position = 0; i_Position < C_PERMGEN_ITEMS; i_Position++)
    m_iaSample[i_Position] = i_Position;

  m_i_Level = -1;

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
void PermGen::vTest_Sample(i8 nItems)
{
  _static_

  PermGen* pPermGen = new PermGen(nItems);
  //pPermGen->vInitSample();

  i8 i_Permutation = 0;
  for (; pPermGen->bNextPermutation(); i_Permutation++) {

    vTrace("%ll3d): "_Stz, i_Permutation);

    for (i8 i_Item = 0; i_Item < nItems; i_Item++)
      vTrace ("%s%lld"_Stz,
              !i_Item?" ":", ",pPermGen->iItem(i_Item));

    pPermGen->vCalcSample(i_Permutation);
    vTrace(" :: "_Stz);
    for (i8 i_Position = 0; i_Position < nItems; i_Position++)
      vTrace ("%s%lld"_Stz,
              !i_Position ? " " : ", ", pPermGen->iSample(i_Position));

    vTrace("\n"_Stz);
  }

  vTrace("%lld permutations in a set of %lld items\n"_Stz, i_Permutation, nItems);
  delete pPermGen;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vTest_Rotation(i8 nItems)
{
  _static_

  PermGen* pPermGen = new PermGen(nItems);

  pPermGen->vInitRotations();

  i8 nPermutations = 0;
  while (pPermGen->bNextRotation())
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
void PermGen::vInitRotations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++)
    m_iaItem[i_Item] = i_Item;

  for (i8 i_Rotate = 0; i_Rotate < m_nItems - 1; i_Rotate++)
    m_naRotated[i_Rotate] = 0;

  m_i_Level = m_nItems - 2;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vInitSample(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++)
    m_iaItem[i_Item] = i_Item;

//  for (i8 i_Rotate = 0; i_Rotate < m_nItems - 1; i_Rotate++)
//    m_naRotated[i_Rotate] = 0;
//
//  m_i_Level = m_nItems - 2;
}


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
bool PermGen::bNextRotation(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PERMGEN, _FOOTER_);

//  return false;

  if (! m_nPermutations) {
    m_nPermutations++;
    vPrint_rot("First"_Stz);
    return true;
  }

  while (true) {

    i8 nMaxRotatesAtLevel = m_nItems - m_i_Level - 1;
    if (++m_naRotated[m_i_Level] <= nMaxRotatesAtLevel) {

      IntLib::vRotate(m_iaItem, m_nItems, m_i_Level);
      //vTrace("nMaxRotatesAtLevel[%lld] %lld\n"_Stz,m_i_Level,nMaxRotatesAtLevel);
      //vPrint_rot("bNextRotation"_Stz);
      m_nPermutations++;
      vPrint_rot("SameL"_Stz);
      return true;
    }

    vPrint_rot("\nStart"_Stz);

    while (true) {

//      IntLib::vRotate(m_iaItem, m_nItems, m_i_Level);
//      ++m_naRotated[m_i_Level];

      if (--m_i_Level < 0)
        return false;

      i8 nMaxRotatesAtLevel = m_nItems - m_i_Level - 1;
      //vTrace("nMaxRotatesAtLevel[%lld] %lld\n"_Stz,m_i_Level,nMaxRotatesAtLevel);
      if (++m_naRotated[m_i_Level] > nMaxRotatesAtLevel) {
        continue;
      }

      IntLib::vRotate(m_iaItem, m_nItems, m_i_Level);
      for (i8 i_reset = m_i_Level+1; i_reset < m_nItems; i_reset++)
        m_naRotated[i_reset] = 0;

      m_i_Level = m_nItems - 2;

      m_nPermutations++;
      vPrint_rot("LeftL"_Stz);

      return true;
    }
  }

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vPrint_rot(Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  s32 sRot;

  vTrace("[%lld / %lld items] %s: "_Stz,
         m_nPermutations,m_nItems,szLabel);

  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++) {

    if (i_Item < m_nItems - 1)
      sRot.vSprint("r%lld%s"_Stz,
                   m_naRotated[i_Item],
                   i_Item == m_i_Level ? "~" : " "
                   );
    else
      sRot = "##"_Stz;

    vTrace("%s(%si%lld)"_Stz,
             i_Item ? " " : "",
             sRot.szStz(),
             m_iaItem[i_Item]
    );
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::iSample(i8 i_Position)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_iaSample[i_Position];
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PermGen::vCalcSample(i8 i_Permutation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

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

  i8 iSample0 = (i_Permutation / n0Rep) % i_Col0Mod;
  i8 iSample1 = (i_Permutation / n1Rep) % i_Col1Mod;
  i8 iSample2 = (i_Permutation / n2Rep) % i_Col2Mod;
  i8 iSample3 = (i_Permutation / n3Rep) % i_Col3Mod;
  i8 iSample4 = ! iSample3;

  m_iaSample[0] = iSample0;
  m_iaSample[1] = iSample1;
  m_iaSample[2] = iSample2;
  m_iaSample[3] = iSample3;
  m_iaSample[4] = iSample4;

}


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
