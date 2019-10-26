/*============================================================================*/
/* CRM.hpp -- counted reference manager                                       */
/*============================================================================*/
#pragma once

#include "AM.hpp"

namespace Gadgets
{
  template <class CLASS>
  class CRM
  {
  public:
  
    CLASS* m_pThis; // pointer to the object
  
    Atomic m_nxReferences; // number of references
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                        (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CRM<CLASS>(CLASS* pThis)
    {
      if ( ! pThis || ! AM::bInBufferPool(pThis) )
        THROW("CRM::CRM(pThis) : invalid pThis parameter"_Stz);
    
      m_pThis = pThis;
    }
    
    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~CRM<CLASS>(void)
    {
      RASSERT(! m_nxReferences.iVal());
    }
  
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
  
    /*------------------------------------------------------------------------*/
    /* borrow a reference to this object                                      */
    /*------------------------------------------------------------------------*/
    void vBorrowRefBy(REF<CLASS>* pBorrowingRef)
    {
       RASSERT(pBorrowingRef);
       RASSERT(! pBorrowingRef->m_pReferencedObject);

       ++ m_nxReferences;
    
       pBorrowingRef->m_pReferencedObject = (CLASS*)this;
    }
  
    /*------------------------------------------------------------------------*/
    /* return a reference to this object                                      */
    /*------------------------------------------------------------------------*/
    void vReturnRefBy(REF<CLASS>* pReturningRef)
    {
      RASSERT(pReturningRef);
      RASSERT(pReturningRef->m_pReferencedObject);

      pReturningRef->m_pReferencedObject = nullptr;
    
      -- m_nxReferences;
    }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
