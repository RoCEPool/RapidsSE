/*============================================================================*/
/* Node_I.hpp - Node Iterator                                                 */
/*============================================================================*/
#pragma once

#include <list>
#include <iterator>

namespace STORE
{
  class Node_I : public Object
  {
  private:
  
    i8 m_iStipulate;
    i8 m_i_Node;

    std::list<Node*>::const_iterator m_itNode;
    std::list<Node*>::const_iterator m_itEndNode;
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Node_I(i8 iStipulate=0);
    Node_I(i8 i_Node,i8 iStipulate=0);
   ~Node_I(void) noexcept(false);
    
    operator Node*(void);   // auto conversion
    Node* operator *(void); // dereference

    void  vFirst(void);
    bool  bMore(void);
    void  vNext(void);
    i8    i_Node(void);
    Node* pNode(void);

  private:
    Footer* aFooter(void);
  };
  
  Node_I* _pNode_I(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
