delete from user_sessions where expires < now();
insert into user_sessions (user_ref, access_key, expires) values (7, '1234aaaa', now() + interval '5 minutes');
