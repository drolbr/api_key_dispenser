#!/usr/bin/env bash

EXECDIR=`dirname $0`

dropdb --if-exists osm_apikey
createdb osm_apikey

pushd $EXECDIR

psql osm_apikey <make_tables.sql
#psql osm_apikey <make_testdata.sql
#psql osm_apikey <make_testsessions.sql

popd
