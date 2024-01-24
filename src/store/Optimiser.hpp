/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"
#include "CardCtl.hpp"
#include "PermGen.hpp"
#include "Error.hpp"
#include "PipelineDef.hpp"
#include "ExtendedText.hpp"

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

    i8           m_nJoinPermutations;
    String_t<512> m_sJoinPermutation[C_PERMUTATIONS]; // join permutation string

    String_t<512> m_sJoinOrder; // join order string

    CardIter*     m_pCI;
    PipelineDef*  m_pPipelineDef;
    ExtendedText* m_pOptStmt;
    ExtendedText* m_pCodeBlock;


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(TagSet* p_JobTags);
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    bool bApplyTransformations(Error& rxError);

    Bench* pBench(void);

    CardCtl* pCardCtl(void);

    ExtendedText* pCodeBlock(void);
    void vCodeBlock(ExtendedText* pCodeBlock);

    void vDissect(VertexSpec* pm_pUnOptPipeSpec);

    TagSet* pJobTags(void);

    Stz* szJoinOrder(void);
    void vJoinOrder(Stz* szJoinOrder);

    Stz* szJoinPermutation(i8 iJoinPermutation);
    void vJoinPermutation(i8 iJoinPermutation, Stz* szJoinOrder);
    i8 nJoinPermutations(void);
    void vJoinPermutations(i8 iJoinPermutations);

    ExtendedText* pOptStmt(void);
    void vOptStmt(ExtendedText* pOptSmt);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

    bool bTransform(Error& rxError, Pds* pPdsRowBuffer);

    PipelineDef* pPipelineDef(void);
    void vPipelineDef(PipelineDef* pPipelineDef);

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
