/*
* my_sd_drive.h
*/
#ifndef MY_SD_DRIVE_H
#define MY_SD_DRIVE_H

#include <FS.h>

#define SD_OK                 0                       // 0  "Card OK"
#define SD_MOUNT_FAILED       SD_OK + 1               // 1  "Card Mount Failed"  
#define SD_NO_ATTACHED        SD_MOUNT_FAILED + 1     // 2  "No SD card attached"
#define SD_OPEN_DIR_FAILED    SD_NO_ATTACHED + 1      // 3  "Failed to open directory"
#define SD_NOT_A_DIR          SD_OPEN_DIR_FAILED + 1  // 4  "Not a directory"
#define SD_MKDIR_FAILED       SD_NOT_A_DIR + 1        // 5  "mkdir failed"
#define SD_RMDIR_FAILED       SD_MKDIR_FAILED + 1     // 6  "rmdir failed"
#define SD_OPEN_READ_FAILED   SD_RMDIR_FAILED + 1     // 7  "Failed to open file for reading"

extern uint16_t sd_listDir(fs::FS &fs, const char * dirname, uint8_t levels);


#endif