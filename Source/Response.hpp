/*============================================================================*/
/* Response.hpp -- Response                                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Response : public Object
  {

  public:
    i8     m_iNode;           // node this response came from
    s4K    m_sResponse;       // response string
    Atomic m_ixRowsProcessed; // rows processed
    Atomic m_ixRowsInvalid;   // invalid rows detected
    Atomic m_ixRowsSkipped;   // rows skipped
    eEC    m_eErrorCode;      // error code

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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
