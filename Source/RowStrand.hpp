/*============================================================================*/
/* RowStrand.hpp -- Row Strand                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowStrand : public Object
  {
  private:
    static const i8 C_STRAND_LENGTH = 16;
    
    Lock       m_zLock;        // lock on this strand (all buffers)
    i8         m_iRowBuffers;  // number of row buffers
    RowBuffer* m_pFirstBuffer; // first buffer in list
    RowBuffer* m_pLastBuffer;  // last buffer in list
    
    RowStrand* m_pPrevStrand; // prev strand
    RowStrand* m_pNextStrand; // next strand
    

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowStrand(void);
    ~RowStrand(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    bool bInsertRow(TxRef* pTxRef, StrVal* pRow);
    i8   iRowBuffers(void);
    
    RowBuffer* pFirstBuffer(void);
    RowBuffer* pLastBuffer(void);
    
    RowStrand* pNextStrand(void);
    void       vNextStrand(RowStrand* pNextStrand);
    
    RowStrand* pPrevStrand(void);
    void       vPrevStrand(RowStrand* pPrevStrand);
    
    void vTruncate(void);
    
  private:
    bool    bExtendStrand(void);
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/