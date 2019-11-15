/*============================================================================*/
/* Request.hpp -- Request                                                     */
/*============================================================================*/
#pragma once

namespace Sessions
{
  class Request : public Focal
  {
  friend SePrompt;
  
  private:
    ReqTuple  m_xReqTuple;
  
    Session*  m_p_Session;
    TokenVec* m_p_TknVec;
  
   // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* aAddr);
    
    //Request(Session* p_Session, TokenVec* p_TknVec);
    Request(Session* p_Session, Stz* szPurpose, i8 iClassSeq);
   ~Request(void) noexcept(false);
  
    ReqTuple* aReqTuple(void);

  private:
    Footer* aFooter(void);
  };
  
  Request* _pRequest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
