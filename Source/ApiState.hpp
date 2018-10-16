/*============================================================================*/
/* ApiState.hpp - Api State                                                   */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class ApiState : public Object
  {
  public:
  
    struct {
      
      Session* m_pSession;
      
      i8       m_iNextFree;
      
    } xState[C_MAX_SESSIONS];
  
    i8 m_iFirstFree;
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ApiState(void);
    ~ApiState(void);
  
    i8   iAllocState(Session* pSession);
    void vFreeState(i8 i_State);

    void vDump(void);
  
    Footer* aFooter(void);
  };
  
  ApiState* _pApiState(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
