/*============================================================================*/
/* NodeCursor.hpp -- Cursor over Nodes in Grid                                */
/*============================================================================*/
#pragma once

namespace GridModel
{
  class NodeCursor : public Object,
                     public ListCursor_t<Node>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, NodeCursor* pNodeCursor);
    void  operator delete(void*);
    
    NodeCursor(List_t<Node>* pList);
    ~NodeCursor(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void vInit(void);
    
    Footer* aFooter(void);
  };
  
  NodeCursor* _pNodeCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
