# Bookstore

ACMCLASS 吴硕

## 	程序功能概述

1. 账户系统，对账户的注册、登录、登出、修改密码、删除用户、添加账户，允许同时登录多个账号，以及不同的账户对应不同的权限。
2. 图书系统：对每一种书的ISBN、名字。作者、数量、价格、存量进行储存，对于图书的检索、购买、选择、修改信息、进货。
3. 日志系统：存储交易的次数、成交金额，并可以生成日志。

## 	主体逻辑说明

1. 账户信息的储存、管理：使用账户类，利用分块链表，存储每一个用户的数据；对于账户的登录登出，使用一个stack储存当前登录的用户的ID以及选中的书的ISBN，登录就是向stack中添加，对账户进行操作、买书等就是取出stack的账户ID并于对应的文件中检索相关信息并更改。
2. 图书信息的储存：首先是使用一个class存储图书的所有信息，这是图书信息的主储存，且排序是按照ISBN排序。然后根据keyword,bookname,author分别进行储存，每个只储存相应的信息与对应的ISBN，方便之后检索。
3. 图书信息管理：图书的购买、信息更改都是在图书的总数据中查询ISBN，删除原数据，修改，再插入。图书的查询则在对应的文件中查到对应的ISBN再在总数据中查到图书的所有数据
4. 日志系统：日志包括两个类，一个是财务类，记录图书的ISBN，单价，数量，购入/售出，另一个是操作类，记录操作的id、等级、操作、对象

## 	类的接口与说明

1. user：账户信息类，存储账户的ID（string）、密码(string)、等级（int）。
2. books：图书信息类，存储图书相关的ISBN（string）、名字（string）、作者（string）、数量（int）、价格（double）、关键词（string）。
3. deal：交易操作类，图书的ISBN(string)，单价(double)，数量(int)，购入/售出(bool)。
4. system_operation：系统操作类，记录操作的id（string）、等级(int)、操作(string)、对象(string)。

## 文件存储说明

将所有数据存储为七份文件份文件，其中图书类四个文件，用户类一个文件，系统类两个文件，均为二进制存储。



编辑说明：

​	2022.12.5   v1.0 , 初步编写。

​	2022.12.31 V1.1 具体讲述了如何实现数据的储存，以及日志类的实现