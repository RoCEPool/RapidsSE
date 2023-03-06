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
  for (i8 i_Position = 0; i_Position < C_PERMGEN_ELEMENTS; i_Position++)
    m_iaElements[i_Position] = C_UNKNOWN;

  m_pIntSet = new IntSet(nItems);

  _checkInitialised_(CN_PERMGEN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PermGen::~PermGen(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  delete m_pIntSet;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



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
i8 PermGen::iElement(i8 i_Position)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_iaElements[i_Position];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nElements(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_nElements;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nPermutations(i8 nElements)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return IntLib::iFactorial(nElements);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
