/*============================================================================*/
/* SizeHeap.h -- Structure used by SizesHeaps                                 */
/*============================================================================*/
#pragma once

typedef void (fpPOSTFN)(void* aObject);
typedef bool (fpSENDFN)(Socket* pSocket, Object* pObject, SysCall& roSysCall);
typedef bool (fpRECVFN)(Socket* pSocket, Object* pObjBase, SysCall& roSysCall,
                        Object** ppObject, void* pvAuxBuf, i8 iAuxSize);

struct SizeHeap
{
  eCN m_eClassType;         // 00 class type

  i8  m_nClassSize;         // 01 size of the class in bytes
  
  eHT m_eWhichHeap;         // 02 from which heap should the instance be allocated
  
  b8  m_bDontMemLog;        // 03 don't memory log?

  b8  m_bIgnoreMemoryLeak;  // 04 ignore during memory leak tracking ?

  b8  m_bInternalErrorDiscard;  // 05 discard received object when low memory?

  b8  m_bLowMemoryDiscard;  // 06 discard received object when low memory?

  Word* m_pPreSendWord;     // 07 word to execute prior to sending the object

  fpPOSTFN* m_fpPostRecvFn; // 08 post receive cpp function

  Word* m_pPostRecvWord;    // 09 word to execute after receiving the object

  fpSENDFN* m_fpObjSendFn;  // 10 object send cpp function

  fpRECVFN* m_fpObjRecvFn;  // 11 object receive cpp function
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
