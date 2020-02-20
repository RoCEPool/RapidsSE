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
    i8  m_iNode;        // stage node
    i8  m_iPartition;   // stage ptn
    i8  m_iThreads;     // threads
  
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
    Stz* szName(void);
    void vNode(i8 i_Nde);
    i8   iNode(void);
    OpSpecVec* pOpSpecVec(void);
    i8   iPartition(void);
    void vPartition(i8 i_Ptn);
    void vPrependConsumeOp(ePX eTopologyX, i8 iStage, i8 iOp);
    void vThreads(i8 iThreads);
    i8   iThreads(void);
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
