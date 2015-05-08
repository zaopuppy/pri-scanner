#!/bin/bash

c++ -g http.cc -I../libs/include -L../libs/lib -levent && ./a.out http://www.baidu.com/

