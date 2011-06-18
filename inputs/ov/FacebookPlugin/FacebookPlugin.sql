BEGIN TRANSACTION;
CREATE TABLE FANPAGEFRIENDLIST(
	id int NOT NULL ,
	fanpage int NOT NULL ,
	friend int NOT NULL ,
	PRIMARY KEY(id,fanpage,friend)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE FANPAGEPOSTLIST(
	id int NOT NULL ,
	fanpage int NOT NULL ,
	post int NOT NULL ,
	PRIMARY KEY(id,fanpage,post)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE FANPAGEPOST(
	id int NOT NULL ,
	post_id VARCHAR(255) ,
	user_id VARCHAR(255) ,
	time dateTime ,
	message VARCHAR(255) ,
	PRIMARY KEY(id)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE FANPAGE(
	id int NOT NULL ,
	page_id VARCHAR(255) ,
	name VARCHAR(255) ,
	picture VARCHAR(255) ,
	link VARCHAR(255) ,
	category VARCHAR(255) ,
	starring VARCHAR(255) ,
	directed_by VARCHAR(255) ,
	plot_outline VARCHAR(255) ,
	fan_count VARCHAR(255) ,
	userList int ,
	friendList int ,
	postList int ,
	PRIMARY KEY(id)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE FANPAGEUSERLIST(
	id int NOT NULL ,
	fanpage int NOT NULL ,
	user int NOT NULL ,
	PRIMARY KEY(id,fanpage,user)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE FRIEND(
	id int NOT NULL ,
	user_id VARCHAR(255) ,
	name VARCHAR(255) ,
	picture VARCHAR(255) ,
	PRIMARY KEY(id)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE USERFEEDPOSTLIST(
	id int NOT NULL ,
	user int NOT NULL ,
	post int NOT NULL ,
	PRIMARY KEY(id,user,post)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE USERFEEDPOST(
	id int NOT NULL ,
	post_id VARCHAR(255) ,
	user_id VARCHAR(255) ,
	time dateTime ,
	message VARCHAR(255) ,
	PRIMARY KEY(id)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE USERFRIENDLIST(
	id int NOT NULL ,
	user int NOT NULL ,
	friend int NOT NULL ,
	PRIMARY KEY(id,user,friend)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE USER(
	id int NOT NULL ,
	user_id VARCHAR(255) ,
	name VARCHAR(255) ,
	email VARCHAR(255) ,
	picture VARCHAR(255) ,
	access_token VARCHAR(255) ,
	friendList int ,
	userfeedpostList int ,
	PRIMARY KEY(id)
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE CONTENT
(
        id int,
        content_id VARCHAR(255) collate nocase,
        type VARCHAR(255) collate nocase,
        title VARCHAR(255) collate nocase,
        PRIMARY KEY(id)
);
CREATE TABLE CONTENTFANPAGELIST
(
        Content int,
        fanpage int,
        PRIMARY KEY(Content, fanpage)
);
COMMIT;
