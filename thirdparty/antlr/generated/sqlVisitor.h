
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "sqlParser.h"


namespace Parser {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by sqlParser.
 */
class  sqlVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by sqlParser.
   */
    virtual std::any visitStatements(sqlParser::StatementsContext *context) = 0;

    virtual std::any visitStatement(sqlParser::StatementContext *context) = 0;

    virtual std::any visitStatementChoice(sqlParser::StatementChoiceContext *context) = 0;

    virtual std::any visitTags(sqlParser::TagsContext *context) = 0;

    virtual std::any visitKeyValueTag(sqlParser::KeyValueTagContext *context) = 0;

    virtual std::any visitKeyOnlyTag(sqlParser::KeyOnlyTagContext *context) = 0;

    virtual std::any visitTagName(sqlParser::TagNameContext *context) = 0;

    virtual std::any visitQueryCmd(sqlParser::QueryCmdContext *context) = 0;

    virtual std::any visitQuery(sqlParser::QueryContext *context) = 0;

    virtual std::any visitQueryHint(sqlParser::QueryHintContext *context) = 0;

    virtual std::any visitHint(sqlParser::HintContext *context) = 0;

    virtual std::any visitSelectQuery(sqlParser::SelectQueryContext *context) = 0;

    virtual std::any visitSelectExpr(sqlParser::SelectExprContext *context) = 0;

    virtual std::any visitSetQuery(sqlParser::SetQueryContext *context) = 0;

    virtual std::any visitSetExpr(sqlParser::SetExprContext *context) = 0;

    virtual std::any visitCorrClause(sqlParser::CorrClauseContext *context) = 0;

    virtual std::any visitWithQuery(sqlParser::WithQueryContext *context) = 0;

    virtual std::any visitSubquery(sqlParser::SubqueryContext *context) = 0;

    virtual std::any visitNaturalJoin(sqlParser::NaturalJoinContext *context) = 0;

    virtual std::any visitJoinUsing(sqlParser::JoinUsingContext *context) = 0;

    virtual std::any visitJoinOn(sqlParser::JoinOnContext *context) = 0;

    virtual std::any visitFetch(sqlParser::FetchContext *context) = 0;

    virtual std::any visitValuesSubquery(sqlParser::ValuesSubqueryContext *context) = 0;

    virtual std::any visitCrossJoin(sqlParser::CrossJoinContext *context) = 0;

    virtual std::any visitTableReference(sqlParser::TableReferenceContext *context) = 0;

    virtual std::any visitJoinType(sqlParser::JoinTypeContext *context) = 0;

    virtual std::any visitExtendedJoinType(sqlParser::ExtendedJoinTypeContext *context) = 0;

    virtual std::any visitValuesExpr(sqlParser::ValuesExprContext *context) = 0;

    virtual std::any visitAnidxCmd(sqlParser::AnidxCmdContext *context) = 0;

    virtual std::any visitAntblCmd(sqlParser::AntblCmdContext *context) = 0;

    virtual std::any visitBpaCmd(sqlParser::BpaCmdContext *context) = 0;

    virtual std::any visitCompactIdxCmd(sqlParser::CompactIdxCmdContext *context) = 0;

    virtual std::any visitCountCmd(sqlParser::CountCmdContext *context) = 0;

    virtual std::any visitCreateCmd(sqlParser::CreateCmdContext *context) = 0;

    virtual std::any visitCreateTableCmd(sqlParser::CreateTableCmdContext *context) = 0;

    virtual std::any visitCreateIndexCmd(sqlParser::CreateIndexCmdContext *context) = 0;

    virtual std::any visitDeleteCmd(sqlParser::DeleteCmdContext *context) = 0;

    virtual std::any visitDisconnectCmd(sqlParser::DisconnectCmdContext *context) = 0;

    virtual std::any visitDloadCmd(sqlParser::DloadCmdContext *context) = 0;

    virtual std::any visitDocolCmd(sqlParser::DocolCmdContext *context) = 0;

    virtual std::any visitDropCmd(sqlParser::DropCmdContext *context) = 0;

    virtual std::any visitDropTableCmd(sqlParser::DropTableCmdContext *context) = 0;

    virtual std::any visitDropIndexCmd(sqlParser::DropIndexCmdContext *context) = 0;

    virtual std::any visitEmptyCmd(sqlParser::EmptyCmdContext *context) = 0;

    virtual std::any visitExplainCmd(sqlParser::ExplainCmdContext *context) = 0;

    virtual std::any visitExplainOptions(sqlParser::ExplainOptionsContext *context) = 0;

    virtual std::any visitExplainStageList(sqlParser::ExplainStageListContext *context) = 0;

    virtual std::any visitExplainRange(sqlParser::ExplainRangeContext *context) = 0;

    virtual std::any visitExplainRangeChoice(sqlParser::ExplainRangeChoiceContext *context) = 0;

    virtual std::any visitExplainFormat(sqlParser::ExplainFormatContext *context) = 0;

    virtual std::any visitExportCmd(sqlParser::ExportCmdContext *context) = 0;

    virtual std::any visitFileCmd(sqlParser::FileCmdContext *context) = 0;

    virtual std::any visitInsertCmd(sqlParser::InsertCmdContext *context) = 0;

    virtual std::any visitLoadCmd(sqlParser::LoadCmdContext *context) = 0;

    virtual std::any visitLoadReplicatedCmd(sqlParser::LoadReplicatedCmdContext *context) = 0;

    virtual std::any visitLoadOption(sqlParser::LoadOptionContext *context) = 0;

    virtual std::any visitLoadOptionKeys(sqlParser::LoadOptionKeysContext *context) = 0;

    virtual std::any visitLogCmd(sqlParser::LogCmdContext *context) = 0;

    virtual std::any visitLscolCmd(sqlParser::LscolCmdContext *context) = 0;

    virtual std::any visitLsidxCmd(sqlParser::LsidxCmdContext *context) = 0;

    virtual std::any visitLsidxcolCmd(sqlParser::LsidxcolCmdContext *context) = 0;

    virtual std::any visitLsptncolCmd(sqlParser::LsptncolCmdContext *context) = 0;

    virtual std::any visitLstblCmd(sqlParser::LstblCmdContext *context) = 0;

    virtual std::any visitMemoryleaksCmd(sqlParser::MemoryleaksCmdContext *context) = 0;

    virtual std::any visitMetricsCtlCmd(sqlParser::MetricsCtlCmdContext *context) = 0;

    virtual std::any visitNoopCmd(sqlParser::NoopCmdContext *context) = 0;

    virtual std::any visitPipelineCmd(sqlParser::PipelineCmdContext *context) = 0;

    virtual std::any visitPipelineStage(sqlParser::PipelineStageContext *context) = 0;

    virtual std::any visitPipelineOpSeq(sqlParser::PipelineOpSeqContext *context) = 0;

    virtual std::any visitPipelineOperator(sqlParser::PipelineOperatorContext *context) = 0;

    virtual std::any visitPipelineOpParam(sqlParser::PipelineOpParamContext *context) = 0;

    virtual std::any visitPipelineTypeNames(sqlParser::PipelineTypeNamesContext *context) = 0;

    virtual std::any visitSetTraceCmd(sqlParser::SetTraceCmdContext *context) = 0;

    virtual std::any visitSleepCmd(sqlParser::SleepCmdContext *context) = 0;

    virtual std::any visitSleepUnits(sqlParser::SleepUnitsContext *context) = 0;

    virtual std::any visitSpaceCmd(sqlParser::SpaceCmdContext *context) = 0;

    virtual std::any visitStatusCmd(sqlParser::StatusCmdContext *context) = 0;

    virtual std::any visitSzidxCmd(sqlParser::SzidxCmdContext *context) = 0;

    virtual std::any visitSztblCmd(sqlParser::SztblCmdContext *context) = 0;

    virtual std::any visitTableSizesCmd(sqlParser::TableSizesCmdContext *context) = 0;

    virtual std::any visitTraceOption(sqlParser::TraceOptionContext *context) = 0;

    virtual std::any visitTruncateCmd(sqlParser::TruncateCmdContext *context) = 0;

    virtual std::any visitTrackmemoryCmd(sqlParser::TrackmemoryCmdContext *context) = 0;

    virtual std::any visitUpdateCmd(sqlParser::UpdateCmdContext *context) = 0;

    virtual std::any visitUnloadCmd(sqlParser::UnloadCmdContext *context) = 0;

    virtual std::any visitTableDefinition(sqlParser::TableDefinitionContext *context) = 0;

    virtual std::any visitTablePartitioning(sqlParser::TablePartitioningContext *context) = 0;

    virtual std::any visitColumnDefinition(sqlParser::ColumnDefinitionContext *context) = 0;

    virtual std::any visitColumnNameList(sqlParser::ColumnNameListContext *context) = 0;

    virtual std::any visitIndexDefinition(sqlParser::IndexDefinitionContext *context) = 0;

    virtual std::any visitIndexMethod(sqlParser::IndexMethodContext *context) = 0;

    virtual std::any visitDataType(sqlParser::DataTypeContext *context) = 0;

    virtual std::any visitSqlDataType(sqlParser::SqlDataTypeContext *context) = 0;

    virtual std::any visitIntervalType(sqlParser::IntervalTypeContext *context) = 0;

    virtual std::any visitYearMonthIntervalType(sqlParser::YearMonthIntervalTypeContext *context) = 0;

    virtual std::any visitDayTimeIntervalType(sqlParser::DayTimeIntervalTypeContext *context) = 0;

    virtual std::any visitFixedLenChar(sqlParser::FixedLenCharContext *context) = 0;

    virtual std::any visitVaryingLenChar(sqlParser::VaryingLenCharContext *context) = 0;

    virtual std::any visitAsSubquery(sqlParser::AsSubqueryContext *context) = 0;

    virtual std::any visitSelectOrValues(sqlParser::SelectOrValuesContext *context) = 0;

    virtual std::any visitSelectList(sqlParser::SelectListContext *context) = 0;

    virtual std::any visitSelectListItem(sqlParser::SelectListItemContext *context) = 0;

    virtual std::any visitGroupByList(sqlParser::GroupByListContext *context) = 0;

    virtual std::any visitGroupByListItem(sqlParser::GroupByListItemContext *context) = 0;

    virtual std::any visitOrderByList(sqlParser::OrderByListContext *context) = 0;

    virtual std::any visitOrderByListItem(sqlParser::OrderByListItemContext *context) = 0;

    virtual std::any visitJoinUsingList(sqlParser::JoinUsingListContext *context) = 0;

    virtual std::any visitJoinUsingListItem(sqlParser::JoinUsingListItemContext *context) = 0;

    virtual std::any visitCorrList(sqlParser::CorrListContext *context) = 0;

    virtual std::any visitCorrListItem(sqlParser::CorrListItemContext *context) = 0;

    virtual std::any visitColAliases(sqlParser::ColAliasesContext *context) = 0;

    virtual std::any visitColAliasList(sqlParser::ColAliasListContext *context) = 0;

    virtual std::any visitColAliasListItem(sqlParser::ColAliasListItemContext *context) = 0;

    virtual std::any visitPropertyList(sqlParser::PropertyListContext *context) = 0;

    virtual std::any visitProperty(sqlParser::PropertyContext *context) = 0;

    virtual std::any visitValueList(sqlParser::ValueListContext *context) = 0;

    virtual std::any visitPredicate(sqlParser::PredicateContext *context) = 0;

    virtual std::any visitJoinPredicate(sqlParser::JoinPredicateContext *context) = 0;

    virtual std::any visitCasePredicate(sqlParser::CasePredicateContext *context) = 0;

    virtual std::any visitCountAll(sqlParser::CountAllContext *context) = 0;

    virtual std::any visitExtractFunc(sqlParser::ExtractFuncContext *context) = 0;

    virtual std::any visitScalarFunc(sqlParser::ScalarFuncContext *context) = 0;

    virtual std::any visitUnaryOp(sqlParser::UnaryOpContext *context) = 0;

    virtual std::any visitCoalesceOp(sqlParser::CoalesceOpContext *context) = 0;

    virtual std::any visitVarRef(sqlParser::VarRefContext *context) = 0;

    virtual std::any visitPositionFunc(sqlParser::PositionFuncContext *context) = 0;

    virtual std::any visitColRef(sqlParser::ColRefContext *context) = 0;

    virtual std::any visitCaseOp(sqlParser::CaseOpContext *context) = 0;

    virtual std::any visitLiteralVal(sqlParser::LiteralValContext *context) = 0;

    virtual std::any visitOverlayFunc(sqlParser::OverlayFuncContext *context) = 0;

    virtual std::any visitIfNullOp(sqlParser::IfNullOpContext *context) = 0;

    virtual std::any visitSubqueryExistsOp(sqlParser::SubqueryExistsOpContext *context) = 0;

    virtual std::any visitIsNullOp(sqlParser::IsNullOpContext *context) = 0;

    virtual std::any visitInOp(sqlParser::InOpContext *context) = 0;

    virtual std::any visitTrimFunc(sqlParser::TrimFuncContext *context) = 0;

    virtual std::any visitLikeOp(sqlParser::LikeOpContext *context) = 0;

    virtual std::any visitTypeOfOp(sqlParser::TypeOfOpContext *context) = 0;

    virtual std::any visitNullIfOp(sqlParser::NullIfOpContext *context) = 0;

    virtual std::any visitInlineCast(sqlParser::InlineCastContext *context) = 0;

    virtual std::any visitBinaryOp(sqlParser::BinaryOpContext *context) = 0;

    virtual std::any visitBetweenOp(sqlParser::BetweenOpContext *context) = 0;

    virtual std::any visitAggFunc(sqlParser::AggFuncContext *context) = 0;

    virtual std::any visitDecodeOp(sqlParser::DecodeOpContext *context) = 0;

    virtual std::any visitConcatOp(sqlParser::ConcatOpContext *context) = 0;

    virtual std::any visitSubqueryExpr(sqlParser::SubqueryExprContext *context) = 0;

    virtual std::any visitIfOp(sqlParser::IfOpContext *context) = 0;

    virtual std::any visitLikeEscapeOp(sqlParser::LikeEscapeOpContext *context) = 0;

    virtual std::any visitSubStringFunc(sqlParser::SubStringFuncContext *context) = 0;

    virtual std::any visitSubqueryInOp(sqlParser::SubqueryInOpContext *context) = 0;

    virtual std::any visitLogicOp(sqlParser::LogicOpContext *context) = 0;

    virtual std::any visitCastOp(sqlParser::CastOpContext *context) = 0;

    virtual std::any visitParenthetic(sqlParser::ParentheticContext *context) = 0;

    virtual std::any visitMethodCall(sqlParser::MethodCallContext *context) = 0;

    virtual std::any visitVariableName(sqlParser::VariableNameContext *context) = 0;

    virtual std::any visitColumnRef(sqlParser::ColumnRefContext *context) = 0;

    virtual std::any visitColumnWildcard(sqlParser::ColumnWildcardContext *context) = 0;

    virtual std::any visitClassName(sqlParser::ClassNameContext *context) = 0;

    virtual std::any visitFunctionName(sqlParser::FunctionNameContext *context) = 0;

    virtual std::any visitMethodName(sqlParser::MethodNameContext *context) = 0;

    virtual std::any visitCatalogName(sqlParser::CatalogNameContext *context) = 0;

    virtual std::any visitSchemaName(sqlParser::SchemaNameContext *context) = 0;

    virtual std::any visitTableName(sqlParser::TableNameContext *context) = 0;

    virtual std::any visitTableAlias(sqlParser::TableAliasContext *context) = 0;

    virtual std::any visitColumnName(sqlParser::ColumnNameContext *context) = 0;

    virtual std::any visitColumnAlias(sqlParser::ColumnAliasContext *context) = 0;

    virtual std::any visitCollationName(sqlParser::CollationNameContext *context) = 0;

    virtual std::any visitIndexName(sqlParser::IndexNameContext *context) = 0;

    virtual std::any visitPropertyName(sqlParser::PropertyNameContext *context) = 0;

    virtual std::any visitHintName(sqlParser::HintNameContext *context) = 0;

    virtual std::any visitExprHint(sqlParser::ExprHintContext *context) = 0;

    virtual std::any visitPropertyValue(sqlParser::PropertyValueContext *context) = 0;

    virtual std::any visitHintValue(sqlParser::HintValueContext *context) = 0;

    virtual std::any visitIdentifier(sqlParser::IdentifierContext *context) = 0;

    virtual std::any visitQualifier(sqlParser::QualifierContext *context) = 0;

    virtual std::any visitPrecision(sqlParser::PrecisionContext *context) = 0;

    virtual std::any visitScale(sqlParser::ScaleContext *context) = 0;

    virtual std::any visitSize(sqlParser::SizeContext *context) = 0;

    virtual std::any visitLiteral(sqlParser::LiteralContext *context) = 0;

    virtual std::any visitSimpleLiteral(sqlParser::SimpleLiteralContext *context) = 0;

    virtual std::any visitBooleanLiteral(sqlParser::BooleanLiteralContext *context) = 0;

    virtual std::any visitIntegerLiteral(sqlParser::IntegerLiteralContext *context) = 0;

    virtual std::any visitSignedIntegerLiteral(sqlParser::SignedIntegerLiteralContext *context) = 0;

    virtual std::any visitDecimalLiteral(sqlParser::DecimalLiteralContext *context) = 0;

    virtual std::any visitSignedDecimalLiteral(sqlParser::SignedDecimalLiteralContext *context) = 0;

    virtual std::any visitFloatLiteral(sqlParser::FloatLiteralContext *context) = 0;

    virtual std::any visitSignedFloatLiteral(sqlParser::SignedFloatLiteralContext *context) = 0;

    virtual std::any visitDateLiteral(sqlParser::DateLiteralContext *context) = 0;

    virtual std::any visitTimeLiteral(sqlParser::TimeLiteralContext *context) = 0;

    virtual std::any visitTimestampLiteral(sqlParser::TimestampLiteralContext *context) = 0;

    virtual std::any visitStringLiteral(sqlParser::StringLiteralContext *context) = 0;

    virtual std::any visitIntervalLiteral(sqlParser::IntervalLiteralContext *context) = 0;

    virtual std::any visitYearMonthInterval(sqlParser::YearMonthIntervalContext *context) = 0;

    virtual std::any visitDayTimeInterval(sqlParser::DayTimeIntervalContext *context) = 0;

    virtual std::any visitNonReservedWord(sqlParser::NonReservedWordContext *context) = 0;


};

}  // namespace Parser
