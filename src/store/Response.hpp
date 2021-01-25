/*============================================================================*/
/* Response.hpp -- Response                                                   */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

namespace STORE
{
  class Response : public Object
  {

  public:
    i8     m_i_Node;          // node this response came from
    s4K    m_sResponse;       // response string
    eEC    m_eErrorCode;      // error code

    bool   m_bJobHasError;    // this job has an error

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Response(void);
   ~Response(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  Response* _pResponse(void* aAddr);


  typedef Vector_t<CN_RESPONSEVEC,Response,C_MAX_NODES> ResponseVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
