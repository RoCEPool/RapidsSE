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
    ProjectOp(Stz* szOpLabel, Digest* pDigest);
   ~ProjectOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ProjectOp* _pProjectOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
