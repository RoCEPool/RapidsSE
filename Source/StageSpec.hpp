/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class StageSpec : public Object
  {
  private:
  
    s32 m_sName;        // stage name
    ePO m_eTopology;    // stage topoplogy
    s32 m_sLocus;       // stage locus
  
    OpSpecVec* m_pOpSpecVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(Stz* szName);
   ~StageSpec(void) noexcept(false);
  
    void vAddOpSpec(OpSpec* pOpSpec);
    void vCopyOps(StageSpec* pStageSpec);
    void vLocus(Stz* szLocus);
    Stz* szName(void);
    OpSpecVec* pOpSpecVec(void);
    void vPrependConsumeOp(ePX eTopologyX);
    void vPrependStartOp(void);
    ePO  eTopology(void);
    void vTopology(ePO eTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
