/* deparse/deparse--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION deparse" to load this file. \quit

CREATE FUNCTION deparse(pg_ddl_command)
  RETURNS text IMMUTABLE STRICT
  AS 'MODULE_PATHNAME' LANGUAGE C;

CREATE FUNCTION deparse_trigger_data()
  RETURNS event_trigger
  AS 'MODULE_PATHNAME' LANGUAGE C;