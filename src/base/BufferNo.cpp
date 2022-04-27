/*============================================================================*/
/* BufferNo.cpp -- a cacheline                                               */
/*============================================================================*/

#include "BASE.h"

extern BufferPool*   gpBufferPool;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferNo::BufferNo(void)
{
  m_iBufferNo = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferNo::~BufferNo(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert buffer no to address of linked buffer                              */
/*----------------------------------------------------------------------------*/
Object* BufferNo::pToAllocatedObject(i8 iBufferNo)
{
  HeapBuffer* pHeapBuffer = pToHeapBuffer(iBufferNo);

  Object* pAllocatedObject =
    reinterpret_cast<Object*>(pHeapBuffer->aFirstChunk());

  return pAllocatedObject;
}

/*----------------------------------------------------------------------------*/
/* convert address of linked buffer to buffer no                              */
/*----------------------------------------------------------------------------*/
i8 BufferNo::iToBufferNo(Object* pObject)
{
  i8 iBufferNo = PoolBuffer::iWithinPoolBuffer(pObject);
  
  if (C_UNKNOWN != iBufferNo)
    vDebugPoint();
  
  return iBufferNo;
}

/*----------------------------------------------------------------------------*/
/* convert buffer no to address of linked buffer                              */
/*----------------------------------------------------------------------------*/
HeapBuffer* BufferNo::pToHeapBuffer(i8 iBufferNo)
{
  PoolBuffer* pPoolBuffer = FileMap::pToPoolBuffer(iBufferNo);
  
  HeapBuffer* pHeapBuffer = reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  return pHeapBuffer;
}

/*----------------------------------------------------------------------------*/
/* convert buffer no to address of linked buffer                              */
/*----------------------------------------------------------------------------*/
LinkedBuffer* BufferNo::pToLinkedBuffer(i8 iBufferNo)
{
  Object* pAllocatedObject = pToAllocatedObject(iBufferNo);

  LinkedBuffer* pLinkedBuffer =
    reinterpret_cast<LinkedBuffer*>(pAllocatedObject);

  return pLinkedBuffer;
}

/*----------------------------------------------------------------------------*/
/* convert buffer no to address of linked buffer                              */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferNo::pToPoolBuffer(i8 iBufferNo)
{
  PoolBuffer* pPoolBuffer = FileMap::pToPoolBuffer(iBufferNo);
    
  return pPoolBuffer;
}

/*----------------------------------------------------------------------------*/
/* get C_UNKNOWN buffer no                                                    */
/*----------------------------------------------------------------------------*/
BufferNo BufferNo::tUNKNOWN(void)
{
  BufferNo tBufferNo;
  
  tBufferNo = C_UNKNOWN;

  return tBufferNo;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign buffer number                                                       */
/*----------------------------------------------------------------------------*/
void BufferNo::operator =(i8 iBufferNo)
{
  m_iBufferNo = iBufferNo;
}

/*----------------------------------------------------------------------------*/
/* assign buffer number                                                       */
/*----------------------------------------------------------------------------*/
void BufferNo::operator =(Object* pObject)
{
  if (pObject)
    m_iBufferNo = iToBufferNo(pObject);
  else
    m_iBufferNo = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get buffer number                                                          */
/*----------------------------------------------------------------------------*/
i8 BufferNo::iGetBuffer(void)
{
  return m_iBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get buffer address                                                         */
/*----------------------------------------------------------------------------*/
LinkedBuffer* BufferNo::pGetLinkedBuffer(void)
{
  if (C_UNKNOWN == m_iBufferNo)
    return nullptr;
  
  LinkedBuffer* pLinkedBuffer = BufferNo::pToLinkedBuffer(m_iBufferNo);
  
  return pLinkedBuffer;
}

/*----------------------------------------------------------------------------*/
/* get buffer address                                                         */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferNo::pGetPoolBuffer(void)
{
  if (C_UNKNOWN == m_iBufferNo)
    return nullptr;
  
  PoolBuffer* pPoolBuffer = FileMap::pToPoolBuffer(m_iBufferNo);
  
  return pPoolBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
