

/**
 * @brief Decompresses/decrypts a stream with a header such as ECTT or ZCTT
 *  and advances the current position to after the encrypted portion
 *
 * @param compressedStreamPtr Pointer to a file stream
 */
int streamDecrypt(FILE **compressedStreamPtr);

/**
 * @brief Splits files contained in @p stream and stores them in @p folderPath
 * assuming @p stream is an archive
 *
 * @param stream archive stream
 * @param folderPath Path to folder where output files will be stored
 */
int archiveSplit(FILE *stream, const char *folderPath);

int streamToFile(FILE *stream, const char *outputPath);

int writeFileNames(FILE *stream, FILE *fileNames);
