/*============================================================================*/
/* Action.hpp - when did an action occur and what happened                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class Action : public Object
  {
  friend class ActionLog;
  
  private:
    i8    m_i_Entry;
    i8    m_i_Root;
    i8    m_iTick;
  
    void* m_aJob;
    i8    m_iJobId;
    eCN   m_eClassNo;
    i8    m_iParentId;
    bool  m_bWasRecv;
    i8    m_i_Activity;

    void* m_aJob_Arg;
    i8    m_iJobId_Arg;
    eCN   m_eClassNo_Arg;
    i8    m_iParentId_Arg;
    bool  m_bWasRecv_Arg;
    i8    m_i_Activity_Arg;

    eAN   m_eAction;
    i8    m_iArg0;
    i8    m_iArg1;
  
    char  m_caName[32];
    char  m_caText[128];

    Link_t<i8> m_wLink;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Action();
    ~Action() noexcept(false);
  
    i8   iDeltaNextUsec(void);
    void vInit(i8 i_Action);
    bool bIsRoot(void);
    Action* pNext(void);
    void vReset(void);
    void vWrite(File* pFile);
    void vWriteJob(File* pFile);
    void vWriteLead(File* pFile);
    void vWriteRefer(File* pFile);
    void vWriteSysJob(File* pFile);

    void vWrite_Default(File* pFile);
    void vWrite_Penum(File* pFile);
    void vWrite_P(File* pFile);
    void vWrite_Pi8(File* pFile);
    void vWrite_Pjob(File* pFile);
    void vWrite_Proot(File* pFile);
    void vWrite_Psysjob(File* pFile);
    void vWrite_Psysjob_enum(File* pFile);
    void vWrite_Psysjob_i8(File* pFile);
    void vWrite_Ptxt(File* pFile);
    void vWrite_Pwrd(File* pFile);

    Footer* aFooter(void);
  };
  
  Action* _pAction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
