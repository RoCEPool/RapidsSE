/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"
#include "CardCtl.hpp"
#include "Weaver.hpp"
#include "PermGen.hpp"
#include "Error.hpp"

namespace STORE
{
  class Optimiser : public Object
  {
  public:
    static const i8 C_PERMUTATIONS = 1024;

  private:
    s32          m_sQueryNm;

    TagSet*      m_p_JobTags;

    VertexSpec*  m_pUnOptPipeSpec;

    CardCtl*     m_pCardCtl;
  
    Bench*       m_pBench;
    Dissector*   m_pDissector;
    Weaver*      m_pWeaver;

    i8           m_nJoinPermutations;
    s256         m_sJoinPermutation[C_PERMUTATIONS]; // join permutation string

    s256         m_sJoinOrder; // join order string

    CardIter*    m_pCI;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(TagSet* p_JobTags);
  
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    bool bApplyTransformationMatrix(Error& rxError);
    void vApplyTransformationMatrix(void);

    Bench* pBench(void);

    CardCtl* pCardCtl(void);
  
    void vDissect(VertexSpec* pm_pUnOptPipeSpec);

    TagSet* pJobTags(void);

    Stz* szJoinOrder(void);
    void vJoinOrder(Stz* szJoinOrder);

    Stz* szJoinPermutation(i8 iJoinPermutation);
    void vJoinPermutation(i8 iJoinPermutation, Stz* szJoinOrder);
    i8 nJoinPermutations(void);
    void vJoinPermutations(i8 iJoinPermutations);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

    bool bTransform(Error& rxError, InfoSet* pInfoSet);

    void vPipeSpec(VertexSpec* pUnOptPlineSpec);

    void vToPipeSpec(VertexSpec* pPipeSpec);
  
    Stz* szWeave(void);

  private:


    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
