/*============================================================================*/
/* JobResult.hpp -- Job Result                                                */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobResult
  {
  public:
    bool   m_bResult;     // final result of job
    eEC    m_eErrorCode;  // error code on originating node e.g EC_INVALID_COMMAND
    struct {
      s256 m_sResponse;   // response string
      eEC  m_eErrorCode;  // error code
    } m_xNode[C_MAX_NODES];
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    JobResult(void);
    ~JobResult(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
