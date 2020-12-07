/*============================================================================*/
/* Result.hpp -- Result                                                       */
/*============================================================================*/
#pragma once

namespace Jobs
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
    
    // buffer queue
    Queue*       m_pOriginBufferQueue; // queue in originating node to receive result buffers
    
    StreamDef*   m_pStreamDef;
    bool         m_bExitMarker; 

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Result(void);
   ~Result(void) noexcept(false);

    void vPrint(eRS eResponse);
    
  public:
    void vPrintDist2pcResponse(void);
    void vPrintLocalResponse(void);
    void vProcessBufferQueue(void);
    bool bProcessOperand(Operand* pOperand);
    
    //! TO GO
    void vPrintOldJobResponses(void);
    
    Footer* aFooter(void);
  };
  
  Result* _pResult(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
