/*============================================================================*/
/* IntBuffer.hpp -- Integer Buffer                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  const i8 ELEMENTS = 100; //(C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(i8);
  
  class IntBuffer : public DataBuffer
  {

  public:
    i8 m_nInts;
    i8 m_iIntArray[ELEMENTS];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    IntBuffer(void);
    ~IntBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    ////////////////////////////////////////////////////////////////////////////
    void       vAppendValue(i8 i_Value);
    i8         nCount(void);
    void       vDump(void);
    void       vBufferCount(i8& nBuffers);
    i8         iGetValue(i8 iIndex);
    i8*        aGetElement(i8 iIndex);
    bool       bHasSpace(void);
    IntBuffer* pNextBuffer(void);
    void       vNextBuffer(IntBuffer* pNextBuffer);
    IntBuffer* pPrevBuffer(void);
    void       vPrevBuffer(IntBuffer* pPrevBuffer);
    void       vShow(void);
    bool       bVerifyOrder(void);
    
    ////////////////////////////////////////////////////////////////////////////

    public:
    Footer* aFooter(void);
  
  };
  
  IntBuffer* _pIntBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
