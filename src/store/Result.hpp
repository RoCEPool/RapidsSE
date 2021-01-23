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
    bool         m_bJobHasError;       // this job has an error
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

    void vPrint(Session* pSession, eRS eResponse);
    
  public:
  
    static void vDecode(Operand* pOperand, eCN& eClassNo, eOM& eOpMarker);

    void vPrintDist2pcResponse(Session* pSession);
    void vPrintLocalResponse(Session* pSession);
        
    Footer* aFooter(void);
  };
  
  Result* _pResult(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
