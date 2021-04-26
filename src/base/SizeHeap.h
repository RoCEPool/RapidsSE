/*============================================================================*/
/* SizeHeap.h -- Structure used by SizesHeaps                                 */
/*============================================================================*/
#pragma once

typedef void (fpPOSTFN)(void* aObject);

struct SizeHeap {
  
  i8  m_nClassSize;         // size of the class in bytes
  
  eHT m_eWhichHeap;         // from which heap should the instance be allocated
  
  i8  m_nVtableAdjust;      // temp: explore support for vtable
  
  b8  m_bDontMemLog;        // don't memory log?
  
  b8  m_bLowMemoryDiscard;  // discard received object when low memory?
    
  Word* m_pPreSendWord;     // word to execute prior to sending the object
  
  fpPOSTFN* m_fpPostRecvFn; // post receive cpp function

  Word* m_pPostRecvWord;    // word to execute after receiving the object
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
