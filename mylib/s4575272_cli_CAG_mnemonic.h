/**
  ******************************************************************************
  * @file    mylib/s457527_cli_CAG_mnemonic.h
  * @author  Kuang Sheng
  * @date    16/05/2022
  * @brief   mylib CLI peripheral driver file
  *
  ******************************************************************************
  * EXTERNAL FUNCTIONS 
  * 
  ******************************************************************************
  */


#ifndef __S4575272_cli_CAG_mnemonic_H__
#define __S4575272_cli_CAG_mnemonic_H__

#include "s4575272_CAG_simulator.h"
#include "FreeRTOS_CLI.h"
#include <string.h>


extern BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
//static BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
//static BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

extern CLI_Command_Definition_t xStill;


/*
CLI_Command_Definition_t xOsc = {
  "osc",					
	"osc <type><x><y>\r\n",	
	prvOscCommand,					
	1					
};

CLI_Command_Definition_t xGlider = {
  "glider",					
	"glider <x><y>\r\n",	
	prvGliderCommand,					
	1					
};
*/


#endif