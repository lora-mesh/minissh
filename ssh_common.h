
#ifndef __SSH_COMMON_H__
#define __SSH_COMMON_H__  1


#include <libssh/libssh.h>
#include <libssh/sftp.h>
int authenticate_console(ssh_session session,char* pass);
int authenticate_kbdint(ssh_session session, const char *password);
int verify_knownhost(ssh_session session);
ssh_session connect_ssh(const char *hostname, const char *user,char * pass, int verbosity);


#endif // __SSH_COMMON_H__
