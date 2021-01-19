/*============================================================================*/
/* MY.hpp -- My helper                                                        */
/*============================================================================*/
#pragma once

//!                    ** Do not include BASE.h **

namespace STORE
{
  class Job;
  class Session;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace OBLIQUE
{
  class MY
  {
  public:
    static Stz* szJobNm(void);
    static Stz* szGridNm(void);
    static Stz* szWordNm(void);
  
    static Job* pJob(void);
    static Session* pSession(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
