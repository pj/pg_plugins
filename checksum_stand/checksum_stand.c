#include <stdio.h>

#define BLCKSZ 8192
#define N_SUMS 32
#define FNV_PRIME 16777619

typedef struct ItemIdData
{
	unsigned    lp_off:15,      /* offset to tuple (from start of page) */
		lp_flags:2,     /* state of line pointer, see below */
		lp_len:15;      /* byte length of tuple */
} ItemIdData;

typedef struct PageHeaderData
{
	/* XXX LSN is member of *any* block, not only page-organized ones */
	uint64 pd_lsn;      /* LSN: next byte after last byte of xlog
                                 * record for last change to this page */
	uint16      pd_checksum;    /* checksum */
	uint16      pd_flags;       /* flag bits, see below */
	uint16 pd_lower;     /* offset to start of free space */
	uint16 pd_upper;     /* offset to end of free space */
	uint16 pd_special;   /* offset to start of special space */
	uint16      pd_pagesize_version;
	uint32 pd_prune_xid; /* oldest prunable XID, or zero if none */
	ItemIdData  pd_linp[FLEXIBLE_ARRAY_MEMBER]; /* line pointer array */
} PageHeaderData;

typedef union
{
	PageHeaderData phdr;
	uint32		data[BLCKSZ / (sizeof(uint32) * N_SUMS)][N_SUMS];
} PGChecksummablePage;

static const uint32 checksumBaseOffsets[N_SUMS] = {
	0x5B1F36E9, 0xB8525960, 0x02AB50AA, 0x1DE66D2A,
	0x79FF467A, 0x9BB9F8A3, 0x217E7CD2, 0x83E13D2C,
	0xF8D4474F, 0xE39EB970, 0x42C6AE16, 0x993216FA,
	0x7B093B5D, 0x98DAFF3C, 0xF718902A, 0x0B1C9CDB,
	0xE58F764B, 0x187636BC, 0x5D7B3BB1, 0xE73DE7DE,
	0x92BEC979, 0xCCA6C0B2, 0x304A0979, 0x85AA43D4,
	0x783125BB, 0x6CA8EAA2, 0xE407EAC6, 0x4B5CFC3E,
	0x9FBF8C76, 0x15CA20BE, 0xF2CA9FD3, 0x959BD756
};

#define CHECKSUM_COMP(checksum, value) \
do { \
	uint32 __tmp = (checksum) ^ (value); \
	(checksum) = __tmp * FNV_PRIME ^ (__tmp >> 17); \
} while (0)

/*
 * Block checksum algorithm.  The page must be adequately aligned
 * (at least on 4-byte boundary).
 */
static uint32
pg_checksum_block(const PGChecksummablePage *page)
{
	uint32		sums[N_SUMS];
	uint32		result = 0;
	uint32		i,
				j;

	/* ensure that the size is compatible with the algorithm */
	Assert(sizeof(PGChecksummablePage) == BLCKSZ);

	/* initialize partial checksums to their corresponding offsets */
	memcpy(sums, checksumBaseOffsets, sizeof(checksumBaseOffsets));

	/* main checksum calculation */
	for (i = 0; i < (uint32) (BLCKSZ / (sizeof(uint32) * N_SUMS)); i++)
		for (j = 0; j < N_SUMS; j++)
			CHECKSUM_COMP(sums[j], page->data[i][j]);

	/* finally add in two rounds of zeroes for additional mixing */
	for (i = 0; i < 2; i++)
		for (j = 0; j < N_SUMS; j++)
			CHECKSUM_COMP(sums[j], 0);

	/* xor fold partial checksums together */
	for (i = 0; i < N_SUMS; i++)
		result ^= sums[i];

	return result;
}

uint16
checksum_page(char *page, BlockNumber blkno)
{
	PGChecksummablePage *cpage = (PGChecksummablePage *) page;
	uint16		save_checksum;
	uint32		checksum;

	/* We only calculate the checksum for properly-initialized pages */
	Assert(!PageIsNew(&cpage->phdr));

	/*
	 * Save pd_checksum and temporarily set it to zero, so that the checksum
	 * calculation isn't affected by the old checksum stored on the page.
	 * Restore it after, because actually updating the checksum is NOT part of
	 * the API of this function.
	 */
	save_checksum = cpage->phdr.pd_checksum;
	cpage->phdr.pd_checksum = 0;
	checksum = pg_checksum_block(cpage);
	cpage->phdr.pd_checksum = save_checksum;

	/* Mix in the block number to detect transposed pages */
	checksum ^= blkno;

	/*
	 * Reduce to a uint16 (to fit in the pd_checksum field) with an offset of
	 * one. That avoids checksums of zero, which seems like a good idea.
	 */
	return (uint16) ((checksum % 65535) + 1);
}

int
main()
{
	exit(0);
}
