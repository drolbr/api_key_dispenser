
AM_CXXFLAGS= -std=c++11

cgibindir = ${prefix}/cgi-bin
cgibin_PROGRAMS = cgi-bin/announce_service cgi-bin/login_mapper cgi-bin/login_service cgi-bin/make_apikey cgi-bin/make_servicekey cgi-bin/renew_apikey cgi-bin/revoke_apikey cgi-bin/revoke_servicekey cgi-bin/retrieve_keys

cgi_bin_announce_service_SOURCES = cgi_helper.cc postgresql_wrapper.cc announce_service.cc random_key.cc
cgi_bin_announce_service_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_announce_service_LDADD = -lpq

cgi_bin_login_mapper_SOURCES = base64.cc cgi_helper.cc curl_client.cc postgresql_wrapper.cc login_mapper.cc osm_permissions_reader.cc random_key.cc
cgi_bin_login_mapper_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_login_mapper_LDADD = -lpq -lcurl

cgi_bin_login_service_SOURCES = base64.cc cgi_helper.cc curl_client.cc postgresql_wrapper.cc login_service.cc osm_permissions_reader.cc random_key.cc
cgi_bin_login_service_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_login_service_LDADD = -lpq

cgi_bin_make_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc make_apikey.cc random_key.cc
cgi_bin_make_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_make_apikey_LDADD = -lpq

cgi_bin_make_servicekey_SOURCES = cgi_helper.cc postgresql_wrapper.cc make_servicekey.cc random_key.cc
cgi_bin_make_servicekey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_make_servicekey_LDADD = -lpq

cgi_bin_renew_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc renew_apikey.cc
cgi_bin_renew_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_renew_apikey_LDADD = -lpq

cgi_bin_revoke_apikey_SOURCES = cgi_helper.cc postgresql_wrapper.cc revoke_apikey.cc
cgi_bin_revoke_apikey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_revoke_apikey_LDADD = -lpq

cgi_bin_revoke_servicekey_SOURCES = cgi_helper.cc postgresql_wrapper.cc revoke_servicekey.cc
cgi_bin_revoke_servicekey_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_revoke_servicekey_LDADD = -lpq

cgi_bin_retrieve_keys_SOURCES = cgi_helper.cc postgresql_wrapper.cc retrieve_keys.cc
cgi_bin_retrieve_keys_CPPFLAGS = -I/usr/include/postgresql/ $(AM_CPPFLAGS)
cgi_bin_retrieve_keys_LDADD = -lpq
