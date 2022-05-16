/*============================================================================*/
/* SizeHeap.h -- Structure used by SizesHeaps                                 */
/*============================================================================*/
#pragma once

typedef void (fpPOSTFN)(void* aObject);

struct SizeHeap
{
  eCN m_eClassType;         // 00 class type

  i8  m_nClassSize;         // 01 size of the class in bytes
  
  eHT m_eWhichHeap;         // 02 from which heap should the instance be allocated
  
  i8  m_nVtableAdjust;      // 03 temp: explore support for vtable
  
  b8  m_bDontMemLog;        // 04 don't memory log?

  b8  m_bIgnoreMemoryLeak;  // 05 ignore during memory leak tracking ?

  b8  m_bLowMemoryDiscard;  // 06 discard received object when low memory?
    
  Word* m_pPreSendWord;     // 07 word to execute prior to sending the object
  
  fpPOSTFN* m_fpPostRecvFn; // 08 post receive cpp function

  Word* m_pPostRecvWord;    // 09 word to execute after receiving the object
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
