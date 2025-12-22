/*
my_sd_card.h
*/
#ifndef MY_SD_CARD_H
#define MY_SD_CARD_H

#include <FS.h>

/***************** */
#define SD_OK                 0                         // 0  "Card OK"
#define SD_MOUNT_FAILED       SD_OK + 1                 // 1  "Card Mount Failed"  
#define SD_NO_ATTACHED        SD_MOUNT_FAILED + 1       // 2  "No SD card attached"
#define SD_OPEN_DIR_FAILED    SD_NO_ATTACHED + 1        // 3  "Failed to open directory"
#define SD_NOT_A_DIR          SD_OPEN_DIR_FAILED + 1    // 4  "Not a directory"
#define SD_MKDIR_FAILED       SD_NOT_A_DIR + 1          // 5  "mkdir failed"
#define SD_RMDIR_FAILED       SD_MKDIR_FAILED + 1       // 6  "rmdir failed"
#define SD_OPEN_READ_FAILED   SD_RMDIR_FAILED + 1       // 7  "Failed to open file for reading"
#define SD_OPEN_WRITE_FAILED  SD_OPEN_READ_FAILED + 1   // 8  "Failed to open file for writing"
#define SD_WRITE_FAILED       SD_OPEN_WRITE_FAILED + 1  // 9  "Write failed"
#define SD_OPEN_APPEND_FAILED SD_WRITE_FAILED + 1       //10  "Failed to open file for appending" 
#define SD_APPEND_FAILED      SD_OPEN_APPEND_FAILED + 1 //11  "Failed to open file for appending" 
#define SD_RENAME_FAILED      SD_APPEND_FAILED + 1      //12  "Rename failed"
#define SD_DELETE_FAILED      SD_RENAME_FAILED + 1      //13  "Delete failed"

/*********** */
#define FILE_CMD_OK       0   //myFile comando riuscito
#define FILE_CMD_ERR      1   //myFile comando non riuscito
#define FILE_CLOSE        2   //myFile close
#define FILE_OPEN_READ    3   //myFile open read
#define FILE_OPEN_WRITE   4   //myFile open write
#define FILE_OPEN_APPEND  5   //myFile open append
#define FILE_OPEN_OPEN    6   //myFile gia open
#define FILE_CLOSE_CLOSE  7   //myFile gia close

#define FILE_END_OF_STRING  CR

/************** */
extern uint8_t  my_sd_myFile_open_append(String name_file);
extern uint8_t  my_sd_myFile_open_read(String name_file);
extern uint8_t  my_sd_myFile_open_write(String name_file);
extern uint8_t  my_sd_myFile_close(void);

extern uint8_t  my_sd_myFile_write_string(String *testo_string);
extern uint8_t  my_sd_myFile_append_string(String *testo_string);
extern uint8_t  my_sd_myFile_read_string(String *testo_string, char terminator);
extern uint16_t my_sd_myFile_get_error(void);

extern uint8_t  my_sd_file_write_char_from_buff(String name_file, uint8_t *p_buff, int16_t n_char);
extern uint8_t  my_sd_file_read_char_to_buff(String name_file, uint8_t *p_buff);
extern uint8_t  my_sd_file_append_char_from_buff(String name_file, uint8_t *p_buff, int16_t n_char);

extern uint8_t  my_sd_card_test(void);
extern uint8_t  my_sd_card_mount(void);
extern void     my_sd_card_end(void);
extern uint8_t  my_sd_card_listDir(fs::FS &fs, const char * dirname, uint8_t levels);
extern uint8_t  my_sd_card_createDir(fs::FS &fs, const char * path);
extern uint8_t  my_sd_card_removeDir(fs::FS &fs, const char * path);
extern uint8_t  my_sd_card_renameFile(fs::FS &fs, const char * path1, const char * path2);
extern uint8_t  my_sd_card_deleteFile(const char * path);
extern bool     my_sd_card_fileIsNotPresent(String path); 
extern void     my_sd_card_testFileIO(fs::FS &fs, const char * path);
extern uint8_t  my_sd_card_write_string_to_file(String tsto, String name_file);
extern uint8_t  my_sd_card_append_string_to_file(String testo_string, String name_file);
extern uint8_t  my_sd_card_read_file_to_string(String *testo_string, String name_file);
extern uint16_t my_sd_card_get_error(void);


#endif