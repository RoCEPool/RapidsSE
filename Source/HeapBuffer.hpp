/*============================================================================*/
/* HeapBuffer.hpp - segment of objects                                        */
/*============================================================================*/
#pragma once

namespace Pool
{
  class HeapBuffer : public PoolBuffer
  {
  private:
    HeapBuffer* m_pNextBuffer;  // next buffer in list
    HeapBuffer* m_pPrevBuffer;  // prev buffer in list
  
    SingleWord m__PadToCacheLine[4]; // round up to cacheline

    // ..allocated object..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    HeapBuffer(void);
   ~HeapBuffer(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vAnalyse(i8 nBytes, i8 xCounts[]);
    HeapBuffer* pNextBuffer(void);
    void        vNextBuffer(HeapBuffer* pNextBuffer);
    HeapBuffer* pPrevBuffer(void);
    void        vPrevBuffer(HeapBuffer* pPrevSegment);
    
    FreeChunk*  aFirstChunk(void);
    FreeChunk*  aNextChunk(FreeChunk* aChunk, i8 nBytes);
    
    void vVerify(i8 iChunkSize);
    
  private:
    Stz* aChunkEnd(FreeChunk* pChunk, i8 nBytes);
    
  };
  
  HeapBuffer* _pHeapBuffer(void* aAddr);
}

/*----------------------------------------------------------------------------*/
/* mask given address to the object it is within                              */
/*----------------------------------------------------------------------------*/
template <class CAST>
static CAST _pMaskDownToObject(void* aEndoAddr, u8 uMask)
{
  u8 uObject = ((u8)aEndoAddr & ~uMask) + sizeof(HeapBuffer);
  
  CAST pObject = reinterpret_cast<CAST>(uObject);
  
  ASSERT(pObject->bValidMagic());
  
  return pObject;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
