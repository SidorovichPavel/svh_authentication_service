-- Database generated with pgModeler (PostgreSQL Database Modeler).
-- pgModeler version: 1.1.3
-- PostgreSQL version: 14.0
-- Project Site: pgmodeler.io
-- Model Author: ---
-- object: svh | type: ROLE --
-- DROP ROLE IF EXISTS svh;
CREATE ROLE svh WITH 
	SUPERUSER
	CREATEDB
	INHERIT
	LOGIN
	 PASSWORD 'svh123qwe';
-- ddl-end --


-- Database creation must be performed outside a multi lined SQL file. 
-- These commands were put in this file only as a convenience.
-- 
-- object: svh_authentication_service_db | type: DATABASE --
-- DROP DATABASE IF EXISTS svh_authentication_service_db;
CREATE DATABASE svh_authentication_service_db;
-- ddl-end --

-- object: svh_authentication_schema | type: SCHEMA --
-- DROP SCHEMA IF EXISTS svh_authentication_schema CASCADE;
CREATE SCHEMA svh_authentication_schema;
-- ddl-end --
ALTER SCHEMA svh_authentication_schema OWNER TO svh;
-- ddl-end --

SET search_path TO pg_catalog,public,svh_authentication_schema;
-- ddl-end --

-- object: "uuid-ossp" | type: EXTENSION --
-- DROP EXTENSION IF EXISTS "uuid-ossp" CASCADE;
CREATE EXTENSION "uuid-ossp"
WITH SCHEMA svh_authentication_schema;
-- ddl-end --

-- object: svh_authentication_schema.users | type: TABLE --
-- DROP TABLE IF EXISTS svh_authentication_schema.users CASCADE;
CREATE TABLE svh_authentication_schema.users (
	id uuid NOT NULL DEFAULT uuid_generate_v4(),
	first_name text NOT NULL,
	last_name text NOT NULL,
	nickname text NOT NULL,
	age smallint NOT NULL,
	password_hash text NOT NULL,
	salt text NOT NULL,
	CONSTRAINT users_pk PRIMARY KEY (id),
	CONSTRAINT unique_nickname UNIQUE (nickname)
);
-- ddl-end --
ALTER TABLE svh_authentication_schema.users OWNER TO svh;
-- ddl-end --

-- object: svh_authentication_schema.signup_credentials_type | type: TYPE --
-- DROP TYPE IF EXISTS svh_authentication_schema.signup_credentials_type CASCADE;
CREATE TYPE svh_authentication_schema.signup_credentials_type AS
(
 first_name text,
 last_name text,
 age smallint,
 nickname text,
 password_hash text
);
-- ddl-end --

-- object: svh_authentication_schema.permitions | type: TABLE --
-- DROP TABLE IF EXISTS svh_authentication_schema.permitions CASCADE;
CREATE TABLE svh_authentication_schema.permitions (
	id integer NOT NULL,
	title text NOT NULL,
	CONSTRAINT permitions_pk PRIMARY KEY (id)
);
-- ddl-end --
ALTER TABLE svh_authentication_schema.permitions OWNER TO svh;
-- ddl-end --

-- object: svh_authentication_schema.permitions_to_users | type: TABLE --
-- DROP TABLE IF EXISTS svh_authentication_schema.permitions_to_users CASCADE;
CREATE TABLE svh_authentication_schema.permitions_to_users (
	id_users uuid,
	id_permitions integer

);
-- ddl-end --
ALTER TABLE svh_authentication_schema.permitions_to_users OWNER TO svh;
-- ddl-end --

-- object: users_fk | type: CONSTRAINT --
-- ALTER TABLE svh_authentication_schema.permitions_to_users DROP CONSTRAINT IF EXISTS users_fk CASCADE;
ALTER TABLE svh_authentication_schema.permitions_to_users ADD CONSTRAINT users_fk FOREIGN KEY (id_users)
REFERENCES svh_authentication_schema.users (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: permitions_fk | type: CONSTRAINT --
-- ALTER TABLE svh_authentication_schema.permitions_to_users DROP CONSTRAINT IF EXISTS permitions_fk CASCADE;
ALTER TABLE svh_authentication_schema.permitions_to_users ADD CONSTRAINT permitions_fk FOREIGN KEY (id_permitions)
REFERENCES svh_authentication_schema.permitions (id) MATCH FULL
ON DELETE SET NULL ON UPDATE CASCADE;
-- ddl-end --

-- object: svh_authentication_schema.signin_creditions_type | type: TYPE --
-- DROP TYPE IF EXISTS svh_authentication_schema.signin_creditions_type CASCADE;
CREATE TYPE svh_authentication_schema.signin_creditions_type AS
(
 id uuid,
 age smallint,
 nickname text,
 password_hash text,
 salt text
);
-- ddl-end --


