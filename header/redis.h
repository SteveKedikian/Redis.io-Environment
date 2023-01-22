#ifndef REDIS_H
#define REDIS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>

enum {
    // Size limits
    NO_DATA = 0,
    COMMANDS_COUNT = 6,
    MAX_COMMAND_SIZE = 10,
    MAX_PORT_SIZE = 20,
    CHANGE_SYMBOL = 32,

    // Commands nums
    ERROR = 404,
    REDIS_PORT = 420, // Can be added more ports as other servers in features
    EXIT = 0,
    CONNECT = 1,
    NEWP = 2,
    SET = 3,
    GET = 4,
    DEL = 5,

    // ASCCI symbols
    ZERO = 48,
    NINE = 57,
    CAPITAL_A = 65,
    CAPITAL_Z = 90,
    SMALL_a = 97,
    SMALL_z = 122
};

namespace skt {

class Redis
{
public:
    Redis();

public:
    void exec();

private: // helper functions
    bool is_letter_symbol(const char symbol);
    bool make_lower_case(std::string& line); // makes all line letters lowercase and returns true, if there is symbol which is not a letter, returns false
    bool is_number(const std::string& line);
    std::string return_token_of_line_and_advance_position(const std::string& line, int& index); // will return first token and also increasing index

private: // program functions
    bool check_and_create_folders(); // returns true if checked/created successfully, false if there was an error
    int command_exists(std::string& line); // returns value of the command, if command doesn't exists, returns ERROR
    bool check_and_create_client_ports_data_files(); // returns true if checked/created successfully, false if there was an error
    void add_client_ports(std::vector<std::string>& client_ports); // adds to client_ports port numbers from 'client_ports/client_data.txt' data file
    bool find_client_port(const std::vector<std::string>& client_ports, const std::string& port); // returns true if client exists
    std::string create_and_return_client_port(std::vector<std::string>& client_ports);
    void connect();
    bool check_and_create_connect_port_folder_files(const std::string& port_path);
    bool check_and_create_connected_port_folder(const std::string& port_path);
    bool connect_to_port(const std::string& port);
    void read_key_value_data_txt(std::unordered_map<std::string, std::string>& key_val_data, const std::string& port_path);
    
    void set_key_value(std::unordered_map<std::string, std::string>& key_val_data, const std::string& key, const std::string& value);
    std::string get_key_value(std::unordered_map<std::string, std::string>& key_val_data, const std::string& key);
    void del_key_value(std::unordered_map<std::string, std::string>& key_val_data, const std::string& key);

    void save_data(const std::unordered_map<std::string, std::string>& key_val_data, const std::string& port); // <-- must add LIST and HASH
    void save_key_value_data_txt(const std::unordered_map<std::string, std::string>& key_val_data, const std::string& port);

    void redis_running(const std::string& port, const std::string& port_path);
     

private:
    static const std::string m_commands[COMMANDS_COUNT]; // Commands

    static const std::string m_main_folder_name; // 'RedisEnvironment'
    static const std::string m_client_ports_folder_path; // '/client_ports'
    static const std::string m_server_folder_path; // '/server'
    static const std::string m_server_port_num_folder_path; // '/server/port_' + 'port number'
    static const std::string m_server_port_lists_folder; // '/server/port_' + 'port number' + '/lists'

    static const std::string m_client_data_txt_path; // '/client_ports/client_data.txt'
    static const std::string m_server_port_key_val_file_data_txt; // '/server/port_' + 'port number' + '/key_val.txt'

    static const std::string m_data_file_end_symbol; // (*) means that it is the end of the file
}; // class - Redis

}; // namespace - skt

#endif // REDIS_H