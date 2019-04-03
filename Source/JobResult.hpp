/*============================================================================*/
/* JobResult.hpp -- Job Result                                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobResult
  {
  public:
    s4K          m_sMetadata;    // meta data of job
    
    //! this should go
    bool         m_bHasError;    // has an error of one or more nodes
    
    Response     m_xResponse;    // response of this job
    ResponseVec* m_pResponseVec; // vector of responses from all the nodes
    
    struct {
      s4K  m_sResponse;          // response string
      Atomic m_ixRowsProcessed;  // rows processed
      Atomic m_ixRowsInvalid;    // invalid rows detected
      Atomic m_ixRowsSkipped;    // rows skipped
      eEC  m_eErrorCode;         // error code
    } m_xNode[C_MAX_NODES];
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    JobResult(void);
    ~JobResult(void) noexcept(false);
    
    void vPrintResult(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
