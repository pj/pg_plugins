SELECT * FROM pg_create_logical_replication_slot('test_deparse', 'decoder_raw');

CREATE EXTENSION deparse;

CREATE OR REPLACE FUNCTION test_deparse()
  RETURNS event_trigger LANGUAGE plpgsql AS
$$
DECLARE
  r record;
  ddl text;
BEGIN
  FOR r IN SELECT * FROM pg_event_trigger_ddl_commands()
  LOOP
    -- verify that tags match
    ddl = deparse(r.command);
    RAISE NOTICE 'tag % ddl %', r.command_tag, ddl;
  END LOOP;
END;
$$;

CREATE EVENT TRIGGER test_deparse_trigger
ON ddl_command_end EXECUTE PROCEDURE deparse_trigger_data();

CREATE TABLE test (field1 serial primary key, field2 text NOT NULL);
INSERT INTO test (field2) VALUES ('asdf'), ('qwer');
SELECT * FROM pg_logical_slot_get_changes('test_deparse', NULL, NULL);