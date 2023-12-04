# Json
A Json by C++

## Introduction
+ A simple Json Operation Module by C++

| Project | Status | Version | Author | Remark |
| -- | -- | -- | -- | -- |
| Json | Done | 1.0 | Matthew | version 1.0 had completed |

## Dependencies
1. [Logger](https://github.com/wangqinghe95/Logger)
2. [FileSystem](https://github.com/wangqinghe95/FilesSystem.git)

## Compile
1. Make sure that two repositories of Dependencies have been downloaded and the structure is as follow
    + FileSystem
    + Json
    + Logger
2. cd Json
3. make
4. ./testJson

## Function
1. Parase string of Json, from reading file or network
2. Generate a Json string only by assignment

## Test
+ Reference [unit-testing](./unittest/unittest.cpp)

## Usage
+ Reference [unit-testing](./unittest/unittest.cpp)

## Todo
1. Format of Json, for example indentation
2. Seralization a json string by Class and so on
3. Memory for JsonMemory
4. The original pointer should be replaced by Smart-Pointer of C++11
