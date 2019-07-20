/*============================================================================*/
/* ProjectOp.hpp -- Project                                                   */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class ProjectOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    ProjectOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~ProjectOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ProjectOp* _pProjectOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
