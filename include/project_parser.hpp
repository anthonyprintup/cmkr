#pragma once

#include <map>
#include <mpark/variant.hpp>
#include <stdexcept>
#include <string>
#include <tsl/ordered_map.h>
#include <vector>

namespace cmkr {
namespace parser {

struct Setting {
    std::string name;
    std::string comment;
    mpark::variant<bool, std::string> val;
    bool cache = false;
    bool force = false;
};

struct Option {
    std::string name;
    std::string comment;
    bool val = false;
};

struct Package {
    std::string name;
    std::string version;
    bool required = true;
    bool config = true;
    std::vector<std::string> components;
};

struct Vcpkg {
    std::string version;
    std::string url;
    std::vector<std::string> packages;
};

enum TargetType {
    target_executable,
    target_library,
    target_shared,
    target_static,
    target_interface,
    target_custom,
};

template <typename T>
using Condition = tsl::ordered_map<std::string, T>;

using ConditionVector = Condition<std::vector<std::string>>;

struct Target {
    std::string name;
    TargetType type = {};

    ConditionVector headers;
    ConditionVector sources;

    // https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html#project-commands
    ConditionVector compile_definitions;
    ConditionVector private_compile_definitions;

    ConditionVector compile_features;
    ConditionVector private_compile_features;

    ConditionVector compile_options;
    ConditionVector private_compile_options;

    ConditionVector include_directories;
    ConditionVector private_include_directories;

    ConditionVector link_directories;
    ConditionVector private_link_directories;

    ConditionVector link_libraries;
    ConditionVector private_link_libraries;

    ConditionVector link_options;
    ConditionVector private_link_options;

    ConditionVector precompile_headers;
    ConditionVector private_precompile_headers;

    std::string condition;
    std::string alias;
    Condition<tsl::ordered_map<std::string, std::string>> properties;

    Condition<std::string> cmake_before;
    Condition<std::string> cmake_after;
    ConditionVector include_before;
    ConditionVector include_after;
};

struct Test {
    std::string name;
    std::vector<std::string> configurations;
    std::string working_directory;
    std::string command;
    std::vector<std::string> arguments;
};

struct Install {
    std::vector<std::string> targets;
    std::vector<std::string> files;
    std::vector<std::string> dirs;
    std::vector<std::string> configs;
    std::string destination;
};

struct Subdir {
    std::string name;
    std::string condition;
    Condition<std::string> cmake_before;
    Condition<std::string> cmake_after;
    ConditionVector include_before;
    ConditionVector include_after;
};

struct Project {
    // This is the CMake version required to use all cmkr versions.
    std::string cmake_version = "3.15";
    std::string cmkr_include = "cmkr.cmake";
    std::string build_dir = "build";
    std::string generator;
    std::string config;
    bool allow_in_tree = false;
    Condition<std::vector<std::string>> project_subdirs;
    std::vector<std::string> cppflags;
    std::vector<std::string> cflags;
    std::vector<std::string> linkflags;
    std::vector<std::string> gen_args;
    std::vector<std::string> build_args;
    std::string project_name;
    std::string project_version;
    std::string project_description;
    std::vector<std::string> project_languages;
    Condition<std::string> cmake_before;
    Condition<std::string> cmake_after;
    ConditionVector include_before;
    ConditionVector include_after;
    std::vector<Setting> settings;
    std::vector<Option> options;
    std::vector<Package> packages;
    Vcpkg vcpkg;
    tsl::ordered_map<std::string, std::map<std::string, std::string>> contents;
    std::vector<Target> targets;
    std::vector<Test> tests;
    std::vector<Install> installs;
    tsl::ordered_map<std::string, std::string> conditions;
    std::vector<Subdir> subdirs;

    Project(const Project *parent, const std::string &path, bool build);
};

bool is_root_path(const std::string &path);

} // namespace parser
} // namespace cmkr