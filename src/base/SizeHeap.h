/*============================================================================*/
/* SizeHeap.h -- Structure used by SizesHeaps                                 */
/*============================================================================*/
#pragma once

typedef void (fpPOSTFN)(void* aObject);

struct SizeHeap {
  
  i8  m_nClassSize;
  
  eHT m_eWhichHeap;
  
  i8  m_nVtableAdjust;      // temp: explore support for vtable
  
  i8  m_nRecvArgs;          // number of additional objects to be received
  
  b8  m_bLowMemoryDiscard;  // discard received object when low memory?
  
  fpPOSTFN* m_fpPostRecvFn; // post receive function
  
  Word* m_pDispatchWord;    // dispatch word on receive
  
  Word* m_pReceivedWord;    // word to execute upon recv()
  
  Word* m_pPreSendWord;     // word to execute prior to sending the object

  b8  m_bDontMemLog;        // don't memory log?
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
