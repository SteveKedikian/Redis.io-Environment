#include "../header/redis.h"

#include <iostream>
#include <filesystem>
#include <fstream>

const std::string skt::Redis::m_commands[COMMANDS_COUNT] = {"exit", "connect", "newp", "set", "get", "del"};

const std::string skt::Redis::m_main_folder_name = "RedisEnvironment";
const std::string skt::Redis::m_client_ports_folder_path = "client_ports";
const std::string skt::Redis::m_server_folder_path = "server";
const std::string skt::Redis::m_server_port_num_folder_path = "server/port_";
const std::string skt::Redis::m_server_port_lists_folder = "lists";

const std::string skt::Redis::m_client_data_txt_path = "client_ports/client_data.txt";
const std::string skt::Redis::m_server_port_key_val_file_data_txt = "key_val.txt";

const std::string skt::Redis::m_data_file_end_symbol = "(*)";

skt::Redis::Redis()
{}

///////////////////////////////////////////////////////////// HELPER FUNCTIONS /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool skt::Redis::is_letter_symbol(const char symbol) {
    if ((symbol >= CAPITAL_A && symbol <= CAPITAL_Z) ||
        (symbol >= SMALL_a && symbol <= SMALL_z))
    {
        return true;
    }
    return false;
}

bool skt::Redis::make_lower_case(std::string& line) {
    for (int i = 0; i < line.length(); ++i) {
        if (!is_letter_symbol(line[i])) {
            return false;
        }

        if (line[i] >= CAPITAL_A && line[i] <= CAPITAL_Z) {
            line[i] = line[i] + CHANGE_SYMBOL; // Making lower case
        }
    }
    return true;
    std::cout << line << "\n";
}

bool skt::Redis::is_number(const std::string& line) {
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] < ZERO || line[i] > NINE) {
            return false;
        }
    }
    return true;
}

std::string skt::Redis::return_token_of_line_and_advance_position(const std::string& line, int& index) {
    std::string token = "";
    for (; index < line.length() && line[index] != ' '; ++index) {
        token += line[index];
    }
    ++index; // to skip space for next time
    return token;
}

///////////////////////////////////////////////////////////// PROGRAM FUNCTIONS /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int skt::Redis::command_exists(std::string& line) {
    if (line.length() > MAX_COMMAND_SIZE || !make_lower_case(line)) {
        return ERROR;
    }
    
    if (line == m_commands[EXIT]) {
        return EXIT;
    }
    else if (line == m_commands[CONNECT]) {
        return CONNECT;
    }
    else if (line == m_commands[NEWP]) {
        return NEWP;
    }
    else if (line == m_commands[SET]) {
        return SET;
    }
    else if (line == m_commands[GET]) {
        return GET;
    }
    else if (line == m_commands[DEL]) {
        return DEL;
    }
    return ERROR;
}

bool skt::Redis::check_and_create_folders() {
    if (!std::filesystem::exists(m_client_ports_folder_path)) {
        if (!std::filesystem::create_directory(m_client_ports_folder_path)) {
            std::cout << "\nError while finding or creating '/" + m_client_ports_folder_path + 
                         "' folder in the program " + m_main_folder_name + " directory !";
            return false;
        }
        std::cout << "\nfolder '" + m_client_ports_folder_path + 
                     "' was created in the program " + m_main_folder_name + " directory.";
    }

    if (!std::filesystem::exists(m_server_folder_path)) {
        if (!std::filesystem::create_directory(m_server_folder_path)) {
            std::cout << "\nError while finding or creating '/" + m_server_folder_path + 
                         "' folder in the program " + m_main_folder_name + " directory !";
            return false;
        }
        std::cout << "\nfolder '" + m_server_folder_path + 
                     "' was created in the program " + m_main_folder_name + " directory.";
    }

    return true;
}

void skt::Redis::exec() {
    std::string line;
    int command_num;

    if (!check_and_create_folders()) {
        return;
    }

    std::cout << std::endl;
    std::cout << "\nRedis srtarted !\n";

    while (true) {
        std::cout << "\nConnect_Com: ";
        std::cin >> line;
        switch (command_exists(line))
        {
        case ERROR: // Error try again
            std::cout << "\nWrong Command !\n";
            break;
        
        case EXIT: // Close the program
            return;

        case CONNECT: // Connecting to the port
            connect();
            break;

        default: // Error try again
            std::cout << "\nWrong Command !\n";
            break;
        }
    }
}

bool skt::Redis::check_and_create_client_ports_data_files() {
    std::ifstream fin; // read file
    fin.open(m_client_data_txt_path);

    if (!fin.is_open()) {
        fin.close();
        std::ofstream fout; // write file
        fout.open(m_client_data_txt_path);
        fout << NO_DATA << "\n" << m_data_file_end_symbol;
        fout.close();
    }
    else {
        fin.close();
        return true;
    }

    fin.open(m_client_data_txt_path); // try to open the file again
    if (!fin.is_open()) {
        std::cout << "\nError while finding or creating '/" + m_client_data_txt_path + 
                         "' file in the program " + m_main_folder_name + " directory !";
        fin.close();
        return false;
    }
    fin.close();
    
    std::cout << "\nfile '" + m_client_data_txt_path + "' was created !";
    return true;
}

void skt::Redis::add_client_ports(std::vector<std::string>& client_ports) {
    std::ifstream fin; // read
    std::string token;
    int client_count;
    int index = 0;

    fin.open(m_client_data_txt_path);
    fin >> token;
    client_count = std::stoi(token);

    if (NO_DATA == client_count) {
        return;
    }

    client_ports.resize(client_count);

    while (index < client_count && !fin.eof() && token != m_data_file_end_symbol) {
        fin >> token;
        client_ports[index] = token;
        ++index;
    }
}

bool skt::Redis::find_client_port(const std::vector<std::string>& client_ports, const std::string& port) {
    for (int i = 0; i < client_ports.size(); ++i) {
        if (client_ports[i] == port) {
            return true;
        }
    }
    return false;
}

std::string skt::Redis::create_and_return_client_port(std::vector<std::string>& client_ports) {
    std::ofstream fout; // write
    std::string token;
    std::string new_client_port = std::to_string(REDIS_PORT) + std::to_string(client_ports.size() + 1); // +1 for new code for new port

    client_ports.push_back(new_client_port);
    fout.open(m_client_data_txt_path);
    
    fout << client_ports.size() << "\n"; // add client count first
    for (int i = 0; i < client_ports.size(); ++i) {
        fout << client_ports[i] << "\n"; // refresh client data file
    }
    fout << m_data_file_end_symbol; // end symbol

    return new_client_port;
}

void skt::Redis::connect() {
    check_and_create_client_ports_data_files();

    std::vector<std::string> client_ports;
    std::string line;
    int client_count;

    add_client_ports(client_ports);
    
    while (true) {
        std::cout << "\nPort: ";
        std::cin >> line;
        if (is_number(line)) { // find client
            if (find_client_port(client_ports, line)) {
                connect_to_port(line);
                continue;
            }
        }
        else if (NEWP == command_exists(line)) { // create client
            std::cout << "\nNew client port created: " << create_and_return_client_port(client_ports) << "\n";
            continue;
        }
        else if (EXIT == command_exists(line)) { // exit connection
            return;
        }
        std::cout << "\nWrong Port or Command !\n";
    }
}

bool skt::Redis::check_and_create_connect_port_folder_files(const std::string& port_path) {
    std::ifstream fin; // read
    std::string path_to_key_val_file = port_path + "/" + m_server_port_key_val_file_data_txt;
    std::string path_to_lists_folder = port_path + "/" + m_server_port_lists_folder;
    // hash

    fin.open(path_to_key_val_file);

    if (!fin.is_open()) {
        std::ofstream fout; // write
        fout.open(path_to_key_val_file);
        fout << NO_DATA << "\n" << m_data_file_end_symbol;
        fout.close();
    }
    fin.close();

    if (!std::filesystem::exists(path_to_lists_folder)) {
        if (!std::filesystem::create_directory(path_to_lists_folder)) {
            std::cout << "\nError while finding or creating '/" + path_to_lists_folder + 
                         "' folder in the program " + m_main_folder_name + " directory !";
            return false;
        }
    }

    // hash

    return true;
}

bool skt::Redis::check_and_create_connected_port_folder(const std::string& port_path) {
    if (!std::filesystem::exists(port_path)) {
        if (!std::filesystem::create_directory(port_path)) {
            std::cout << "\nError while finding or creating '/" + port_path + 
                         "' folder in the program " + m_main_folder_name + " directory !";
            return false;
        }
        check_and_create_connect_port_folder_files(port_path);
        std::cout << "\nfolder '" + port_path + 
                     "' with data files was created in the program " + m_main_folder_name + " directory.";
    }
    return true;
}

bool skt::Redis::connect_to_port(const std::string& port) {
    std::string port_code_path = m_server_port_num_folder_path + port;
    if (!check_and_create_connected_port_folder(port_code_path)) {
        std::cout << "\nConnecting to the port failed !\n";
        return false;
    }
    redis_running(port, port_code_path);
    return true;
}

void skt::Redis::read_key_value_data_txt(std::unordered_map<std::string, std::string>& key_val_data, const std::string& port_path) {
    std::ifstream fin;
    std::string key_val_path = port_path + "/" + m_server_port_key_val_file_data_txt;
    std::string token;
    std::string key;
    std::string val;
    int data_count;
    int index = 0;

    fin.open(key_val_path);
    fin >> token;
    data_count = std::stoi(token);

    while (index < data_count && !fin.eof() && token != m_data_file_end_symbol) {
        fin >> token;
        key = token;
        fin >> token;
        val = token;
        key_val_data[key] = val;
        ++index;
    }
    fin.close();
}

void skt::Redis::set_key_value(std::unordered_map<std::string, std::string>& key_val_data, const std::string& key, const std::string& value) {
    if (key.empty() || value.empty()) {
        std::cout << "\nKey/Value must not be empty !\n";
        return;
    }
    key_val_data[key] = value;
}

std::string skt::Redis::get_key_value(std::unordered_map<std::string, std::string>& key_val_data, const std::string& key) {
    if (key_val_data.find(key) == key_val_data.end()) {
        return ""; // empty
    }
    return key_val_data[key];
}

void skt::Redis::del_key_value(std::unordered_map<std::string, std::string>& key_val_data, const std::string& key) {
    key_val_data.erase(key);
}

void skt::Redis::save_key_value_data_txt(const std::unordered_map<std::string, std::string>& key_val_data, const std::string& port) {
    std::ofstream fout; // write
    std::string key_val_file_path = m_server_port_num_folder_path + port + "/" + m_server_port_key_val_file_data_txt;
    fout.open(key_val_file_path);

    fout << key_val_data.size() << "\n";
    for (auto it : key_val_data) {
        fout << it.first << " " << it.second << "\n";
    }
    fout << m_data_file_end_symbol;
    fout.close();
}

void skt::Redis::save_data(const std::unordered_map<std::string, std::string>& key_val_data, const std::string& port) {
    save_key_value_data_txt(key_val_data, port);
}

void skt::Redis::redis_running(const std::string& port, const std::string& port_path) {
    std::unordered_map<std::string, std::string> key_val_data;
    std::unordered_map<std::string, std::list<std::string>> lists_data;
    read_key_value_data_txt(key_val_data, port_path);
    std::cout << std::endl;

    while (true) {
        std::string line;
        std::string result; // Will be used to show/get data
        int index = 0;
        
        std::cout << "Port::" + port + "> ";
        std::cin.sync();
        std::getline(std::cin, line);

        std::string command = return_token_of_line_and_advance_position(line, index); // first token 'command'
        std::string key = return_token_of_line_and_advance_position(line, index); // second token 'key'
        std::string value = return_token_of_line_and_advance_position(line, index); // third token 'value'
        /*
            tokens can be added here
        */

        switch (command_exists(command))
        {
        case ERROR: // Error try again
            std::cout << "\nWrong Command !\n";
            break;
        
        case EXIT: // Save and Exit port
            save_data(key_val_data, port);
            return;

        case SET: // Set key value
            set_key_value(key_val_data, key, value);
            std::cout << "OK\n";
            break;
        
        case GET: // Get key
            result = get_key_value(key_val_data, key);
            if (result.empty()) {
                std::cout << "(nil)\n";
            }
            else {
                std::cout << "'" + result + "'\n";
            }
            break;

        case DEL: // Del key
            del_key_value(key_val_data, key);
            break;

        default: // Error try again
            std::cout << "\nWrong Command !\n";
            break;
        }
    }
}