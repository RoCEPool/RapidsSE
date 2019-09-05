/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage : public Object
  {
  private:
  
    s32    m_sName;   // name of the stage
    OpVec  m_oOpVec;  // set of operators

    // must be last
    Footer m_xFooter;

  public:
    Stage(Stz* szName);
   ~Stage(void) noexcept(false);
  
    Stz* szName(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
