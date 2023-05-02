#pragma once

#include "jsoncpp/json/json.h"

#include <fstream>
#include <filesystem>
#include <regex>

#include "date/date.h"
#include "date/tz.h"
#include <chrono>

namespace sakurajin{
    class jsonRecorder {
      private:
        static void ensureFileExists(const std::filesystem::path& path);

        static void checkObject(const Json::Value& jsonData);
        static bool checkTimestamp(const std::string& timestamp);
        static void recordObjects(const Json::Value& jsonData, const std::string& path);
      public:
        static void record(const Json::Value& jsonData, const std::filesystem::path& path);
        static std::string currentTimestamp();
    };
} // namespace sakurajin

void sakurajin::jsonRecorder::ensureFileExists(const std::filesystem::path& path) {
    const auto& folder = path.parent_path();

    if(!std::filesystem::exists(folder)){
        std::filesystem::create_directories(folder);
    }

    if(std::filesystem::exists(path)) {
        if (!std::filesystem::is_regular_file(path)) {
            throw std::runtime_error("path is not a regular file");
        }
        return;
    }

    std::ofstream file(path);
    file << "[]";
    file.close();
}

bool sakurajin::jsonRecorder::checkTimestamp(const std::string& timestamp) {
    const std::regex timestampRegex("\\d{4}-\\d{2}-\\d{2}\\ \\d{2}:\\d{2}:\\d{2}");
    return std::regex_match(timestamp, timestampRegex);
}

void sakurajin::jsonRecorder::checkObject(const Json::Value& jsonData) {
    if(!jsonData.isObject()){
        throw std::invalid_argument("json data is not an object");
    }

    if(!jsonData.isMember("timestamp")){
        throw std::invalid_argument("json data does not have a timestamp member");
    }
    if(!jsonData["timestamp"].isString()){
        throw std::invalid_argument("json data timestamp member is not a string");
    }
    if(!checkTimestamp(jsonData["timestamp"].asString())){
        throw std::invalid_argument("json data timestamp member is not a valid timestamp");
    }

    if(jsonData.isMember("temperature")){
        if(!jsonData["temperature"].isDouble()){
            throw std::invalid_argument("json data temperature member is not a double");
        }
    }

    if(jsonData.isMember("humidity")){
        if(!jsonData["humidity"].isDouble()){
            throw std::invalid_argument("json data humidity member is not a double");
        }
    }
}

void sakurajin::jsonRecorder::recordObjects(const Json::Value& jsonData, const std::string& path) {
    if(!jsonData.isArray()){
        throw std::invalid_argument("json data is not an array");
    }
    try{
        ensureFileExists(path);
    }catch (...){
        std::throw_with_nested(std::runtime_error("failed to ensure file exists"));
    }

    try{
        for(const auto& data : jsonData){
            checkObject(data);
        }
    }catch (...){
        std::throw_with_nested(std::runtime_error("invalid object in data array"));
    }

    //load the existing data
    std::ifstream jsonIfFile(path, std::ifstream::binary);
    Json::Value fileData;
    jsonIfFile >> fileData;
    jsonIfFile.close();

    //append the new data to the existing data
    for(const auto& data : jsonData){
        fileData.append(data);
    }

    //write the new data to the file
    std::ofstream jsonOfFile(path, std::ofstream::binary);
    jsonOfFile << fileData;
    jsonOfFile.close();
}

void sakurajin::jsonRecorder::record(const Json::Value& jsonData, const std::filesystem::path& path) {
    try{
        ensureFileExists(path);
    }catch(...){
        std::throw_with_nested(std::runtime_error("failed to ensure file exists"));
    }

    if(jsonData.isObject()) {
        try{
            Json::Value data = Json::arrayValue;
            data.append(jsonData);
            recordObjects(data, path);
        }catch(...){
            std::throw_with_nested(std::runtime_error("failed to record object"));
        }
    }else if(jsonData.isArray()){
        try {
            recordObjects(jsonData, path);
        } catch (...) {
            std::throw_with_nested(std::runtime_error("failed to record array"));
        }
    }else{
        throw std::runtime_error("json data is not an object or an array");
    }
}

std::string sakurajin::jsonRecorder::currentTimestamp() {
    auto localTimeStamp = date::make_zoned(date::current_zone(), std::chrono::system_clock::now());

    const std::string dateFormats[] = {
        "%F %R:%OS",
        "%F %T",
        "%F %R:%2S",
    };

    //try all formats until one works as expected
    for(const auto& format : dateFormats){
        std::string data = date::format(format, localTimeStamp);
        if(checkTimestamp(data)){
            return data;
        }
    }
    throw std::runtime_error("failed to get current timestamp in a correct format");
}

