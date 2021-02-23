/*============================================================================*/
/* Result.hpp -- Result                                                       */
/*============================================================================*/
#pragma once

#include "eOM.h"
#include "eRS.h"

#include "Definition.hpp"
#include "StreamDef.hpp"

namespace STORE
{
  class Result : public Object
  {

  public:
    // this job
     Response     m_xJobResponse;       // response of this job
    
    // participant responses
    bool         m_bPartHasError;      // participant has an error
    ResponseVec* m_pOriginResponseVec; // vector of responses from all participant nodes on origin
        
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Result(void);
   ~Result(void) noexcept(false);

    
  public:
  
    static void vDecode(Operand* pOperand, eCN& eClassNo, eOM& eOpMarker);
        
    Footer* aFooter(void);
  };
  
  Result* _pResult(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
