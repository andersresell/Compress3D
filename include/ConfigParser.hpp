#pragma once
#include "Config.hpp"



template<typename T, size_t N_ROWS>
struct YAML::convert<Eigen::Vector<T, N_ROWS>>{
    static bool decode(const Node& node, Eigen::Vector<T, N_ROWS>& vec) {
        if (!node.IsSequence()) {
            return false;
        }

        assert(vec.cols() == 1);
        if (node.size() != vec.rows()) 
            throw std::runtime_error("The vector " + node.Tag() + " has the wrong length\n"); 

        size_t i{0};
        for (const auto& element : node){
            vec[i++] = element.as<T>();
        }
        
        return true;
    }
};



class ConfigParser{

    YAML::Node root_node;

    const string config_filename;
    
public:
    ConfigParser(const string& config_filename);

    /*Sets member variables in the Config object*/
    
    void parse_config(Config& config);


private:

    // const std::map<string, std::type_index> data_type_map{
    //     {"mesh_filename", std::type_index(typeid(string))},
    //     {"output_basename", std::type_index(typeid(string))},
    //     {"solver", std::type_index(typeid(int))}
    // };

    /*Specifies options that are not specified in the 
    config file, but can be inferred after config file options are set*/

    void parse_yaml_file_options(Config& config);

    void infer_hidden_options(Config& config);

    string option_not_specified_msg(string option_name) const {return option_name + " not specified in the input file " + config_filename;}

    template<typename T>
    T read_required_option(string option_name){
        if (root_node[option_name]){
            return root_node[option_name].as<T>(); 
        } else {
            throw std::runtime_error(option_not_specified_msg(option_name));
        }
    }

    template<typename T>
    T read_optional_option(string option_name, T default_value){
        if (root_node[option_name]){
            return root_node[option_name].as<T>(); 
        } else {
            return default_value;
        }
    }

    template<typename EnumType>
    EnumType read_required_enum_option(string option_name, const map<string, EnumType>& enum_map){
        if (root_node[option_name]){
            return enum_map.at(root_node[option_name].as<string>()); 
        } else {
            throw std::runtime_error(option_not_specified_msg(option_name));
        }
    }

    template<typename EnumType>
    EnumType read_optional_enum_option(string option_name, const map<string, EnumType>& enum_map, EnumType default_value){
        if (root_node[option_name]){
            return enum_map.at(root_node[option_name].as<string>()); 
        } else {
            return default_value;
        }
    }

    void read_patches(Config& config);

};