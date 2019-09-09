/*-------------------------------------------------------------------------
 *
 * uint.c
 *		Overflow checks for unsigned integers
 *
 * Copyright (c) 1996-2019, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *		  overflow/uint.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "fmgr.h"

#include "common/int.h"
#include "common/string.h"
#include "utils/builtins.h"

/* smallint functions */
PG_FUNCTION_INFO_V1(pg_add_uint16_overflow);
PG_FUNCTION_INFO_V1(pg_sub_uint16_overflow);
PG_FUNCTION_INFO_V1(pg_mul_uint16_overflow);

Datum
pg_add_uint16_overflow(PG_FUNCTION_ARGS)
{
	uint16		v1 = PG_GETARG_UINT16(0);
	uint16		v2 = PG_GETARG_UINT16(1);
	uint16		result;

	PG_RETURN_BOOL(pg_add_u16_overflow(v1, v2, &result));
}

Datum
pg_sub_uint16_overflow(PG_FUNCTION_ARGS)
{
	uint16		v1 = PG_GETARG_UINT16(0);
	uint16		v2 = PG_GETARG_UINT16(1);
	uint16		result;

	PG_RETURN_BOOL(pg_sub_u16_overflow(v1, v2, &result));
}

Datum
pg_mul_uint16_overflow(PG_FUNCTION_ARGS)
{
	uint16		v1 = PG_GETARG_UINT16(0);
	uint16		v2 = PG_GETARG_UINT16(1);
	uint16		result;

	PG_RETURN_BOOL(pg_mul_u16_overflow(v1, v2, &result));
}

/* int functions */
PG_FUNCTION_INFO_V1(pg_add_uint32_overflow);
PG_FUNCTION_INFO_V1(pg_sub_uint32_overflow);
PG_FUNCTION_INFO_V1(pg_mul_uint32_overflow);

Datum
pg_add_uint32_overflow(PG_FUNCTION_ARGS)
{
	uint32		v1 = PG_GETARG_UINT32(0);
	uint32		v2 = PG_GETARG_UINT32(1);
	uint32		result;

	PG_RETURN_BOOL(pg_add_u32_overflow(v1, v2, &result));
}

Datum
pg_sub_uint32_overflow(PG_FUNCTION_ARGS)
{
	uint32		v1 = PG_GETARG_UINT32(0);
	uint32		v2 = PG_GETARG_UINT32(1);
	uint32		result;

	PG_RETURN_BOOL(pg_sub_u32_overflow(v1, v2, &result));
}

Datum
pg_mul_uint32_overflow(PG_FUNCTION_ARGS)
{
	uint32		v1 = PG_GETARG_UINT32(0);
	uint32		v2 = PG_GETARG_UINT32(1);
	uint32		result;

	PG_RETURN_BOOL(pg_mul_u32_overflow(v1, v2, &result));
}

/* bigint functions */
PG_FUNCTION_INFO_V1(pg_add_uint64_overflow);
PG_FUNCTION_INFO_V1(pg_sub_uint64_overflow);
PG_FUNCTION_INFO_V1(pg_mul_uint64_overflow);

Datum
pg_add_uint64_overflow(PG_FUNCTION_ARGS)
{
	uint64		v1 = (uint64) PG_GETARG_INT64(0);
	uint64		v2 = (uint64) PG_GETARG_INT64(1);
	uint64		result;

	PG_RETURN_BOOL(pg_add_u64_overflow(v1, v2, &result));
}

Datum
pg_sub_uint64_overflow(PG_FUNCTION_ARGS)
{
	uint64		v1 = (uint64) PG_GETARG_INT64(0);
	uint64		v2 = (uint64) PG_GETARG_INT64(1);
	uint64		result;

	PG_RETURN_BOOL(pg_sub_u64_overflow(v1, v2, &result));
}

Datum
pg_mul_uint64_overflow(PG_FUNCTION_ARGS)
{
	uint64		v1 = (uint64) PG_GETARG_INT64(0);
	uint64		v2 = (uint64) PG_GETARG_INT64(1);
	uint64		result;

	PG_RETURN_BOOL(pg_mul_u64_overflow(v1, v2, &result));
}

/* string to uint conversion functions */
PG_FUNCTION_INFO_V1(pg_string_to_uint16);
PG_FUNCTION_INFO_V1(pg_string_to_uint32);
PG_FUNCTION_INFO_V1(pg_string_to_uint64);

Datum
pg_string_to_uint16(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	char       *val = text_to_cstring(val_txt);
	pg_strtoint_status status = PG_STRTOINT_OK;
	uint16		res;

	status = pg_strtouint16(val, &res);

	if (status == PG_STRTOINT_RANGE_ERROR)
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("value \"%s\" is out of range for type %s",
						val, "smallint")));
	else if (status == PG_STRTOINT_SYNTAX_ERROR)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"smallint", val)));

	PG_RETURN_INT16(res);
}

/*
 * String to uint functions
 */
Datum
pg_string_to_uint32(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	char       *val = text_to_cstring(val_txt);
	pg_strtoint_status status = PG_STRTOINT_OK;
	uint32		res;

	status = pg_strtouint32(val, &res);

	if (status == PG_STRTOINT_RANGE_ERROR)
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("value \"%s\" is out of range for type %s",
						val, "integer")));
	else if (status == PG_STRTOINT_SYNTAX_ERROR)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"integer", val)));

	PG_RETURN_INT32(res);
}

/*
 * String to uint functions
 */
Datum
pg_string_to_uint64(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	char       *val = text_to_cstring(val_txt);
	pg_strtoint_status status = PG_STRTOINT_OK;
	uint64		res;

	status = pg_strtouint64(val, &res);

	if (status == PG_STRTOINT_RANGE_ERROR)
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("value \"%s\" is out of range for type %s",
						val, "bigint")));
	else if (status == PG_STRTOINT_SYNTAX_ERROR)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"bigint", val)));

	PG_RETURN_INT64(res);
}
