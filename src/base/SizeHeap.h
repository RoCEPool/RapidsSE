/*============================================================================*/
/* SizeHeap.h -- Structure used by SizesHeaps                                 */
/*============================================================================*/
#pragma once

#include "eHT.h"

typedef void (fpPOSTFN)(void* aObject);

struct SizeHeap {
  
  i8  m_nClassSize;
  
  eHT m_eWhichHeap;
  
  i8  m_nVtableAdjust; // temp: explore support for vtable
  
  b8  m__Test0;
  
  fpPOSTFN* m_fpPostRecvFn; // post receive function
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
