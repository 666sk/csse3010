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

CLI_Command_Definition_t xOsc = {
  "osc",					
	"osc <type><x><y>\r\n",	
	prvOscCommand,					
	1					
};


extern BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {
  	long lParam_len;
	const char *cCmd_string;

	/* Get parameters from command string */
	cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 2, &lParam_len);

	/* Write command echo output string to write buffer. */
	xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

	/* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;

}


extern BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {
  	long lParam_len;
	const char *cCmd_string;

	/* Get parameters from command string */
	cCmd_string = FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParam_len);

	/* Write command echo output string to write buffer. */
	xWriteBufferLen = sprintf((char *) pcWriteBuffer, "2%s\n\r", cCmd_string);

	/* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}

/*
static BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

}
*/