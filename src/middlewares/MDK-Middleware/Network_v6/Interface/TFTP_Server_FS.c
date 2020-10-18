/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    TFTP_Server_FS.c
 * Purpose: TFTP Server File System Interface
 * Rev.:    V6.2
 *----------------------------------------------------------------------------*/
//! [code_TFTP_Server_FS]
#include <stdio.h>
#include "rl_net.h"

// Open a file for reading or writing in TFTP server.
__weak void *tftp_server_fopen (const char *fname, const char *mode) {
  return (fopen (fname, mode));
}

// Close a file previously open in TFTP server.
__weak void tftp_server_fclose (void *file) {
  fclose (file);
}

// Read block of data from a file in TFTP server.
__weak uint32_t tftp_server_fread (void *file, uint8_t *buf, uint32_t len) {
  return (fread (buf, 1, len, file));
}

// Write block of data to a file in TFTP server.
__weak uint32_t tftp_server_fwrite (void *file, const uint8_t *buf, uint32_t len) {
  return (fwrite (buf, 1, len, file));
}
//! [code_TFTP_Server_FS]
