/*============================================================================*/
/* StageSpec.hpp -- Stage Specification                                       */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class StageSpec : public Object
  {
  private:
  
    s32 m_sName;

    ePO m_eTopology;
    s32 m_sLocus;
  
    OpSpecVec* m_pOpSpecVec;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StageSpec(Stz* szName);
   ~StageSpec(void) noexcept(false);
  
    void vAddOpSpec(OpSpec* pOpSpec);
    void vLocus(Stz* szLocus);
    void vTopology(ePO eTopology);
    void vToPipeStr(Stz* szPipeStr);
  
    void vPrint(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
