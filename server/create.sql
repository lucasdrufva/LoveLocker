create sequence hibernate_sequence start 1 increment 1
create table client (id int8 not null, client_name varchar(255), password varchar(255), type varchar(255), primary key (id))
create table device (id int8 not null, client_id int8, primary key (id))
create table device_users (device_id int8 not null, user_id int8 not null)
create table image (id int8 not null, date timestamp, name varchar(255), primary key (id))
create table image_part (id int8 not null, content oid, image_id int8 not null, primary key (id))
create table lbuser (id int8 not null, client_id int8, primary key (id))
create table status (id int8 not null, date timestamp, notifier int4 not null, preview varchar(255), seen boolean not null, type int4 not null, device_id int8 not null, image_id int8, text_id int8, primary key (id))
create table text (id int8 not null, background_color int4 not null, color int4 not null, size int4 not null, text varchar(255), primary key (id))
create table touch (id int8 not null, date timestamp, size int4 not null, device_id int8 not null, primary key (id))
alter table if exists device add constraint FKdu2w3cqp9s5nydbum1dkl1wcb foreign key (client_id) references client
alter table if exists device_users add constraint FK5as1bsiq33idntjhviw3xqf25 foreign key (user_id) references lbuser
alter table if exists device_users add constraint FKfds0nchd0eupj3cl907ku2rar foreign key (device_id) references device
alter table if exists image_part add constraint FKb20w5ayhgcfgyqurcy2q4r9p1 foreign key (image_id) references image
alter table if exists lbuser add constraint FKmpoer7wjwcu2ubvj9vr70qs12 foreign key (client_id) references client
alter table if exists status add constraint FKbgiq0blnmvsytekap1nu6nnii foreign key (device_id) references device
alter table if exists status add constraint FK121a0cgfqbtsxqpkl4jwmov54 foreign key (image_id) references image
alter table if exists status add constraint FK227q7gtgk7dg0thop21b0wfjw foreign key (text_id) references text
alter table if exists touch add constraint FKrnk5ttyqcsykgdjmwnb2bwv9u foreign key (device_id) references device
