/*============================================================================*/
/* JobDesc.hpp -- the class which populates the job catalog                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobDesc
  {
  public:
    JobDesc(
            eCN  eClass,     // enum of the class
            eJK  eKind,      // job kind
            Stz* aComment,   // comment about the job
            Stz* aCommand1,  // first word of prompt command
            Stz* aCommand2   // optional second word of prompt command
           );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
