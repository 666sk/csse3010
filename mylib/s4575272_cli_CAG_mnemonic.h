/**
  ******************************************************************************
  * @file    mylib/s457527_cli_CAG_mnemonic.h
  * @author  Kuang Sheng
  * @date    16/05/2022
  * @brief   mylib CLI peripheral driver file
  *
  ******************************************************************************
  * EXTERNAL FUNCTIONS 
  * BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvStartCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvStopCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvDelCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvCreCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvSystemCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  * BaseType_t prvUsageCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
  ******************************************************************************
  */


#ifndef __S4575272_cli_CAG_mnemonic_H__
#define __S4575272_cli_CAG_mnemonic_H__

#include "s4575272_CAG_simulator.h"
#include "FreeRTOS_CLI.h"
#include <string.h>


extern BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvStartCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvStopCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvDelCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvCreCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvSystemCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
extern BaseType_t prvUsageCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );


extern CLI_Command_Definition_t xStill;
extern CLI_Command_Definition_t xOsc;
extern CLI_Command_Definition_t xGlider;
extern CLI_Command_Definition_t xStart;
extern CLI_Command_Definition_t xStop;
extern CLI_Command_Definition_t xClear;
extern CLI_Command_Definition_t xDel;
extern CLI_Command_Definition_t xCre;
extern CLI_Command_Definition_t xSystem;
extern CLI_Command_Definition_t xUsage;


#endif