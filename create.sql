drop table if exists users;
drop table if exists word_history;
--drop table if exists dict;
create table users (user_id integer primary key autoincrement,user_name varchar(20),pwd varchar(20),type integer);
create table word_history(user_id varchar(10),word varchar(20),time varchar(20),view_count integer);
--create table dict(word varchar(30),word_definition varchar(200));
insert into users values ('1001','root','123456',1),('1002','杨昕豪','123',2);

