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
    s4K    m_sMetadata;      // meta data of job
    bool   m_bHasError;      // has an error of one or more nodes

    struct {
      s4K  m_sResponse;      // response string
      i8   m_iRowsProcessed; // rows processed
      eEC  m_eErrorCode;     // error code
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
