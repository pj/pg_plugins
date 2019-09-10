/*-------------------------------------------------------------------------
 *
 * overflow.c
 *		Common routines for overflow checks
 *
 * Copyright (c) 1996-2019, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *		  overflow/overflow.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "fmgr.h"

#include "common/int.h"
#include "common/string.h"
#include "utils/builtins.h"

typedef enum
{
	NONE = 0,
	INT16,
	INT32,
	INT64,
	UINT16,
	UINT32,
	UINT64
} PGOverflowType;

typedef enum
{
	OPR_NONE = 0,
	OPR_ADD,
	OPR_SUB,
	OPR_MUL
} PGOverflowOpr;

/* smallint functions */
PG_FUNCTION_INFO_V1(pg_overflow_check);

Datum
pg_overflow_check(PG_FUNCTION_ARGS)
{
	int64		v1 = PG_GETARG_INT64(0);
	int64		v2 = PG_GETARG_INT64(1);
	int32		count = PG_GETARG_INT32(2);
	text	   *type_txt = PG_GETARG_TEXT_PP(3);
	text	   *opr_txt = PG_GETARG_TEXT_PP(4);
	bool		result = false;
	PGOverflowType type = NONE;
	PGOverflowOpr opr = OPR_NONE;
	int			i;
	char	   *type_str = text_to_cstring(type_txt);
	char	   *opr_str = text_to_cstring(opr_txt);

	/* extract variable type to work on */
	if (strcmp(type_str, "int16") == 0)
		type = INT16;
	else if (strcmp(type_str, "int32") == 0)
		type = INT32;
	else if (strcmp(type_str, "int64") == 0)
		type = INT32;
	else if (strcmp(type_str, "uint16") == 0)
		type = UINT16;
	else if (strcmp(type_str, "uint32") == 0)
		type = UINT32;
	else if (strcmp(type_str, "uint64") == 0)
		type = UINT64;
	else
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("unsupported overflow type")));

	/* extract variable type to work on */
	if (strcmp(opr_str, "add") == 0)
		opr = OPR_ADD;
	else if (strcmp(opr_str, "sub") == 0)
		opr = OPR_SUB;
	else if (strcmp(opr_str, "mul") == 0)
		opr = OPR_MUL;
	else
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("unsupported overflow operation")));

	switch (type)
	{
		case INT16:
		{
			int16		val1 = (int16) v1;
			int16		val2 = (int16) v2;
			int16		val_res;

			switch (opr)
			{
				case OPR_ADD:
				{
					for (i = 0; i < count; i++)
						result = pg_add_s16_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_SUB:
				{
					for (i = 0; i < count; i++)
						result = pg_sub_s16_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_MUL:
				{
					for (i = 0; i < count; i++)
						result = pg_mul_s16_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_NONE:
					Assert(false);
					break;
			}
			break;
		}
		case INT32:
		{
			int32		val1 = (int32) v1;
			int32		val2 = (int32) v2;
			int32		val_res;

			switch (opr)
			{
				case OPR_ADD:
				{
					for (i = 0; i < count; i++)
						result = pg_add_s32_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_SUB:
				{
					for (i = 0; i < count; i++)
						result = pg_sub_s32_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_MUL:
				{
					for (i = 0; i < count; i++)
						result = pg_mul_s32_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_NONE:
					Assert(false);
					break;
			}
			break;
		}
		case INT64:
		{
			int64		val1 = (int64) v1;
			int64		val2 = (int64) v2;
			int64		val_res;

			switch (opr)
			{
				case OPR_ADD:
				{
					for (i = 0; i < count; i++)
						result = pg_add_s64_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_SUB:
				{
					for (i = 0; i < count; i++)
						result = pg_sub_s64_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_MUL:
				{
					for (i = 0; i < count; i++)
						result = pg_mul_s64_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_NONE:
					Assert(false);
					break;
			}
			break;
		}

		/* unsigned part */
		case UINT16:
		{
			uint16		val1 = (uint16) v1;
			uint16		val2 = (uint16) v2;
			uint16		val_res;

			switch (opr)
			{
				case OPR_ADD:
				{
					for (i = 0; i < count; i++)
						result = pg_add_u16_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_SUB:
				{
					for (i = 0; i < count; i++)
						result = pg_sub_u16_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_MUL:
				{
					for (i = 0; i < count; i++)
						result = pg_mul_u16_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_NONE:
					Assert(false);
					break;
			}
			break;
		}
		case UINT32:
		{
			uint32		val1 = (uint32) v1;
			uint32		val2 = (uint32) v2;
			uint32		val_res;

			switch (opr)
			{
				case OPR_ADD:
				{
					for (i = 0; i < count; i++)
						result = pg_add_u32_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_SUB:
				{
					for (i = 0; i < count; i++)
						result = pg_sub_u32_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_MUL:
				{
					for (i = 0; i < count; i++)
						result = pg_mul_u32_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_NONE:
					Assert(false);
					break;
			}
			break;
		}
		case UINT64:
		{
			uint64		val1 = (uint64) v1;
			uint64		val2 = (uint64) v2;
			uint64		val_res;

			switch (opr)
			{
				case OPR_ADD:
				{
					for (i = 0; i < count; i++)
						result = pg_add_u64_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_SUB:
				{
					for (i = 0; i < count; i++)
						result = pg_sub_u64_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_MUL:
				{
					for (i = 0; i < count; i++)
						result = pg_mul_u64_overflow(val1, val2, &val_res);
					break;
				}
				case OPR_NONE:
					Assert(false);
					break;
			}
			break;
		}

		case NONE:
			Assert(false);
			break;
	}

	PG_RETURN_BOOL(result);
}

/*
 * pg_strtouint64
 *	  Converts 'str' into an unsigned 64-bit integer.
 *
 * This has the identical API to strtoul(3), except that it will handle
 * 64-bit ints even where "long" is narrower than that.
 *
 * For the moment it seems sufficient to assume that the platform has
 * such a function somewhere; let's not roll our own.
 *
 * Note: this is an old implementation from PostgreSQL upstream which
 * has been removed in v13, used here for performance comparison.
 */
static uint64
pg_strtouint64_old(const char *str, char **endptr, int base)
{
#ifdef _MSC_VER                    /* MSVC only */
	return _strtoui64(str, endptr, base);
#elif defined(HAVE_STRTOULL) && SIZEOF_LONG < 8
	return strtoull(str, endptr, base);
#else
	return strtoul(str, endptr, base);
#endif
}


/*
 * Wrapper of pg_strtouint64_old used for performance comparison.
 */
PG_FUNCTION_INFO_V1(pg_strtouint64_old_check);
Datum
pg_strtouint64_old_check(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	int32		count = PG_GETARG_INT32(1);
	char	   *val = text_to_cstring(val_txt);
	uint64		res;
	int			i;

	for (i = 0; i < count; i++)
		res = pg_strtouint64_old(val, NULL, 10);

	PG_RETURN_INT64(res);
}

/*
 * Wrapper of pg_strtouint64 used for performance comparison.
 *
 * This ignores errors.
 */
PG_FUNCTION_INFO_V1(pg_strtouint64_new_check);
Datum
pg_strtouint64_new_check(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	int32		count = PG_GETARG_INT32(1);
	char	   *val = text_to_cstring(val_txt);
	uint64		res;
	int			i;

	/* just don't care about the status result here */
	for (i = 0; i < count; i++)
		(void) pg_strtouint64(val, &res);

	PG_RETURN_INT64(res);
}

/*
 * Wrapper of pg_strtoint() used for performance comparison.
 *
 * Note that this throws an ereport(ERROR) immediately on invalid
 * input or overflow.
 */
PG_FUNCTION_INFO_V1(pg_strtoint32_check_perf);
Datum
pg_strtoint32_check_perf(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	int32		count = PG_GETARG_INT32(1);
	char	   *val = text_to_cstring(val_txt);
	int32		res;
	int			i;

	for (i = 0; i < count; i++)
		res = pg_strtoint32_check(val);

	PG_RETURN_INT32(res);
}

/*
 * Wrapper of pg_strtoint32 used for performance comparison.
 *
 * This ignores errors.
 */
PG_FUNCTION_INFO_V1(pg_strtoint32_perf);
Datum
pg_strtoint32_perf(PG_FUNCTION_ARGS)
{
	text	   *val_txt = PG_GETARG_TEXT_PP(0);
	int32		count = PG_GETARG_INT32(1);
	char	   *val = text_to_cstring(val_txt);
	int32		res;
	int			i;

	/* just don't care about the status result here */
	for (i = 0; i < count; i++)
		(void) pg_strtoint32(val, &res);

	PG_RETURN_INT32(res);
}
