/*============================================================================*/
/* ObjBuffer.hpp -- Ent Buffer                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ObjBuffer : public DataBuffer,
                    public MiniHeap_t<Object>
  {
  protected:
 
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    ObjBuffer(void);
   ~ObjBuffer(void) noexcept(false);


    public:
    Footer* aFooter(void);
  };
  
  ObjBuffer* _pObjBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
