Requirements:
    Operating System:
        - Linux Ubuntu

    Database:
        - MySQL Database Server ( apt package name: mysql-server )

    Server module:
        - C++ 11 or newer
        - MySQL connector-cpp 8.0.29 (https://dev.mysql.com/doc/connector-cpp/8.0/en/)

    Portal module:
        - Raspberry 3B+ with Raspbian OS and root privileges
        - libnfc (https://github.com/nfc-tools/libnfc.git)
    
    Web module:
        - Python 3.8.10
        - Django 3.2.9
        - Python mysqlclient 2.1.0

Building automatically and running server module:
    Change the dbcon arguments to match the database configuration
    Change the build/debug scripts by replacing the path to mysql connector
    Execute a script
    Execute the outcoming binary

Building automatically and running portal module:
    Change the value of SERVER_NAME in main.cpp to the IP of the
    machine that is running the server module
    Execute a build or debug script
    Execute the outcoming binary as root or equivalent

Building automatically and running web module:
    Navigate to the web module folder
    Execute in terminal: python manage.py runserver

Tested with:
    - Operating system: Ubuntu version 20.04.3
    - Database: MySQL version 8.0.28-0ubuntu0.20.04.3 for Linux
    - Compiler: g++ version (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0

*Notes*
1. The server module and the database must be on single pc executing simultaneously
unless configured specifically

2. The Portal module's software has to be compiled on Raspberry Pi 3 B+ with 
Raspbian OS and executed as root or equivalent
