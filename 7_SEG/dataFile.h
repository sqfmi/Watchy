#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <vector>

#define FORMAT_LITTLEFS_IF_FAILED true
#define DATA_FOLDER "/data"
#define STEPS_FOLDER "/data/steps"

class FSData {
    public:
        String content;
        std::vector< String > files;

    public:
        void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
        static void createDir(fs::FS &fs, const char * path);
        static void removeDir(fs::FS &fs, const char * path);
        void readFile(fs::FS &fs, const char * path);
        static void writeFile(fs::FS &fs, const char * path, const char * message);
        static void appendFile(fs::FS &fs, const char * path, const char * message);
        static void renameFile(fs::FS &fs, const char * path1, const char * path2);
        static void deleteFile(fs::FS &fs, const char * path);
};

#endif