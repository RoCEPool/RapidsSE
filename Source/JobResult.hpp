/*============================================================================*/
/* JobResult.hpp -- Job Result                                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  enum eRK // response kind
  {
    /* 0 */ RK_NONE=0,
    
    /* 1 */ RK_DATA_ROWS,
    /* 2 */ RK_LOAD_ROWS,
    /* 3 */ RK_NODE_ERRORS,
    /* 4 */ RK_NODE_ROWS,
    /* 5 */ RK_ZERO_ROWS,
    
    /* 6 */ RK_ENUMS_
  };
  
  static Stz* aRK[]
  {
    /* 0 */ "NONE"_Stz,
    
    /* 1 */ "DATA_ROWS"_Stz,
    /* 2 */ "LOAD_ROWS"_Stz,
    /* 3 */ "NODE_ERRORS"_Stz,
    /* 4 */ "NODE_ROWS"_Stz,
    /* 5 */ "ZERO_ROWS"_Stz,
    
    /* 6 */ "ENUMS_"_Stz
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobResult
  {
  public:
    s4K          m_sMetadata;    // meta data of job
    
    bool         m_bHasError;    // has an error from one or more nodes
    
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
    
    void vAddError(eEC eError);
    
    void vPrintNodeErrors(void);
    void vPrintNodeRows(void);
    void vPrintLoadRows(void);
    void vPrintZeroRows(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
