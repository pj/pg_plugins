MODULES = deparse

EXTENSION = deparse
DATA = deparse--1.0.sql
PGFILEDESC = "deparse - deparse various kinds of statements"

REGRESS = basic
REGRESS_OPTS = --temp-config=./logical.conf

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
