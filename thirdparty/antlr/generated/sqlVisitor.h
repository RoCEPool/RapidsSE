
// Generated from ./sql.g4 by ANTLR 4.8

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
    virtual antlrcpp::Any visitStatement(sqlParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitStatementChoice(sqlParser::StatementChoiceContext *context) = 0;

    virtual antlrcpp::Any visitQueryStatement(sqlParser::QueryStatementContext *context) = 0;

    virtual antlrcpp::Any visitExplainStatement(sqlParser::ExplainStatementContext *context) = 0;

    virtual antlrcpp::Any visitStatisticsStatement(sqlParser::StatisticsStatementContext *context) = 0;

    virtual antlrcpp::Any visitQuery(sqlParser::QueryContext *context) = 0;

    virtual antlrcpp::Any visitQueryHint(sqlParser::QueryHintContext *context) = 0;

    virtual antlrcpp::Any visitHint(sqlParser::HintContext *context) = 0;

    virtual antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *context) = 0;

    virtual antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *context) = 0;

    virtual antlrcpp::Any visitSetQuery(sqlParser::SetQueryContext *context) = 0;

    virtual antlrcpp::Any visitSetExpr(sqlParser::SetExprContext *context) = 0;

    virtual antlrcpp::Any visitCorrClause(sqlParser::CorrClauseContext *context) = 0;

    virtual antlrcpp::Any visitWithQuery(sqlParser::WithQueryContext *context) = 0;

    virtual antlrcpp::Any visitSubquery(sqlParser::SubqueryContext *context) = 0;

    virtual antlrcpp::Any visitJoinUsing(sqlParser::JoinUsingContext *context) = 0;

    virtual antlrcpp::Any visitJoinOn(sqlParser::JoinOnContext *context) = 0;

    virtual antlrcpp::Any visitFetch(sqlParser::FetchContext *context) = 0;

    virtual antlrcpp::Any visitValuesSubquery(sqlParser::ValuesSubqueryContext *context) = 0;

    virtual antlrcpp::Any visitCrossJoin(sqlParser::CrossJoinContext *context) = 0;

    virtual antlrcpp::Any visitTableReference(sqlParser::TableReferenceContext *context) = 0;

    virtual antlrcpp::Any visitJoinType(sqlParser::JoinTypeContext *context) = 0;

    virtual antlrcpp::Any visitJoinHint(sqlParser::JoinHintContext *context) = 0;

    virtual antlrcpp::Any visitValuesExpr(sqlParser::ValuesExprContext *context) = 0;

    virtual antlrcpp::Any visitInsertQuery(sqlParser::InsertQueryContext *context) = 0;

    virtual antlrcpp::Any visitDeleteQuery(sqlParser::DeleteQueryContext *context) = 0;

    virtual antlrcpp::Any visitUpdateQuery(sqlParser::UpdateQueryContext *context) = 0;

    virtual antlrcpp::Any visitCreateQuery(sqlParser::CreateQueryContext *context) = 0;

    virtual antlrcpp::Any visitCreateTableQuery(sqlParser::CreateTableQueryContext *context) = 0;

    virtual antlrcpp::Any visitCreateIndexQuery(sqlParser::CreateIndexQueryContext *context) = 0;

    virtual antlrcpp::Any visitDropTable(sqlParser::DropTableContext *context) = 0;

    virtual antlrcpp::Any visitDropIndex(sqlParser::DropIndexContext *context) = 0;

    virtual antlrcpp::Any visitTruncateQuery(sqlParser::TruncateQueryContext *context) = 0;

    virtual antlrcpp::Any visitTableDefinition(sqlParser::TableDefinitionContext *context) = 0;

    virtual antlrcpp::Any visitTablePartitioning(sqlParser::TablePartitioningContext *context) = 0;

    virtual antlrcpp::Any visitColumnDefinition(sqlParser::ColumnDefinitionContext *context) = 0;

    virtual antlrcpp::Any visitColumnNameList(sqlParser::ColumnNameListContext *context) = 0;

    virtual antlrcpp::Any visitTableConstraint(sqlParser::TableConstraintContext *context) = 0;

    virtual antlrcpp::Any visitIndexDefinition(sqlParser::IndexDefinitionContext *context) = 0;

    virtual antlrcpp::Any visitIndexMethod(sqlParser::IndexMethodContext *context) = 0;

    virtual antlrcpp::Any visitDataType(sqlParser::DataTypeContext *context) = 0;

    virtual antlrcpp::Any visitSqlDataType(sqlParser::SqlDataTypeContext *context) = 0;

    virtual antlrcpp::Any visitAsSubquery(sqlParser::AsSubqueryContext *context) = 0;

    virtual antlrcpp::Any visitSelectOrValues(sqlParser::SelectOrValuesContext *context) = 0;

    virtual antlrcpp::Any visitSelectList(sqlParser::SelectListContext *context) = 0;

    virtual antlrcpp::Any visitSelectListItem(sqlParser::SelectListItemContext *context) = 0;

    virtual antlrcpp::Any visitGroupByList(sqlParser::GroupByListContext *context) = 0;

    virtual antlrcpp::Any visitGroupByListItem(sqlParser::GroupByListItemContext *context) = 0;

    virtual antlrcpp::Any visitOrderByList(sqlParser::OrderByListContext *context) = 0;

    virtual antlrcpp::Any visitOrderByListItem(sqlParser::OrderByListItemContext *context) = 0;

    virtual antlrcpp::Any visitJoinUsingList(sqlParser::JoinUsingListContext *context) = 0;

    virtual antlrcpp::Any visitJoinUsingListItem(sqlParser::JoinUsingListItemContext *context) = 0;

    virtual antlrcpp::Any visitCorrList(sqlParser::CorrListContext *context) = 0;

    virtual antlrcpp::Any visitCorrListItem(sqlParser::CorrListItemContext *context) = 0;

    virtual antlrcpp::Any visitPropertyList(sqlParser::PropertyListContext *context) = 0;

    virtual antlrcpp::Any visitProperty(sqlParser::PropertyContext *context) = 0;

    virtual antlrcpp::Any visitValueList(sqlParser::ValueListContext *context) = 0;

    virtual antlrcpp::Any visitPredicate(sqlParser::PredicateContext *context) = 0;

    virtual antlrcpp::Any visitJoinPredicate(sqlParser::JoinPredicateContext *context) = 0;

    virtual antlrcpp::Any visitCasePredicate(sqlParser::CasePredicateContext *context) = 0;

    virtual antlrcpp::Any visitCountAll(sqlParser::CountAllContext *context) = 0;

    virtual antlrcpp::Any visitExtractFunc(sqlParser::ExtractFuncContext *context) = 0;

    virtual antlrcpp::Any visitScalarFunc(sqlParser::ScalarFuncContext *context) = 0;

    virtual antlrcpp::Any visitUnaryOp(sqlParser::UnaryOpContext *context) = 0;

    virtual antlrcpp::Any visitCoalesceOp(sqlParser::CoalesceOpContext *context) = 0;

    virtual antlrcpp::Any visitVarRef(sqlParser::VarRefContext *context) = 0;

    virtual antlrcpp::Any visitPositionFunc(sqlParser::PositionFuncContext *context) = 0;

    virtual antlrcpp::Any visitColRef(sqlParser::ColRefContext *context) = 0;

    virtual antlrcpp::Any visitCaseOp(sqlParser::CaseOpContext *context) = 0;

    virtual antlrcpp::Any visitLiteralVal(sqlParser::LiteralValContext *context) = 0;

    virtual antlrcpp::Any visitOverlayFunc(sqlParser::OverlayFuncContext *context) = 0;

    virtual antlrcpp::Any visitOrOp(sqlParser::OrOpContext *context) = 0;

    virtual antlrcpp::Any visitIfNullOp(sqlParser::IfNullOpContext *context) = 0;

    virtual antlrcpp::Any visitSubqueryExistsOp(sqlParser::SubqueryExistsOpContext *context) = 0;

    virtual antlrcpp::Any visitAndOp(sqlParser::AndOpContext *context) = 0;

    virtual antlrcpp::Any visitNotOp(sqlParser::NotOpContext *context) = 0;

    virtual antlrcpp::Any visitIsNullOp(sqlParser::IsNullOpContext *context) = 0;

    virtual antlrcpp::Any visitInOp(sqlParser::InOpContext *context) = 0;

    virtual antlrcpp::Any visitTrimFunc(sqlParser::TrimFuncContext *context) = 0;

    virtual antlrcpp::Any visitLikeOp(sqlParser::LikeOpContext *context) = 0;

    virtual antlrcpp::Any visitTypeOfOp(sqlParser::TypeOfOpContext *context) = 0;

    virtual antlrcpp::Any visitNullIfOp(sqlParser::NullIfOpContext *context) = 0;

    virtual antlrcpp::Any visitInlineCast(sqlParser::InlineCastContext *context) = 0;

    virtual antlrcpp::Any visitBinaryOp(sqlParser::BinaryOpContext *context) = 0;

    virtual antlrcpp::Any visitAggFunc(sqlParser::AggFuncContext *context) = 0;

    virtual antlrcpp::Any visitDecodeOp(sqlParser::DecodeOpContext *context) = 0;

    virtual antlrcpp::Any visitConcatOp(sqlParser::ConcatOpContext *context) = 0;

    virtual antlrcpp::Any visitSubqueryExpr(sqlParser::SubqueryExprContext *context) = 0;

    virtual antlrcpp::Any visitIfOp(sqlParser::IfOpContext *context) = 0;

    virtual antlrcpp::Any visitLikeEscapeOp(sqlParser::LikeEscapeOpContext *context) = 0;

    virtual antlrcpp::Any visitSubStringFunc(sqlParser::SubStringFuncContext *context) = 0;

    virtual antlrcpp::Any visitSubqueryInOp(sqlParser::SubqueryInOpContext *context) = 0;

    virtual antlrcpp::Any visitCountFunc(sqlParser::CountFuncContext *context) = 0;

    virtual antlrcpp::Any visitCastOp(sqlParser::CastOpContext *context) = 0;

    virtual antlrcpp::Any visitParenthetic(sqlParser::ParentheticContext *context) = 0;

    virtual antlrcpp::Any visitMethodCall(sqlParser::MethodCallContext *context) = 0;

    virtual antlrcpp::Any visitVariableName(sqlParser::VariableNameContext *context) = 0;

    virtual antlrcpp::Any visitColumnRef(sqlParser::ColumnRefContext *context) = 0;

    virtual antlrcpp::Any visitColumnWildcard(sqlParser::ColumnWildcardContext *context) = 0;

    virtual antlrcpp::Any visitClassName(sqlParser::ClassNameContext *context) = 0;

    virtual antlrcpp::Any visitFunctionName(sqlParser::FunctionNameContext *context) = 0;

    virtual antlrcpp::Any visitMethodName(sqlParser::MethodNameContext *context) = 0;

    virtual antlrcpp::Any visitCatalogName(sqlParser::CatalogNameContext *context) = 0;

    virtual antlrcpp::Any visitSchemaName(sqlParser::SchemaNameContext *context) = 0;

    virtual antlrcpp::Any visitTableName(sqlParser::TableNameContext *context) = 0;

    virtual antlrcpp::Any visitTableAlias(sqlParser::TableAliasContext *context) = 0;

    virtual antlrcpp::Any visitColumnName(sqlParser::ColumnNameContext *context) = 0;

    virtual antlrcpp::Any visitColumnAlias(sqlParser::ColumnAliasContext *context) = 0;

    virtual antlrcpp::Any visitCollationName(sqlParser::CollationNameContext *context) = 0;

    virtual antlrcpp::Any visitIndexName(sqlParser::IndexNameContext *context) = 0;

    virtual antlrcpp::Any visitPropertyName(sqlParser::PropertyNameContext *context) = 0;

    virtual antlrcpp::Any visitHintName(sqlParser::HintNameContext *context) = 0;

    virtual antlrcpp::Any visitPropertyValue(sqlParser::PropertyValueContext *context) = 0;

    virtual antlrcpp::Any visitHintValue(sqlParser::HintValueContext *context) = 0;

    virtual antlrcpp::Any visitIdentifier(sqlParser::IdentifierContext *context) = 0;

    virtual antlrcpp::Any visitQualifier(sqlParser::QualifierContext *context) = 0;

    virtual antlrcpp::Any visitPrecision(sqlParser::PrecisionContext *context) = 0;

    virtual antlrcpp::Any visitScale(sqlParser::ScaleContext *context) = 0;

    virtual antlrcpp::Any visitSize(sqlParser::SizeContext *context) = 0;

    virtual antlrcpp::Any visitLiteral(sqlParser::LiteralContext *context) = 0;

    virtual antlrcpp::Any visitSimpleLiteral(sqlParser::SimpleLiteralContext *context) = 0;

    virtual antlrcpp::Any visitBooleanLiteral(sqlParser::BooleanLiteralContext *context) = 0;

    virtual antlrcpp::Any visitIntegerLiteral(sqlParser::IntegerLiteralContext *context) = 0;

    virtual antlrcpp::Any visitDecimalLiteral(sqlParser::DecimalLiteralContext *context) = 0;

    virtual antlrcpp::Any visitFloatLiteral(sqlParser::FloatLiteralContext *context) = 0;

    virtual antlrcpp::Any visitDateLiteral(sqlParser::DateLiteralContext *context) = 0;

    virtual antlrcpp::Any visitTimestampLiteral(sqlParser::TimestampLiteralContext *context) = 0;

    virtual antlrcpp::Any visitStringLiteral(sqlParser::StringLiteralContext *context) = 0;

    virtual antlrcpp::Any visitIntervalLiteral(sqlParser::IntervalLiteralContext *context) = 0;

    virtual antlrcpp::Any visitLooseInterval(sqlParser::LooseIntervalContext *context) = 0;

    virtual antlrcpp::Any visitSql92CalendarInterval(sqlParser::Sql92CalendarIntervalContext *context) = 0;

    virtual antlrcpp::Any visitSql92TimeInterval(sqlParser::Sql92TimeIntervalContext *context) = 0;

    virtual antlrcpp::Any visitNonReservedWord(sqlParser::NonReservedWordContext *context) = 0;


};

}  // namespace Parser
