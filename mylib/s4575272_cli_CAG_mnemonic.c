/**
  ******************************************************************************
  * @file    mylib/s457527_cli_CAG_mnemonic.c
  * @author  Kuang Sheng
  * @date    16/05/2022
  * @brief   mylib CLI peripheral driver file
  *
  ******************************************************************************
  * EXTERNAL FUNCTIONS 
  * 
  ******************************************************************************
  */

#include "s4575272_cli_CAG_mnemonic.h"


CLI_Command_Definition_t xStill = {
  "still",					
	"still <type><x><y>\r\n",	
	prvStillCommand,					
	2					
};


extern BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {


}

/*
static BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

}


static BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

}
*/