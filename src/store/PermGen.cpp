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

  m_pElementSeq = new IntSet(nItems);
  m_pInvalidSeq = new IntSet(nItems);

  _checkInitialised_(CN_PERMGEN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PermGen::~PermGen(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  delete m_pElementSeq;
  delete m_pInvalidSeq;
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

  i8 m_nElements = m_pElementSeq->nInts();

  m_pElementSeq->vAscending();

  for (i8 i_Position = 0; i_Position < m_nElements - 1; i_Position++) {
    i8 nTimesPositionRepeats = IntLib::iFactorial(m_nElements - i_Position - 1);
    i8 i_NextElementOfPerm = (i_Permutation / nTimesPositionRepeats)
                               % (m_nElements - i_Position);

    m_pElementSeq->vRotateToEnd(i_NextElementOfPerm);
  }

  m_pElementSeq->vRotateToEnd(0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::iElement(i8 i_Position)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_pElementSeq->iInt(i_Position);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nElements(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return m_pElementSeq->nInts();
}

/*----------------------------------------------------------------------------*/
/* get the next val;id permutation                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::iNextValid(i8 i_Position, i8 i_StartingAt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  m_pInvalidSeq->vCopy(m_pElementSeq);

  i8 nMaxPermutations = nPermutations();
  i8 i_TestPermutation = i_StartingAt + 1;

  for (/*none*/; i_TestPermutation < nMaxPermutations; i_TestPermutation++) {

    vCalcPermutation(i_TestPermutation);

    i8 nLeading = i_Position + 1;
    if (! m_pElementSeq->bSame(m_pInvalidSeq,nLeading))
      return i_TestPermutation;

    //vTrace("Permutation %lld is not valid\n"_Stz,i_TestPermutation);
  }

  return nMaxPermutations;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return nPermutations(m_pElementSeq->nInts());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 PermGen::nPermutations(i8 nElements)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERMGEN,_FOOTER_);

  return IntLib::iFactorial(nElements);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
