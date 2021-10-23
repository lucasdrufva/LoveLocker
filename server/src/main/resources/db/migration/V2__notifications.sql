create table notification (id int8 not null, date timestamp, message varchar(255), seen boolean, user_id int8 not null, primary key (id), foreign key (user_id) references lbuser);
alter table if exists lbuser add firebase_device_token varchar(255), add notification_id int8, add constraint jdkshfkjsdhfkj foreign key (notification_id) references notification;