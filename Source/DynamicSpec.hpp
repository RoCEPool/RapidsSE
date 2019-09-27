/*============================================================================*/
/* DynamicSpec.hpp -- Dynamic Specification                                   */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class DynamicSpec : public Object
  {
  private:
  
    s32 m_sName;
  
    StageSpecVec* m_pStageSpecVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    DynamicSpec(Stz* szName);
   ~DynamicSpec(void) noexcept(false);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    Stz* szName(void);

    StageSpec* pFindStageSpec(Stz* szName);
    StageSpec* pStageSpec(i8 i_Stage);
    StageSpecVec* pStageSpecVec(void);

    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
