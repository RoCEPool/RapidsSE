/*============================================================================*/
/* Result.hpp -- Result                                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Result : public Object
  {

  public:
    s4K          m_sMetadata;          // meta data of result
    
    // this job
    eEC          m_eSyntaxError;       // syntax errors
    bool         m_bJobHasError;       // this job has an error
    Response     m_xJobResponse;       // response of this job
    
    // participant responses
    bool         m_bPartHasError;      // participant has an error
    ResponseVec* m_pOriginResponseVec; // vector of responses from all participant nodes on origin
    
    Queue*       m_pOriginBufferQueue; // queue in originating node to receive result buffers
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Result(void);
   ~Result(void) noexcept(false);

    void vPrint(eRS eResponse);
    
    //! ALKA : private
  public:
    void vPrintLoadRows(void);
    void vPrintLocalErrors(void);
    void vPrintLocalRows(void);
    void vPrintLocalZero(void);
    void vPrintNodeRows(void);
    void vPrintQueueRows(void);
    
    Footer* aFooter(void);
  };
  
  Result* _pResult(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
