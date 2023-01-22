////////////////////////// REDIS ENVIRONMENT //////////////////////////

It's a "Redis.io" but offline



////////////////////////// REDIS START //////////////////////////

Program will show line 'Connect_com:' where user must write a command.

Commands:
exit - exit current place to previous

connect - connect to the server



////////////////////////// CONNECTING SERVER //////////////////////////

After writing 'connect' program will show line 'Port:' where user must write an existing port or use other commands.
Port number must contain only numbers.

Commands:
exit - exit current place to previous

newp - create new port for the user


////////////////////////// SERVER PORT //////////////////////////

If port exists, connect to the working place of the server by given port, after that user can use commands to work with data.

Commands:
exit - save and exit

set - create or change data. Structure: COMMAND KEY VALUE
                    example: > SET f1 car
                             OK
                             > SET f2 another_car (please notice, that for spaces you must use '_' underscore)
                             OK

get - get value by given key, if it doesn't exists, returns (nil). Structure: COMMAND KEY
                    example: > GET f1
                             'car'
                             > GET f2
                             'another_car'

del - delete existing value by given key. Structure: COMMAND KEY
                    example: > DEL f1 (deletes 'car')
                             > GET f1
                             (nil)



////////////////////////// FEATURES FOR FUTURE //////////////////////////

Commands:
LPUSH, RPUSH, LLEN
HSET, HGET