/*============================================================================*/
/* JobResult.hpp -- Job Result                                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobResult
  {
  public:
    s4K          m_sMetadata;    // meta data of job
    bool         m_bHasError;    // has an error from one or more nodes
    Response     m_xResponse;    // response of this job
    ResponseVec* m_pResponseVec; // vector of responses from all the nodes
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    JobResult(void);
   ~JobResult(void) noexcept(false);
    
    void vAddError(eEC eError);
    
    void vPrintLoadRows(void);
    void vPrintLocalErrors(void);
    void vPrintLocalRows(void);
    void vPrintLocalZero(void);
    void vPrintNodeErrors(void);
    void vPrintNodeRows(void);
    void vPrintNodeZero(void);
  
    Stz* szResponse(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
