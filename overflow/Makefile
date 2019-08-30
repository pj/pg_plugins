MODULE_big = overflow

EXTENSION = overflow
OBJS = int.c uint.c $(WIN32RES)
DATA = overflow--1.0.sql
PGFILEDESC = "overflow - checks for overflows"

REGRESS = overflow

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
