insert into users (id, name) values (6, 'alice');
insert into users (id, name) values (7, 'bob');

insert into services (id, user_ref, uri, created, access_key) values (1, 6, 'example.com', now(), 'abcd0123');

insert into key_events (id, happened) values (1, now());
insert into key_events (id, happened) values (2, now());
insert into key_events (id, happened) values (3, now());

insert into keys (created_ref, service_ref, user_ref, data, expires) values (1, 1, 7, 'abcd0000', now() + interval '30 days');
update keys set revoked_ref = 2 where created_ref = 1;
insert into keys (created_ref, service_ref, user_ref, data, expires) values (3, 1, 7, 'abcd1111', now() + interval '30 days');
