/*============================================================================*/
/* LogicUnit_GenCode.hpp -- Logic Unit (generate code functions)              */
/*============================================================================*/

#include "BASE.h"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "TagConstants.h"
#include "Grid.hpp"
#include "Node.hpp"

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAcceptToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ++ m_i_Tkn;
  
  m_pTkn = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_LOGICUNIT
  //vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vBeginToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_LOGICUNIT
  //vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCollectOpcode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  //vTrace("Tkn: %s\n"_Stz, m_pTkn->azChars());
  
  if (m_pTkn->bSame("Label"_Stz)) {
    vAcceptToken();
    //vTrace("Tkn: %s\n"_Stz, m_pTkn->azChars());
    vDefineLabel(m_pTkn->szChars());
    return;
  }
  
  if (m_pTkn->bSame("DeleteLabel"_Stz)) {
    vAcceptToken();
    vDeleteLabel(m_pTkn->szChars());
    return;
  }

  
  TagSet* pJobTags = AM::pPipelineJob()->pJobTags();

//  for (i8 i_OpCode = 0; i_OpCode < EO_ENUMS; i_OpCode++) {
//    vTrace("%lld: %s %s\n"_Stz, i_OpCode, aEO[i_OpCode], aEA[EoArgs[i_OpCode].m_eArgument]);
//  }

  for (i8 i_OpCode = 0; i_OpCode < EO_ENUMS; i_OpCode++) {

    //vTrace("Opcode: %s; Arg: %s\n"_Stz, aEO[i_OpCode], aEA[EoArgs[i_OpCode].m_eArgument]);

    if (m_pTkn->bSame(aEO[i_OpCode])) {

      //vTrace("Tkn: %s %s\n"_Stz, m_pTkn->azChars(), aEO[i_OpCode]);

      // debug
      char* azEname;
      
      switch ((i8)(EoArgs[i_OpCode].m_eArgument)) {
          
        case EA_DATE:
          vAcceptToken();
          azEname = m_pTkn->azChars();
          if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
            i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
            ASSERT(i_Ename != C_UNKNOWN);
            if (pJobTags->eToken(i_Ename) != TN_STRING) {
              pJobTags->vDumpSet(true);
              vTrace("azEname : %s \n"_Stz,azEname);
              THROW_USER("vCollectOpcode"_Stz);
            }
            Stz* szParam = pJobTags->szValue(i_Ename);
            vAddCodeDte((eEO)i_OpCode, szParam);
          }
          else
            vAddCodeDte((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_INT: {
          i8 iArg = C_UNKNOWN;
      
          vAcceptToken();
          if ( m_pTkn->bIsInteger() ) {
            iArg = m_pTkn->iToI8();
          }
          else {
            if (0 == StrLib::iCompare(m_pTkn->azChars(),"CT_", 3,  C_CASE)) {
              for(iArg = 0; iArg < CT_ENUMS; iArg++) {
                if (StrLib::bEqualTo(m_pTkn->azChars()+3,(char*)aCT[iArg]))
                  break;
              }
            }
            else if (0 == StrLib::iCompare(m_pTkn->azChars(),"ZK_", 3,  C_CASE)) {
              for(iArg = 0; iArg < ZK_ENUMS; iArg++) {
                if (StrLib::bEqualTo(m_pTkn->azChars(),(char*)aZK[iArg]))
                  break;
              }
            }
            else if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
              i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
              ASSERT(i_Ename != C_UNKNOWN);
            
              //pJobTags->vContentDump();
              if (pJobTags->eToken(i_Ename) == TN_INTEGER)
                iArg = pJobTags->iValue(i_Ename);
              else if (pJobTags->eToken(i_Ename) == TN_STRING) {
                Stz* szParam = pJobTags->szValue(i_Ename);
                iArg = StrLib::iToI8((char*)szParam);
              }
              else
                THROW_USER("vCollectOpcode"_Stz);

            }
            else {

              vTrace("\n*** Invalid integer argument to opCode %s ***\n"_Stz, m_pTkn->szChars());
            }
          }
          ASSERT(iArg != C_UNKNOWN);
          vAddCodeArg((eEO)i_OpCode, iArg);
          return;
        }
          
        case EA_LABEL:
          vAcceptToken();
          vAddCodeLabel((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_STRING:
          vAcceptToken();
          if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
            i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
            ASSERT(i_Ename != C_UNKNOWN);
            if (pJobTags->eToken(i_Ename) != TN_STRING)
              THROW_USER("vCollectOpcode"_Stz);
            Stz* szParam = pJobTags->szValue(i_Ename);
            vAddCodeString((eEO)i_OpCode, szParam);
          }
          else
            vAddCodeString((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_SUBSCR:
          vAcceptToken();
          vAddCodeArg((eEO)i_OpCode, m_pTkn->iToI8());
          return;

        case EA_TIME:
          vAcceptToken();
          azEname = m_pTkn->azChars();
          if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
            i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
            ASSERT(i_Ename != C_UNKNOWN);
            if (pJobTags->eToken(i_Ename) != TN_STRING) {
              pJobTags->vDumpSet(true);
              vTrace("azEname : %s \n"_Stz,azEname);
              THROW_USER("vCollectOpcode"_Stz);
            }
            Stz* szParam = pJobTags->szValue(i_Ename);
            vAddCodeTme((eEO)i_OpCode, szParam);
          }
          else
            vAddCodeTme((eEO)i_OpCode, m_pTkn->szChars());
          return;

        case EA_TIMESTAMP:
          vAcceptToken();
          azEname = m_pTkn->azChars();
          if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
            i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
            ASSERT(i_Ename != C_UNKNOWN);
            if (pJobTags->eToken(i_Ename) != TN_STRING) {
              pJobTags->vDumpSet(true);
              vTrace("azEname : %s \n"_Stz,azEname);
              THROW_USER("vCollectOpcode"_Stz);
            }
            Stz* szParam = pJobTags->szValue(i_Ename);
            vAddCodeTsp((eEO)i_OpCode, szParam);
          }
          else
            vAddCodeTsp((eEO)i_OpCode, m_pTkn->szChars());
          return;

       case EO_NONE:
         vAddCode((eEO)i_OpCode);
         return;
          
        case EO_ENUMS:
          ASSERT(0);
      }
    }
    
  }
  
  vTrace("\n*** Invalid opCode %s ***\n"_Stz, m_pTkn->szChars());
  
  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
