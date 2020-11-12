/*============================================================================*/
/* Node_I.hpp - Node Iterator                                                 */
/*============================================================================*/
#pragma once

namespace Grids
{
  class Node_I : public Object
  {
  private:
  
    i8 m_i_Node;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Node_I(void);
    Node_I(i8 i_Node);
   ~Node_I(void) noexcept(false);
    
    
    operator Node*(void);   // auto conversion
    Node* operator *(void); // dereference

    bool bCont(void);
    void vNext(void);
    i8   i_Node(void);

  private:
    Footer* aFooter(void);
  };
  
  Node_I* _pNode_I(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
