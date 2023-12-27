#pragma once
#include "libssh2_config.h"
#include"libssh2.h"
#include"libssh2_sftp.h"


typedef struct SSH_CONFIG
{
	const char *hostname;
	const char* commandline;
	const char *username;
	const char *password;
}ssh_config;


class CSSH
{
public:
	CSSH();
	~CSSH();	
	int sssock;
	struct sockaddr_in sin;
	const char *fingerprint;
	LIBSSH2_SESSION *session;
	LIBSSH2_CHANNEL *channel;
	int rc;
	int exitcode;
	char *exitsignal = (char *)"none";
	int bytecount = 0;
	size_t len;
	LIBSSH2_KNOWNHOSTS *nh;
	int type;
	WSADATA wsadata;
	int err;

	int waitsocket(int socket_fd, LIBSSH2_SESSION *session);
	bool SSH_Query_NO_password(ssh_config m_sshconfig, CString* ERROR_Info, CString* Result, CString* err_result, int sleep_time);
	bool SSH_Send_Command_No_password(ssh_config m_sshconfig, CString *ERROR_Info);
	bool SSH_Close();
	// 带登录用户名及密码登录SSH
	bool SSH_Query(ssh_config m_sshconfig, CString *ERROR_Info, CString *Result, CString *err_result, int sleep_time);
	bool SSH_Send_Command(ssh_config m_sshconfig, CString *ERROR_Info);
};

