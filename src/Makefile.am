
AM_CXXFLAGS= -std=c++11

bin_PROGRAMS = renew_apikey revoke_apikey retrieve_keys

renew_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc renew_apikey.cc
renew_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
renew_apikey_LDADD = -lpq

retrieve_keys_SOURCES = cgi_helper.cc postgresql_wrapper.cc retrieve_keys.cc
retrieve_keys_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
retrieve_keys_LDADD = -lpq

revoke_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc revoke_apikey.cc
revoke_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
revoke_apikey_LDADD = -lpq
