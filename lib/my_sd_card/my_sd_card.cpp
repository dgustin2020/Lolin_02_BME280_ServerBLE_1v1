/*
* my_sd_card-cpp
*
* ----------------------------
* SD Reader pins (default VSPI pins)
* ----------------------------
*#define SD_SCK   18
*#define SD_MISO  19
*#define SD_MOSI  23
*#define SD_CS    5

*/

#include  <Arduino.h>
#include  <my_define.h>
#include  <SD.h>
#include  <FS.h>
#include  <My_Enable_Debug.h>
#include  "my_sd_card.h"

static uint16_t SD_error = SD_OK;          
static uint16_t File_error = FILE_CMD_OK;

static void     my_sd_card_type_to_serial(uint8_t type);

static uint8_t   cardType = 0;
static uint64_t  cardSize = 0;
static String    readFile = "";
static uint8_t   type_file_open = FILE_CLOSE;
static String const     path_file_setting = "/impo.txt";
static File      myFile;

static uint16_t test_sd_ok(void)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    type_file_open = FILE_CLOSE;
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  SD_error = SD_OK;
  return SD_OK;    //sd non montata, esco con errore
}

/************ pubbliche  ********************/
/********************************************************************** */
/*************** gestione myFile ****************************************/
/********************************************************************** */


/*
* apre il "myFile" in lettura
* set "type_file_open" con FILE_OPEN_READ
* se non esiste esiste esce con errore
* set "type_file_open" con FILE_CLOSE
*/
uint8_t my_sd_myFile_open_read(String name_file)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    type_file_open = FILE_CLOSE;
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  if (type_file_open != FILE_CLOSE){//test se file gia aperto
    File_error = FILE_OPEN_OPEN;
    return FILE_OPEN_OPEN;//exit con file gia aperto
  }
  #ifdef DEBUG_SD
  Serial.printf("Open file Read: %s\n\r", name_file);
  #endif
  /* apri in scrittura e verifica se file esiste */
  myFile = SD.open(name_file, FILE_READ);
  if (!myFile) {//test se file aperto
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for reading");
    #endif
    type_file_open = FILE_CLOSE;
    File_error = SD_OPEN_READ_FAILED;
    return SD_OPEN_READ_FAILED;//file non esiste
  }
  type_file_open = FILE_OPEN_READ;
  File_error = FILE_CMD_OK;
  return FILE_CMD_OK;
}

/*
* apre il "myFile" in scrittura
* set "type_file_open" con FILE_OPEN_WRITE
* se non esiste esiste esce con errore
* set "type_file_open" con FILE_CLOSE
*/
uint8_t my_sd_myFile_open_write(String name_file)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    type_file_open = FILE_CLOSE;
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  if (type_file_open != FILE_CLOSE){//test se file gia aperto
    File_error = FILE_OPEN_OPEN;
    return FILE_OPEN_OPEN;//exit con file gia aperto
  }
  #ifdef DEBUG_SD
  Serial.printf("Open file Write: %s\n\r", name_file);
  #endif
  /* aprin in scrittura e verifica se file esiste */
  myFile = SD.open(name_file, FILE_WRITE);
  if (!myFile) {//test se file aperto
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for writing");
    #endif
    type_file_open = FILE_CLOSE;
    SD_error = SD_OPEN_READ_FAILED;
    return SD_OPEN_READ_FAILED;//file non esiste
  }
  type_file_open = FILE_OPEN_WRITE;
  File_error = FILE_CMD_OK;
  return FILE_CMD_OK;
}

/*
* apre il "myFile" in append
* set "type_file_open" con FILE_OPEN_APPEND
* se non esiste esiste esce con errore
* set "type_file_open" con FILE_CLOSE
*/
uint8_t my_sd_myFile_open_append(String name_file)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    type_file_open = FILE_CLOSE;
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  if (type_file_open != FILE_CLOSE){//test se file gia aperto
    File_error = FILE_OPEN_OPEN;
    return FILE_OPEN_OPEN;//exit con file gia aperto
  }
  #ifdef DEBUG_SD
  Serial.printf("Open file Append: %s\n\r", name_file);
  #endif
  /* aprin in scrittura e verifica se file esiste */
  myFile = SD.open(name_file, FILE_APPEND);
  if (!myFile) {//test se file aperto
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for appeding");
    #endif
    type_file_open = FILE_CLOSE;
    SD_error = SD_OPEN_READ_FAILED;
    return SD_OPEN_READ_FAILED;//file non esiste
  }
  type_file_open = FILE_OPEN_APPEND;
  File_error = FILE_CMD_OK;
  return FILE_CMD_OK;
}

/*
* chiude "myFile"
* set "type_file_open" con FILE_CLOSE
*/
uint8_t my_sd_myFile_close(void)
{
  if (type_file_open == FILE_CLOSE){//test se muFile open
    File_error = FILE_CLOSE_CLOSE;
    return FILE_CLOSE_CLOSE;    
  }
  type_file_open = FILE_CLOSE;
  myFile.close();
  SD.end();
  File_error = FILE_CMD_OK;
  SD_error = SD_OK;
  return FILE_CMD_OK;
}

/*
* write la stringa passata in  "myFile"
* chiude myFile
* sostituitsce vecchio file in SD
*/
uint8_t my_sd_myFile_write_string(String *testo_string)
{
  if (type_file_open != FILE_OPEN_WRITE){//test se myFile aperto in modo write 
    return type_file_open;//exit con tipo di open o close
  }
  /*file aperto write, metti testo*/
  uint8_t result;
  testo_string->concat(FILE_END_OF_STRING);
  if (myFile.print(*testo_string)) {//scrivi messaggio
    #ifdef DEBUG_SD
    Serial.println("File written");
    #endif
    result = FILE_CMD_OK;//exit comando riuscito
  } 
  else {
    #ifdef DEBUG_SD
    Serial.println("Write failed");
    #endif
    result = FILE_CMD_ERR;
  }
  my_sd_myFile_close();//chiudi myFile
  File_error = result;
  return result;
}

/*
* append la stringa passata in  "myFile"
* non chiude myFile
*/
uint8_t my_sd_myFile_append_string(String *testo_string)
{
  if (type_file_open != FILE_OPEN_APPEND){//test se myFile aperto in modo append 
    return type_file_open;//exit con tipo di open o close
  }
  /*file aperto append, metti stringa*/
  uint8_t result;
  testo_string->concat(FILE_END_OF_STRING);
  if (myFile.print(*testo_string)) {//scrivi messaggio
    #ifdef DEBUG_SD
    Serial.println("File append");
    #endif
    result = FILE_CMD_OK;//exit comando riuscito
  } 
  else {
    #ifdef DEBUG_SD
    Serial.println("append failed");
    #endif
    result = FILE_CMD_ERR;
  }
  File_error = result;
  return result;
}

/*
* legge la prima stringa da "myFile" e la mette in "testo_string"
* non chiude myFile
* se non esiste una stringa disponibile, esce con errore
*/
uint8_t my_sd_myFile_read_string(String *testo_string, char terminator)
{
  if (type_file_open != FILE_OPEN_READ){//test se myFile aperto in modo append 
    return type_file_open;//exit con tipo di open o close
  }
  /*file aperto read, leggo stringa fino a marker fine stringa, se esiste*/
  uint8_t result;
  if (myFile.available()) { 
    if (terminator){
      #ifdef DEBUG_SD
      //Serial.println("read con terminatore");
      #endif
      int n_char = myFile.available();
      char* p_char = (char*)testo_string;
      while (n_char--){
        *p_char = myFile.read();
        if (*p_char == terminator){
          *p_char = 0;//fine stringa
          break;
        }
        p_char++;
      }
      *p_char = 0;//fine stringa
    }
    else {
      #ifdef DEBUG_SD
      Serial.println("read tutto file");
      #endif
      int rlen = myFile.available();
      int read_len = myFile.readBytes((char*)testo_string, rlen);
    }
    File_error = FILE_CMD_OK;
    return FILE_CMD_OK;
  }
  else {
    File_error = FILE_CMD_ERR;
    return FILE_CMD_ERR;
  }
}

/*********************************************************** */
/************   gestione sd card *****************************/
/*********************************************************** */
/*
* chiude connessione con sd
*/
void my_sd_card_end(void)
{
  SD_error = SD_OK;
  SD.end();
}

/*
* test se sd presente
* se tutto ok esce con SD_OK
* altrimenti esce i vari errori
* salva il tipo di card e le dimensioni in MByte --> forse inutile
*/
uint8_t my_sd_card_test(void)
{
  if (!SD.begin()) {
    #ifdef DEBUG_SD
    Serial.println("ERR: Card Mount Failed");
    #endif
    cardSize = 0;
    cardType = CARD_NONE;
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;
  }
  //sd montntata
  cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    #ifdef DEBUG_SD
    Serial.println("ERR: No SD card attached");
    #endif
    SD_error = SD_NO_ATTACHED;
    return SD_NO_ATTACHED;
  }
  cardSize = SD.cardSize() / (1024 * 1024);
  #ifdef DEBUG_SD
  my_sd_card_type_to_serial(cardType);
  #endif
  SD.end();
  SD_error = SD_OK;
  return SD_OK;
}

/*
*/
uint8_t my_sd_card_mount(void)
{
  if (!SD.begin()) {
    #ifdef DEBUG_SD
    Serial.println("ERR: Card Mount Failed");
    #endif
    cardSize = 0;
    cardType = CARD_NONE;
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;
  }
  return SD_OK;
}

/*
* list file su Serial
* da usare solo come test
*/
uint8_t my_sd_card_listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);
  File root = fs.open(dirname);
  if (!root) {
    #ifdef DEBUG_SD
    Serial.println("Failed to open directory");
    #endif
    SD_error = SD_OPEN_DIR_FAILED;
    return SD_OPEN_DIR_FAILED;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    SD_error = SD_NOT_A_DIR;
    return SD_NOT_A_DIR;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        my_sd_card_listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  SD_error = SD_OK;
  return SD_OK;
}

/*
* cre una dir
* esce con il riultato dell'operazione
*/
uint8_t my_sd_card_createDir(fs::FS &fs, const char * path) 
{
  #ifdef DEBUG_SD
  Serial.printf("Creating Dir: %s\n", path);
  #endif
  if (fs.mkdir(path)) {
    #ifdef DEBUG_SD
    Serial.println("Dir created");
    #endif
    SD_error = SD_OK;
    return SD_OK;
  } else {
    #ifdef DEBUG_SD
    Serial.println("mkdir failed");
    #endif 
    SD_error = SD_MKDIR_FAILED;
    return SD_MKDIR_FAILED;
  }
}

/*
* cancella la dir passata
*/
uint8_t my_sd_card_removeDir(fs::FS &fs, const char * path) 
{
  #ifdef DEBUG_SD
  Serial.printf("Removing Dir: %s\n", path);
  #endif
  if (fs.rmdir(path)) {
  #ifdef DEBUG_SD
    Serial.println("Dir removed");
  #endif
  SD_error = SD_OK;
  return SD_OK;
  } else {
  #ifdef DEBUG_SD
    Serial.println("rmdir failed");
  #endif
  SD_error = SD_RMDIR_FAILED;
  return SD_RMDIR_FAILED;
  }
}

/*
*
*/
uint8_t my_sd_card_renameFile(fs::FS &fs, const char * path1, const char * path2) 
{
  #ifdef DEBUG_SD
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  #endif
  if (fs.rename(path1, path2)) {
    #ifdef DEBUG_SD
    Serial.println("File renamed");
    #endif
    SD_error = SD_OK;
    return SD_OK;
  } 
  else {
    #ifdef DEBUG_SD
    Serial.println("Rename failed");
    #endif
    SD_error = SD_RENAME_FAILED;
    return SD_RENAME_FAILED;
  }
}

/*
*
*/
uint8_t my_sd_card_deleteFile(const char * path)
{
  #ifdef DEBUG_SD
  Serial.printf("Deleting file: %s\n", path);
  #endif
  if (SD.remove(path)) {
    #ifdef DEBUG_SD
    Serial.println("File deleted");
    #endif
    SD_error = SD_OK;
    return SD_OK;
  } 
  else {
    #ifdef DEBUG_SD
    Serial.println("Delete failed");
    #endif
    SD_error = SD_DELETE_FAILED;
    return SD_DELETE_FAILED;
  }
}

/*
* test se esite il file passato come path
* se esiste     -> false
* se non esiste -> true
 */
bool my_sd_card_fileIsNotPresent(String path) 
{
  myFile = SD.open(path, FILE_READ);
  if (myFile){
    myFile.close();
    return false;
  }
  return true;
#if 0
  if (SD.exists(path)){
    SD_error = SD_OK;
    return false;
  }
  SD_error = SD_NOT_A_DIR;
  return true;
  //return SD.exists(path);
#endif
}


/*
*
*/
void my_sd_card_testFileIO(fs::FS &fs, const char * path) 
{
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}



/*
* write la stringa passata nel file passato
* sostituitsce vecchio file
*/
uint8_t my_sd_card_write_string_to_file(String testo_string, String name_file)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  uint8_t result;  
  #ifdef DEBUG_SD
  Serial.printf("Writing file: %s\n\r", name_file);
  #endif
  /* aprin in scrittura e verifica se file esiste */
  File file = SD.open(name_file, FILE_WRITE);
  if (!file) {
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for writing");
    #endif
    SD_error = SD_OPEN_WRITE_FAILED;
    return SD_OPEN_WRITE_FAILED;//errore apetuta file in scrittura
  }
  /*file aperto, metti testo*/
  if (file.print(testo_string)) {//scrivi messaggio
    #ifdef DEBUG_SD
    Serial.println("File written");
    #endif
    result = SD_OK;
  } 
  else {
    #ifdef DEBUG_SD
    Serial.println("Write failed");
    #endif
    result = SD_WRITE_FAILED;
  }
  file.close();
  SD.end();
  SD_error = result;
  return result;
}

/*
* append la stringa passata nel file passato
* agiunge in coda al vecchio file
*/
uint8_t my_sd_card_append_string_to_file(String testo_string, String name_file)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  uint8_t result;  
  #ifdef DEBUG_SD
  Serial.printf("Append file: %s\n\r", name_file);
  #endif
  /* aprin in scrittura e verifica se file esiste */
  File file = SD.open(name_file, FILE_APPEND);
  if (!file) {
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for append");
    #endif
    SD_error = SD_OPEN_APPEND_FAILED;
    return SD_OPEN_APPEND_FAILED;//errore apetuta file in scrittura
  }
  /*file aperto, metti testo*/
  if (file.print(testo_string)) {//scrivi messaggio
    #ifdef DEBUG_SD
    Serial.println("File append");
    #endif
    result = SD_OK;
  } 
  else {
    #ifdef DEBUG_SD
    Serial.println("Append failed");
    #endif
    result = SD_APPEND_FAILED;
  }
  file.close();
  SD.end();
  SD_error = result;
  return result;
}

/*
* legge il file passato e sotto forma di stringa lo mette in "testo_srringa"
*/
uint8_t my_sd_card_read_file_to_string(String *testo_string, String name_file)
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  uint8_t result;  
  #ifdef DEBUG_SD
  Serial.printf("Read file: %s\n\r", name_file);
  #endif
//verifica se file esiste
  File file = SD.open(name_file);//open file
  if (!file) {
  #ifdef DEBUG_SD
    Serial.println("Failed to open file for reading");
  #endif
    SD_error = SD_OPEN_READ_FAILED;
    return SD_OPEN_READ_FAILED;//file non esiste
  }

  #ifdef DEBUG_SD
  Serial.print("Read from file: ");
  #endif
  /* test se file disponibile alla lettura */
  String str;
  while (file.available()) {
    //*testo_string = file.readString();
    //Serial.write(file.read());//su seriale il file letto
    str = file.readStringUntil(CR);
    Serial.printf("-%s \r",str);
    testo_string->concat("-");
    testo_string->concat(str);
    testo_string->concat(" \r");
  }
  file.close();
  SD.end();
  SD_error = SD_OK;
  return SD_OK;
}



/************ private ********************
/*
*su seriiale il tipo di sd card
*/
static void my_sd_card_type_to_serial(uint8_t type)
{
  Serial.print("SD Card Type: ");
  if (type == CARD_MMC) {
    Serial.println("MMC");
  } else if (type == CARD_SD) {
    Serial.println("SDSC");
  } else if (type == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
    return;
  }
  if (!cardSize){
      return;
  }
  Serial.printf("SD Card Size: %lluMB\n\r", cardSize);
}


/*
* legge il file passato byte x byte e lo mette "readfile"
* false -> tutto OK
* true  -> problem
*/
uint8_t my_sd_file_read_char_to_buff(String name_file, uint8_t *p_buff) 
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  File file = SD.open(name_file);//open file
  if (!file) {
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for reading char");
    #endif
    SD_error = SD_OPEN_READ_FAILED;
    return SD_OPEN_READ_FAILED;//file non esiste
  }

  //read char
  int rlen = file.available();
  file.read(p_buff, rlen);
  file.close();  // close the file:
  SD_error = SD_OK;
  File_error = FILE_CMD_OK;
  return FILE_CMD_OK;
}

/*
* write singolo byte nel file passato per la uqnatità n_char
* chiude il file
* FILE_CMD_OK -> tutto OK
* altro  -> problem
*/
uint8_t my_sd_file_write_char_from_buff(String name_file, uint8_t *p_buff, int16_t n_char) 
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  File file = SD.open(name_file, FILE_WRITE);//open file in write
  if (!file) {
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for write char");
    #endif
    SD_error = SD_OPEN_WRITE_FAILED;
    return SD_OPEN_WRITE_FAILED;//errore apetuta file in scrittura
  }
  //write uint8_t
  file.write(p_buff, n_char);
  file.close();  // close the file: 
  SD_error = SD_OK;
  File_error = FILE_CMD_OK;
  return FILE_CMD_OK;
}

/*
* write nel name_file passato, i byte del p_buff passato per la uqnatità n_char
* mettndoli in cods
* chiude il file
* FILE_CMD_OK -> tutto OK
* altro  -> problem
*/
uint8_t my_sd_file_append_char_from_buff(String name_file, uint8_t *p_buff, int16_t n_char) 
{
  if (!SD.begin()) { // Check if SD card is mounted successfully
    SD_error = SD_MOUNT_FAILED;
    return SD_MOUNT_FAILED;    //sd non montata, esco con errore
  }
  File file = SD.open(name_file, FILE_WRITE);//open file in write
  if (!file) {
    #ifdef DEBUG_SD
    Serial.println("Failed to open file for append char");
    #endif
    SD_error = SD_OPEN_APPEND_FAILED;
    return SD_OPEN_APPEND_FAILED;//errore apetuta file in scrittura
  }
  //write uint8_t
  file.write(p_buff, n_char);
  file.close();  // close the file:  
  SD_error = SD_OK;
  File_error = FILE_CMD_OK;
  return FILE_CMD_OK;
}

/*
* exit con il tipo di errore nella manipolazione della SD
*/
uint16_t my_sd_card_get_error(void)
{
  return SD_error;
}

uint16_t my_sd_myFile_get_error(void)
{
  return File_error;
}
