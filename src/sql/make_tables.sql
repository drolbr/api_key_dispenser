create table users (
    id bigint primary key,
    name varchar(256)
    );
grant all on users to public;

create table user_sessions (
    user_ref bigint,
    access_key varchar(40),
    expires timestamp,
    constraint fk_us_user_ref foreign key (user_ref) references users (id)
    );
grant all on user_sessions to public;

create table services (
    id bigint primary key,
    user_ref bigint,
    uri varchar(256),
    created timestamp,
    access_key varchar(40) null,
    constraint fk_srv_user_ref foreign key (user_ref) references users (id)
    );
grant all on services to public;

create table key_events (
    id bigint primary key,
    happened timestamp
    );
grant all on key_events to public;

create table keys (
    created_ref bigint primary key,
    revoked_ref bigint null,
    service_ref bigint,
    user_ref bigint,
    data varchar(40) unique,
    constraint fk_keys_created_ref foreign key (created_ref) references key_events (id),
    constraint fk_keys_revoked_ref foreign key (revoked_ref) references key_events (id),
    constraint fk_keys_user_ref foreign key (user_ref) references users (id),
    constraint fk_keys_service_ref foreign key (service_ref) references services (id)
    );
grant all on keys to public;

create table oauth (
    oauth_token varchar(40) primary key,
    oauth_token_secret varchar(40),
    oauth_verfier varchar(40)
    );
grant all on oauth to public;
