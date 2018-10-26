
AM_CXXFLAGS= -std=c++11
I_POSTGRESQL = -I$(shell pg_config --includedir)

bindir = ${prefix}/bin

cgibindir = ${prefix}/cgi-bin
cgibin_PROGRAMS = \
  cgi-bin/announce_service \
  cgi-bin/login_mapper \
  cgi-bin/login_service \
  cgi-bin/make_apikey \
  cgi-bin/make_servicekey \
  cgi-bin/renew_apikey \
  cgi-bin/revoke_apikey \
  cgi-bin/revoke_servicekey \
  cgi-bin/retrieve_keys

cgi_bin_announce_service_SOURCES = \
  api_calls/announce_service.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc \
  tools/random_key.cc
cgi_bin_announce_service_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_announce_service_LDADD = -lpq

cgi_bin_login_mapper_SOURCES = \
  pages/login_mapper.cc \
  tools/base64.cc \
  tools/cgi_helper.cc \
  tools/curl_client.cc \
  tools/osm_permissions_reader.cc \
  tools/postgresql_wrapper.cc \
  tools/random_key.cc
cgi_bin_login_mapper_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_login_mapper_LDADD = -lpq -lcurl

cgi_bin_login_service_SOURCES = \
  pages/login_service.cc \
  tools/base64.cc \
  tools/cgi_helper.cc \
  tools/curl_client.cc \
  tools/osm_permissions_reader.cc \
  tools/postgresql_wrapper.cc \
  tools/random_key.cc
cgi_bin_login_service_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_login_service_LDADD = -lpq

cgi_bin_make_apikey_SOURCES = \
  api_calls/make_apikey.cc \
  tools/base64.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc \
  tools/random_key.cc
cgi_bin_make_apikey_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_make_apikey_LDADD = -lpq

cgi_bin_make_servicekey_SOURCES = \
  api_calls/make_servicekey.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc \
  tools/random_key.cc
cgi_bin_make_servicekey_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_make_servicekey_LDADD = -lpq

cgi_bin_renew_apikey_SOURCES = \
  api_calls/renew_apikey.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc
cgi_bin_renew_apikey_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_renew_apikey_LDADD = -lpq

cgi_bin_revoke_apikey_SOURCES = \
  api_calls/revoke_apikey.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc
cgi_bin_revoke_apikey_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_revoke_apikey_LDADD = -lpq

cgi_bin_revoke_servicekey_SOURCES = \
  api_calls/revoke_servicekey.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc
cgi_bin_revoke_servicekey_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_revoke_servicekey_LDADD = -lpq

cgi_bin_retrieve_keys_SOURCES = \
  api_calls/retrieve_keys.cc \
  tools/cgi_helper.cc \
  tools/postgresql_wrapper.cc
cgi_bin_retrieve_keys_CPPFLAGS = $(I_POSTGRESQL) $(AM_CPPFLAGS)
cgi_bin_retrieve_keys_LDADD = -lpq

dist_bin_SCRIPTS = \
  bin/init.sh

distdir = osm_api_key_dispenser

nobase_dist_HEADERS = \
  tools/cgi_helper.h \
  tools/postgresql_wrapper.h \
  tools/curl_client.h \
  tools/osm_permissions_reader.h \
  tools/base64.h \
  tools/random_key.h

EXTRA_DIST = \
  html/login_mapper.js \
  html/login_service.js \
  html/mapper.html \
  html/service.html \
  sql/make_testsessions.sql \
  sql/make_testdata.sql \
  sql/make_tables.sql
