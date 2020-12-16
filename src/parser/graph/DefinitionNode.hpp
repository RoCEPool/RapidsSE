/*============================================================================*/
/* DefinitionNode.hpp - A node that holds definitions of other nodes          */
/*============================================================================*/
#pragma once

namespace Parser
{
  class DefinitionNode : public PlanNode
  {
  protected:
    s256 m_sPath;  // Path of the current definition in the hierarchy

  private:
    Footer m_xFooter;
    
  public:
    // Overriding these because we inherited from ListItem_t and ListItem_t overrides
    // operator new with a fatal error that it cannot directly be newed.
    void* operator new(size_t);
    void  operator delete(void*);

    DefinitionNode(Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren, Stz* sPath);
    virtual ~DefinitionNode(void);

    Footer* aFooter(void);

  };
  
  DefinitionNode* _pDefinitionNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
