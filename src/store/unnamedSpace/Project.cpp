/*============================================================================*/
/* Project.cpp -- Project column                                              */
/*============================================================================*/

#include "STORE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Project* Expressions::_pProject(void* aAddr)
{
  Project* pProject = reinterpret_cast<Project*>(aAddr);
  
  RASSERT_TYPE(pProject,CN_PROJECT);
  
  return pProject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Project::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_PROJECT);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Project::operator delete(void* aAddr)
{
  Project* pProject = reinterpret_cast<Project*>(aAddr);
  
  HeapMem::vFree(pProject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Project::Project(void)
: Definition(CN_PROJECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);

  m_iColumns = 0;
  m_uProjectColumns = 0;
  //m_sAsmText;

  //for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
  //  m_i_Code[i_Col] = 0;
  
  //_checkInitialised_(CN_PROJECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Project::~Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column vec from stream def                                             */
/*----------------------------------------------------------------------------*/
eEC Project::eProjectColumns(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, Stz* sProjectColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);

  // tokenise the projectColumns
  TokenVec *pTknVec = TokenVec::pTokenise(sProjectColumns);

  eEC eError = EC_NONE;
  Token* pTkn;
  i8 i_Tkn;

  // first collect all the StreamDefIn's source columns needed for projected
  eError = eCollectSourceColumns(pStreamDefIn, pTknVec);

  if (eError != EC_NONE)
    goto L_RETRUN;

  pTkn = pTknVec->pNextToken();

  // now project each destination column
  while (pTkn) {
    eError = eProjectColumn(pStreamDefIn, pStreamDefOut, pTknVec);
    if (eError != EC_NONE)
       break;

    pTkn = pTknVec->pCurrToken();

    if ( pTkn && ! pTkn->bSame(","_Stz) ) {
      eError = EC_INVALID_COMMAND;
      break;
    }
    if (pTkn)
      pTkn = pTknVec->pNextToken();
  }

  L_RETRUN:
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

///*----------------------------------------------------------------------------*/
///* add code                                                                   */
///*----------------------------------------------------------------------------*/
//void Project::vAddCode(i8 i_Column, eEO eCode)
//{
//    _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);
//
//    m_eaCode[i_Column][m_i_Code[i_Column]++] = eCode;
//}
//
///*----------------------------------------------------------------------------*/
///* add code with argument                                                     */
///*----------------------------------------------------------------------------*/
//void Project::vAddCodeArg(i8 i_Column, eEO eCode, i8 iArg)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECT,_FOOTER_);
//
//  m_eaCode[i_Column][m_i_Code[i_Column]++] = eCode;
//
//  i8* piArg = (i8*) & m_eaCode[m_i_Code[i_Column]];
//  * piArg = iArg;
//
//  m_i_Code[i_Column] += sizeof(i8);
//}

/*----------------------------------------------------------------------------*/
/* collect source columns needed for projection                               */
/*----------------------------------------------------------------------------*/
eEC Project::eCollectSourceColumns(StreamDef* pStreamDefIn, TokenVec* pTknVec) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_PROJECT, _FOOTER_);

  eEC eError = EC_NONE;
  Token* pTkn;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);
    if (pTkn->bIsName() && pTkn->bSame(","_Stz)) {
      if (m_iColumns)
        continue;
      else {
        eError = EC_INVALID_COMMAND;
        break;
      }
    }

    // alias ?
    if (pTkn->bSame("as"_Stz)) {

      // skip next token
      i_Tkn++;
      pTkn = pTknVec->pGet(i_Tkn);

      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        break;
      }

      // move to the next token
      continue;
    }

    // skip any token other than a column name
    if (!pTkn->bIsName()) {
      continue;
    }

    // look for the column in the streamDefIn
    Column *pColumn = nullptr;
    bool bFound = false;
    i8 nColumns = pStreamDefIn->nColumns(C_ALL);
    Stz* sColumnNm = pTkn->szChars();

    for (i8 i_StreamInCol = 0; i_StreamInCol < nColumns; i_StreamInCol++) {
      pColumn = pStreamDefIn->pGetColumn(i_StreamInCol);
      if (pColumn->szColNm()->bSame(sColumnNm)) {
        //bFound = true;

        // set bit on for the column in m_uProjectColumns
        BitLib::vSetOn(m_uProjectColumns, i_StreamInCol);
        m_iColumns++;
        break;
      }
    }

    //if (!bFound) {
    //  eError = EC_INVALID_COMMAND;
    //  break;
    //}

    continue;
  }

  L_RETRUN:
    return eError;
}


/*----------------------------------------------------------------------------*/
/* parse project                                                              */
/*----------------------------------------------------------------------------*/
eEC Project::eProjectColumn(StreamDef* pStreamDefIn, StreamDef* pStreamDefOut, TokenVec* pTknVec) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PROJECT, _FOOTER_);

  eEC eError = EC_NONE;

  Stz *sAliasName = ""_Stz;
  i8 i_DstColumn = pStreamDefOut->nTotalColumns();
  Column *pColumn = nullptr;
  TagSet oTagSet;

  Token* pTkn = pTknVec->pCurrToken();

  Stz* azCrs = m_sAsmText[i_DstColumn].szStz();

  while ( pTkn && ! pTkn->bSame(","_Stz) ) {

    // alias ?
    if (pTkn->bSame("as"_Stz)) {
      pTkn = pTknVec->pNextToken();
      if (!pTkn->bIsName()) {
        eError = EC_INVALID_COMMAND;
        goto L_RETURN;
      }

      // extract column name
      sAliasName = pTkn->szChars();
      pTkn = pTknVec->pNextToken();
      continue;
    }

    // arithmetic operators
    if (pTkn->bSame("+"_Stz)) {
      //vAddCode(i_DstColumn, EO_ADD);
      StrLib::vSprintCat(azCrs,"EO_ADD\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("/"_Stz)) {
      //vAddCode(i_DstColumn, EO_DIVIDE);
      StrLib::vSprintCat(azCrs,"EO_DIVIDE\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("%"_Stz)) {
      //vAddCode(i_DstColumn, EO_MODULO);
      StrLib::vSprintCat(azCrs,"EO_MODULO\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("*"_Stz)) {
      //vAddCode(i_DstColumn, EO_MULTIPLY);
      StrLib::vSprintCat(azCrs,"EO_MULTIPLY\n"_Stz);
      pTkn = pTknVec->pNextToken();
      continue;
    }
    if (pTkn->bSame("-"_Stz)) {
      //vAddCode(i_DstColumn, EO_SUBTRACT);
      StrLib::vSprintCat(azCrs,"EO_SUBTRACT\n"_Stz);
      continue;
    }

    // is it an integer ?
    if ( pTkn->bIsInteger() ) {
      StrLib::vSprintCat(azCrs,"EO_INTEGER %lld\n"_Stz,pTkn->iToI8());
      pTkn = pTknVec->pNextToken();
      continue;
    }

    if ( ! pTkn->bIsName() ) {
      eError = EC_INVALID_COMMAND;
      break;
    }

    // found a src column
    Stz* sColumnNm = pTkn->szChars();

    // look for column in streamDef
    bool bFound = false;
    i8 nColumns = pStreamDefIn->nColumns(C_ALL);

    pColumn = nullptr;
    i8 i_StreamInCol;
    for (i_StreamInCol = 0; i_StreamInCol < nColumns; i_StreamInCol++) {
      pColumn = pStreamDefIn->pGetColumn(i_StreamInCol);
      if (pColumn->szColNm()->bSame(sColumnNm)) {
        break;
      }
    }

    // add the column to the streamDefOut
    pStreamDefOut->vAddColumn(sAliasName->bSame(""_Stz) ? pColumn->szColNm() : sAliasName,
                              pColumn->eColumnType(),
                              pColumn->iColumnLength(),
                              pColumn->iColumnScale());

    // find the column in the ProjectColumns
    i8 i_Local = -1;
    for (i8 i_Bit = 0; i_Bit <= i_StreamInCol; i_Bit++) {
      if (BitLib::bIsOn(m_uProjectColumns, i_Bit))
        i_Local++;
    }
    //vAddCodeArg(i_DstColumn, EO_PUSH_LOCALI, i_Local);
    StrLib::vSprintCat(azCrs,"EO_PUSH_LOCALI %lld\n"_Stz,i_Local);
    pTkn = pTknVec->pNextToken();
  }

    pColumn = pStreamDefOut->pGetColumn(i_DstColumn);
    switch (pColumn->eColumnType()) {
      case CT_INTEGER:
      case CT_DECIMAL:
        //vAddCodeArg(i_Col, EO_ROW_DST_POPCOLI, i_Col);
        StrLib::vSprintCat(azCrs,"EO_ROW_DST_POPCOLI %lld\n"_Stz,i_DstColumn);
        break;
      case CT_CHAR:
      case CT_DATE:
      case CT_VARCHAR:
        //vAddCodeArg(i_Col, EO_ROW_DST_POPCOLS, i_Col);
        StrLib::vSprintCat(azCrs,"EO_ROW_DST_POPCOLS %lld\n"_Stz,i_DstColumn);
        break;
      default:
        RASSERT(0);
    }

  L_RETURN:
    return eError;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Project);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
