//
// Created by bytedance on 2020/6/17.
//

#ifndef BMF_MODS_PARAM_H
#define BMF_MODS_PARAM_H

#include <string>
#include "bmf_common.h"
namespace bmf_mods_lite{

    /** @ingroup CppMdSDK
        */
    class ParamImpl;
    class BMF_MODS_EXPORT Param {
        public:
        /** @brief Param struct
            */
        Param();

        /** @brief judge the json has key
         *  @param name name of key
         *  @return true for has, false for not have
            */
        bool has_key(std::string name);

        /** @brief erase the key content from json param
         *  @param name name of key
         *  @return 0 for success, else for failed
            */
        int erase(std::string name);

        /** @brief get string according to the key name
         *  @param name name of key
         *  @param result result of string
         *  @return 0 for success, else for failed
            */
        int get_string(std::string name, std::string &result);
        int set_string(std::string name, std::string result);

        /** @brief get string list according to the key name
         *  @param name name of key
         *  @param result result of string list
         *  @return 0 for success, else for failed
            */
        int get_string_list(std::string name, std::vector<std::string> &result);
        int set_string_list(std::string name, std::vector<std::string> result);

        /** @brief get int according to the key name
         *  @param name name of key
         *  @param result result of int
         *  @return 0 for success, else for failed
            */
        int get_int(std::string name, int &result);
        int set_int(std::string name, int result);

        /** @brief get long value according to the key name
         *  @param name name of key
         *  @param result result of long
         *  @return 0 for success, else for failed
            */
        int get_long(std::string name, int64_t &result);
        int set_long(std::string name, int64_t result);
        /** @brief get int value list according to the key name
         *  @param name name of key
         *  @param result result of int list
         *  @return 0 for success, else for failed
            */
        int get_int_list(std::string name, std::vector<int> &result);
        int set_int_list(std::string name, std::vector<int> result);
        /** @brief get double value according to the key name
         *  @param name name of key
         *  @param result result of double
         *  @return 0 for success, else for failed
            */
        int get_double(std::string name, double &result);
        int set_double(std::string name, double result);
        /** @brief get double value list according to the key name
         *  @param name name of key
         *  @param result result of doule list
         *  @return 0 for success, else for failed
            */
        int get_double_list(std::string name, std::vector<double> &result);
        int set_double_list(std::string name, std::vector<double> result);

        /** @brief get float value according to the key name
         *  @param name name of key
         *  @param result result of float
         *  @return 0 for success, else for failed
            */
        int get_float(std::string name, float &result);
        int set_float(std::string name, float result);
        /** @brief get float value list according to the key name
         *  @param name name of key
         *  @param result result of float list
         *  @return 0 for success, else for failed
            */
        int get_float_list(std::string name, std::vector<float> &result);
        int set_float_list(std::string name, std::vector<float> result);
        /** @brief dump json object to string
         *  @return json string
            */
        std::string dump() const;

    public:
        std::shared_ptr<ParamImpl> param_impl_;

    };

}

#endif //BMF_PARSE_JSON_H
