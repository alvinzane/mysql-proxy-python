/* LICENSE BEGIN

Copyright (c) 2010 Ysj.Ray

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 LICENSE END */

#include "sql-tokenizer.h"

#define S(x) #x

/**
 * this list has to be kept in sync with the tokens itself 
 *
 * maps token_ids (array-pos) to token-names
 */
static const char *token_names[] = {
	S(TK_UNKNOWN),
	S(TK_LE),
	S(TK_GE),
	S(TK_LT),
	S(TK_GT),
	S(TK_EQ),
	S(TK_NE),
	S(TK_STRING),
	S(TK_COMMENT),
	S(TK_LITERAL),
	S(TK_FUNCTION),

	S(TK_INTEGER),
	S(TK_FLOAT),
	S(TK_DOT),
	S(TK_COMMA),
	S(TK_ASSIGN),
	S(TK_OBRACE),
	S(TK_CBRACE),
	S(TK_SEMICOLON),
	S(TK_STAR),
	S(TK_PLUS),
	S(TK_MINUS),
	S(TK_DIV),

	S(TK_BITWISE_AND),
	S(TK_BITWISE_OR),
	S(TK_BITWISE_XOR),
	S(TK_LOGICAL_AND),
	S(TK_LOGICAL_OR),

	S(TK_SQL_ACCESSIBLE),
	S(TK_SQL_ACTION),
	S(TK_SQL_ADD),
	S(TK_SQL_ALL),
	S(TK_SQL_ALTER),
	S(TK_SQL_ANALYZE),
	S(TK_SQL_AND),
	S(TK_SQL_AS),
	S(TK_SQL_ASC),
	S(TK_SQL_ASENSITIVE),
	S(TK_SQL_BEFORE),
	S(TK_SQL_BETWEEN),
	S(TK_SQL_BIGINT),
	S(TK_SQL_BINARY),
	S(TK_SQL_BIT),
	S(TK_SQL_BLOB),
	S(TK_SQL_BOTH),
	S(TK_SQL_BY),
	S(TK_SQL_CALL),
	S(TK_SQL_CASCADE),
	S(TK_SQL_CASE),
	S(TK_SQL_CHANGE),
	S(TK_SQL_CHAR),
	S(TK_SQL_CHARACTER),
	S(TK_SQL_CHECK),
	S(TK_SQL_COLLATE),
	S(TK_SQL_COLUMN),
	S(TK_SQL_CONDITION),
	S(TK_SQL_CONSTRAINT),
	S(TK_SQL_CONTINUE),
	S(TK_SQL_CONVERT),
	S(TK_SQL_CREATE),
	S(TK_SQL_CROSS),
	S(TK_SQL_CURRENT_DATE),
	S(TK_SQL_CURRENT_TIME),
	S(TK_SQL_CURRENT_TIMESTAMP),
	S(TK_SQL_CURRENT_USER),
	S(TK_SQL_CURSOR),
	S(TK_SQL_DATABASE),
	S(TK_SQL_DATABASES),
	S(TK_SQL_DATE),
	S(TK_SQL_DAY_HOUR),
	S(TK_SQL_DAY_MICROSECOND),
	S(TK_SQL_DAY_MINUTE),
	S(TK_SQL_DAY_SECOND),
	S(TK_SQL_DEC),
	S(TK_SQL_DECIMAL),
	S(TK_SQL_DECLARE),
	S(TK_SQL_DEFAULT),
	S(TK_SQL_DELAYED),
	S(TK_SQL_DELETE),
	S(TK_SQL_DESC),
	S(TK_SQL_DESCRIBE),
	S(TK_SQL_DETERMINISTIC),
	S(TK_SQL_DISTINCT),
	S(TK_SQL_DISTINCTROW),
	S(TK_SQL_DIV),
	S(TK_SQL_DOUBLE),
	S(TK_SQL_DROP),
	S(TK_SQL_DUAL),
	S(TK_SQL_EACH),
	S(TK_SQL_ELSE),
	S(TK_SQL_ELSEIF),
	S(TK_SQL_ENCLOSED),
	S(TK_SQL_ENUM),
	S(TK_SQL_ESCAPED),
	S(TK_SQL_EXISTS),
	S(TK_SQL_EXIT),
	S(TK_SQL_EXPLAIN),
	S(TK_SQL_FALSE),
	S(TK_SQL_FETCH),
	S(TK_SQL_FLOAT),
	S(TK_SQL_FLOAT4),
	S(TK_SQL_FLOAT8),
	S(TK_SQL_FOR),
	S(TK_SQL_FORCE),
	S(TK_SQL_FOREIGN),
	S(TK_SQL_FROM),
	S(TK_SQL_FULLTEXT),
	S(TK_SQL_GRANT),
	S(TK_SQL_GROUP),
	S(TK_SQL_HAVING),
	S(TK_SQL_HIGH_PRIORITY),
	S(TK_SQL_HOUR_MICROSECOND),
	S(TK_SQL_HOUR_MINUTE),
	S(TK_SQL_HOUR_SECOND),
	S(TK_SQL_IF),
	S(TK_SQL_IGNORE),
	S(TK_SQL_IN),
	S(TK_SQL_INDEX),
	S(TK_SQL_INFILE),
	S(TK_SQL_INNER),
	S(TK_SQL_INOUT),
	S(TK_SQL_INSENSITIVE),
	S(TK_SQL_INSERT),
	S(TK_SQL_INT),
	S(TK_SQL_INT1),
	S(TK_SQL_INT2),
	S(TK_SQL_INT3),
	S(TK_SQL_INT4),
	S(TK_SQL_INT8),
	S(TK_SQL_INTEGER),
	S(TK_SQL_INTERVAL),
	S(TK_SQL_INTO),
	S(TK_SQL_IS),
	S(TK_SQL_ITERATE),
	S(TK_SQL_JOIN),
	S(TK_SQL_KEY),
	S(TK_SQL_KEYS),
	S(TK_SQL_KILL),
	S(TK_SQL_LEADING),
	S(TK_SQL_LEAVE),
	S(TK_SQL_LEFT),
	S(TK_SQL_LIKE),
	S(TK_SQL_LIMIT),
	S(TK_SQL_LINEAR),
	S(TK_SQL_LINES),
	S(TK_SQL_LOAD),
	S(TK_SQL_LOCALTIME),
	S(TK_SQL_LOCALTIMESTAMP),
	S(TK_SQL_LOCK),
	S(TK_SQL_LONG),
	S(TK_SQL_LONGBLOB),
	S(TK_SQL_LONGTEXT),
	S(TK_SQL_LOOP),
	S(TK_SQL_LOW_PRIORITY),
	S(TK_SQL_MASTER_SSL_VERIFY_SERVER_CERT),
	S(TK_SQL_MATCH),
	S(TK_SQL_MEDIUMBLOB),
	S(TK_SQL_MEDIUMINT),
	S(TK_SQL_MEDIUMTEXT),
	S(TK_SQL_MIDDLEINT),
	S(TK_SQL_MINUTE_MICROSECOND),
	S(TK_SQL_MINUTE_SECOND),
	S(TK_SQL_MOD),
	S(TK_SQL_MODIFIES),
	S(TK_SQL_NATURAL),
	S(TK_SQL_NO),
	S(TK_SQL_NOT),
	S(TK_SQL_NO_WRITE_TO_BINLOG),
	S(TK_SQL_NULL),
	S(TK_SQL_NUMERIC),
	S(TK_SQL_ON),
	S(TK_SQL_OPTIMIZE),
	S(TK_SQL_OPTION),
	S(TK_SQL_OPTIONALLY),
	S(TK_SQL_OR),
	S(TK_SQL_ORDER),
	S(TK_SQL_OUT),
	S(TK_SQL_OUTER),
	S(TK_SQL_OUTFILE),
	S(TK_SQL_PRECISION),
	S(TK_SQL_PRIMARY),
	S(TK_SQL_PROCEDURE),
	S(TK_SQL_PURGE),
	S(TK_SQL_RANGE),
	S(TK_SQL_READ),
	S(TK_SQL_READ_ONLY),
	S(TK_SQL_READS),
	S(TK_SQL_READ_WRITE),
	S(TK_SQL_REAL),
	S(TK_SQL_REFERENCES),
	S(TK_SQL_REGEXP),
	S(TK_SQL_RELEASE),
	S(TK_SQL_RENAME),
	S(TK_SQL_REPEAT),
	S(TK_SQL_REPLACE),
	S(TK_SQL_REQUIRE),
	S(TK_SQL_RESTRICT),
	S(TK_SQL_RETURN),
	S(TK_SQL_REVOKE),
	S(TK_SQL_RIGHT),
	S(TK_SQL_RLIKE),
	S(TK_SQL_SCHEMA),
	S(TK_SQL_SCHEMAS),
	S(TK_SQL_SECOND_MICROSECOND),
	S(TK_SQL_SELECT),
	S(TK_SQL_SENSITIVE),
	S(TK_SQL_SEPARATOR),
	S(TK_SQL_SET),
	S(TK_SQL_SHOW),
	S(TK_SQL_SMALLINT),
	S(TK_SQL_SPATIAL),
	S(TK_SQL_SPECIFIC),
	S(TK_SQL_SQL),
	S(TK_SQL_SQL_BIG_RESULT),
	S(TK_SQL_SQL_CALC_FOUND_ROWS),
	S(TK_SQL_SQLEXCEPTION),
	S(TK_SQL_SQL_SMALL_RESULT),
	S(TK_SQL_SQLSTATE),
	S(TK_SQL_SQLWARNING),
	S(TK_SQL_SSL),
	S(TK_SQL_STARTING),
	S(TK_SQL_STRAIGHT_JOIN),
	S(TK_SQL_TABLE),
	S(TK_SQL_TERMINATED),
	S(TK_SQL_TEXT),
	S(TK_SQL_THEN),
	S(TK_SQL_TIME),
	S(TK_SQL_TIMESTAMP),
	S(TK_SQL_TINYBLOB),
	S(TK_SQL_TINYINT),
	S(TK_SQL_TINYTEXT),
	S(TK_SQL_TO),
	S(TK_SQL_TRAILING),
	S(TK_SQL_TRIGGER),
	S(TK_SQL_TRUE),
	S(TK_SQL_UNDO),
	S(TK_SQL_UNION),
	S(TK_SQL_UNIQUE),
	S(TK_SQL_UNLOCK),
	S(TK_SQL_UNSIGNED),
	S(TK_SQL_UPDATE),
	S(TK_SQL_USAGE),
	S(TK_SQL_USE),
	S(TK_SQL_USING),
	S(TK_SQL_UTC_DATE),
	S(TK_SQL_UTC_TIME),
	S(TK_SQL_UTC_TIMESTAMP),
	S(TK_SQL_VALUES),
	S(TK_SQL_VARBINARY),
	S(TK_SQL_VARCHAR),
	S(TK_SQL_VARCHARACTER),
	S(TK_SQL_VARYING),
	S(TK_SQL_WHEN),
	S(TK_SQL_WHERE),
	S(TK_SQL_WHILE),
	S(TK_SQL_WITH),
	S(TK_SQL_WRITE),
	S(TK_SQL_X509),
	S(TK_SQL_XOR),
	S(TK_SQL_YEAR_MONTH),
	S(TK_SQL_ZEROFILL),
	
	S(TK_COMMENT_MYSQL),

	NULL
};
#undef S

/**
 * get the name for a token-id
 */
const gchar *sql_token_get_name(sql_token_id token_id) {
	if (token_id >= TK_LAST_TOKEN) return NULL;

	if (sizeof(token_names)/sizeof(char *) != TK_LAST_TOKEN + 1) {
		g_error("sql_token_get_name() is out of sync [%"G_GSIZE_FORMAT" != %d]", sizeof(token_names)/sizeof(char *), TK_LAST_TOKEN + 1);
	}

	return token_names[token_id];
}

int sql_token_get_last_id() {
	return (sizeof(token_names)/sizeof(token_names[0])) - 1; /* the last one is not a token */
}

