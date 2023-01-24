/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"
#include "CardCtl.hpp"
#include "Transformer.hpp"
#include "Weaver.hpp"

namespace STORE
{
  class Optimiser : public Object
  {
  private:
    s32          m_sQueryNm;

    TagSet*      m_p_JobTags;

    VertexSpec*  m_pUnOptPipeSpec;

    CardCtl*     m_pCardCtl;
  
    Bench*       m_pBench;
    Dissector*   m_pDissector;
    Transformer* m_pTransformer;
    Weaver*      m_pWeaver;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(TagSet* p_JobTags);
  
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    Bench* pBench(void);

    CardCtl* pCardCtl(void);
  
    void vDissect(VertexSpec* pm_pUnOptPipeSpec);

    TagSet* pJobTags(void);

    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

    bool bTransform(eEC* peErrorCode, InfoSet* pInfoSet);
    Transformer* pTransformer(void);

    void vPipeSpec(VertexSpec* pUnOptPlineSpec);
    void vPopBoard(void);
    void vPushBoard(Board* pBoard);
 
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
