/*============================================================================*/
/* GraphEditor.hpp - a utility for editing plan graphs                        */
/*============================================================================*/
#pragma once

namespace Parser
{
  class GraphEditor : public Object
  {
  private:
    Footer m_xFooter;

  public:

    void* operator new(size_t);
    void  operator delete(void*);

    GraphEditor();
    ~GraphEditor(void);

    Footer* aFooter(void);

    // Create a new copy of the plan graph using the template
    static PlanNode* pNewFromTemplate(PlanNode* pRootNode);
  };

  GraphEditor* _pGraphEditor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
