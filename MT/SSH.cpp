#include "stdafx.h"
#include "SSH.h"


CSSH::CSSH()
{
}


CSSH::~CSSH()
{
}


int CSSH::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
	struct timeval timeout;
	int rc;
	fd_set fd;
	fd_set *writefd = NULL;
	fd_set *readfd = NULL;
	int dir;

	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	FD_ZERO(&fd);

	FD_SET(socket_fd, &fd);

	/* now make sure we wait in the correct direction */
	dir = libssh2_session_block_directions(session);


	if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
		readfd = &fd;

	if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
		writefd = &fd;

	rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

	return rc;
}



bool CSSH::SSH_Query_NO_password(ssh_config m_sshconfig, CString *ERROR_Info, CString *Result, CString *err_result, int sleep_time)
{
	//unsigned long hostaddr = 0;
	CString csData;
	*Result = _T("");
	*err_result = _T("");
	bool Link;

	try
	{
		//打开socket连接库
		err = WSAStartup(MAKEWORD(2, 0), &wsadata);
		//创建socket套字
		sssock = socket(AF_INET, SOCK_STREAM, 0);

		if (err != 0)
		{
			csData.Format(_T("%d"), err);
			*ERROR_Info = _T("打开socket连接库失败:") + csData;
			throw(0);
		}


		sin.sin_family = AF_INET;
		sin.sin_port = htons(22);
		InetPton(sin.sin_family, CString(m_sshconfig.hostname), &sin.sin_addr.s_addr);
		if (connect(sssock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
		{
			*ERROR_Info = _T("连接到ssh服务器失败:");
			Link = false;
			throw(0);
		}
		Link = true;

		//初始化
		session = libssh2_session_init();

		// tell libssh2 we want it all done non-blocking
		//libssh2_session_set_blocking(session, 0);

		/* ... start it up. This will trade welcome banners, exchange keys,
		* and setup crypto, compression, and MAC layers
		*/
		while ((rc = libssh2_session_handshake(session, sssock)) == LIBSSH2_ERROR_EAGAIN);
		if (rc)
		{
			csData.Format(_T("%d"), rc);
			*ERROR_Info = _T("socket和session握手通信失败:") + csData;
			throw(0);
			//fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
			//MessageBox(_T("socket和session握手通信失败"));
		}

		nh = libssh2_knownhost_init(session);
		

		//若密码不为空
		if (strlen(m_sshconfig.password) != 0)
		{
			//while ((rc = libssh2_userauth_password(session, m_sshconfig.username, m_sshconfig.password)) == LIBSSH2_ERROR_EAGAIN);
			while ((rc = libssh2_userauth_password_ex(session, m_sshconfig.username, strlen(m_sshconfig.username), NULL, 0,NULL )) == LIBSSH2_ERROR_EAGAIN);
			if (rc)
			{
				*ERROR_Info = _T("验证登陆失败");
				throw(0);
				//fprintf(stderr, "Authentication by password failed.\n");
				//goto shutdown;
				//MessageBox(_T("验证登陆失败"));
			}
		}
		//若密码为空
			///*
		else
		{
			while (rc = libssh2_userauth_keyboard_interactive(session, m_sshconfig.username, NULL) == LIBSSH2_ERROR_EAGAIN)
				if (rc)
				{
					*ERROR_Info = _T("验证登陆失败");
					throw(0);
					//fprintf(stderr, "Authentication by password failed.\n");
					//goto shutdown;
					//MessageBox(_T("验证登陆失败"));
				}

		}
		//*/
		char* userauthlist = libssh2_userauth_list(session, m_sshconfig.username, strlen(m_sshconfig.username));

		//打开通道
		///*
		while ((channel = libssh2_channel_open_session(session)) == NULL && libssh2_session_last_error(session, NULL, NULL, 0)LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		//channel = libssh2_channel_open_session(session);
		if (channel == NULL)
		{
			*ERROR_Info = _T("打开通道失败");
			throw(0);
		}

	
		//发送指令
		//rc = libssh2_channel_write(channel, m_sshconfig.commandline, sizeof(m_sshconfig.commandline));
		///*
		while ((rc = libssh2_channel_exec(channel, m_sshconfig.commandline)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		if (rc != 0) {
			*ERROR_Info = _T("发送指令失败");
			throw(0);
		}

		Sleep(sleep_time);

		//读数据
		/*
		char buffer[0x50000];
		char buffer2[0x50000];
		memset(buffer, '\0', sizeof(buffer));
		memset(buffer2, '\0', sizeof(buffer2));
		rc = libssh2_channel_read(channel, buffer, sizeof(buffer));
		*Result = buffer;


		while (rc>0)
		{
			rc = libssh2_channel_read(channel, buffer, sizeof(buffer));
			for (int i = 0; i < rc; ++i)
			{
				*Result = *Result + buffer[i];
			}
		}

		rc = libssh2_channel_read_stderr(channel, buffer2, sizeof(buffer2));
		*ERR = buffer2;
		while (rc>0)
		{
			rc = libssh2_channel_read_stderr(channel, buffer2, sizeof(buffer2));
			for (int i = 0; i < rc; ++i)
			{
				*ERR = *ERR + buffer2[i];
			}
		}
		*/

		
		//读数据
		for (;;)
		{
			//loop until we block
			int rrc;
			int rrrc;


			do
			{
				char buffer[0x4000];
				char buffer2[0x4000];
				memset(buffer, '\0', sizeof(buffer));
				memset(buffer2, '\0', sizeof(buffer2));
				rrc = libssh2_channel_read(channel, buffer, sizeof(buffer));
				rrrc = libssh2_channel_read_stderr(channel, buffer2, sizeof(buffer2));

				Sleep(1000);

				if (rrc > 0)
				{
					int i;
					bytecount += rrc;
					//fprintf(stderr, "We read:\n");
					if (rrrc > 0)
					{
						for (i = 0; i < rrrc; ++i)
						{
							*err_result = *err_result + buffer2[i];
						}
					}
					for (i = 0; i < rrc; ++i)
					{
						*Result = *Result + buffer[i];
					}
					//fputc(buffer[i], stderr);
					//fprintf(stderr, "\n");
				}
				else
				{
					//if (rrc != LIBSSH2_ERROR_EAGAIN)
					//no need to output this for the EAGAIN case
					//fprintf(stderr, "libssh2_channel_read returned %d\n", rrc);

					if (rrc != LIBSSH2_ERROR_EAGAIN);
					break;
				}
			} while (rrc > 0);

			// this is due to blocking that would occur otherwise so we loop on this condition

			if (rrc == LIBSSH2_ERROR_EAGAIN)
			{
				waitsocket(sssock, session);
			}
			else
				break;
		}
		

		/*
		if (channel) {
			libssh2_channel_free(channel);

			channel = NULL;
		}
		//*/

		///*
		//关闭通道
		while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		if (rc == 0)
		{
			exitcode = libssh2_channel_get_exit_status(channel);
			libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
		}

		libssh2_channel_free(channel);
		channel = NULL;

		libssh2_session_disconnect(session,"Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(sssock);
		libssh2_exit();
		//*/
	}
	catch (...)
	{
		
		if (Link == true)
		{
			//关闭通道
			while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
			{
				waitsocket(sssock, session);
			}

			if (rc == 0)
			{
				exitcode = libssh2_channel_get_exit_status(channel);
				libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
			}

			libssh2_channel_free(channel);
			channel = NULL;

			libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
			libssh2_session_free(session);
			closesocket(sssock);
			libssh2_exit();
		}
		else
		{
			closesocket(sssock);
		}
		return false;
	}
	return true;
}


bool CSSH::SSH_Send_Command_No_password(ssh_config m_sshconfig, CString *ERROR_Info)
{
	//unsigned long hostaddr = 0;
	CString csData;

	//打开socket连接库
	err = WSAStartup(MAKEWORD(2, 0), &wsadata);
	//创建socket套字
	sssock = socket(AF_INET, SOCK_STREAM, 0);

	try
	{

		if (err != 0)
		{
			csData.Format(_T("%d"), err);
			*ERROR_Info = _T("打开socket连接库失败:") + csData;
			throw(0);
		}

		sin.sin_family = AF_INET;
		sin.sin_port = htons(22);
		InetPton(sin.sin_family, CString(m_sshconfig.hostname), &sin.sin_addr.s_addr);
		if (connect(sssock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
		{
			*ERROR_Info = _T("连接到ssh服务器失败:");
			throw(0);
		}

		//初始化
		session = libssh2_session_init();

		// tell libssh2 we want it all done non-blocking
		//libssh2_session_set_blocking(session, 0);

		/* ... start it up. This will trade welcome banners, exchange keys,
		* and setup crypto, compression, and MAC layers
		*/
		while ((rc = libssh2_session_handshake(session, sssock)) == LIBSSH2_ERROR_EAGAIN);
		if (rc)
		{
			csData.Format(_T("%d"), rc);
			*ERROR_Info = _T("socket和session握手通信失败:") + csData;
			throw(0);
			//fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
			//MessageBox(_T("socket和session握手通信失败"));
		}

		nh = libssh2_knownhost_init(session);


		//若密码不为空
		if (strlen(m_sshconfig.password) != 0)
		{
			//while ((rc = libssh2_userauth_password(session, m_sshconfig.username, m_sshconfig.password)) == LIBSSH2_ERROR_EAGAIN);
			while ((rc = libssh2_userauth_password_ex(session, m_sshconfig.username, strlen(m_sshconfig.username), NULL, 0, NULL)) == LIBSSH2_ERROR_EAGAIN);
			if (rc)
			{
				*ERROR_Info = _T("验证登陆失败");
				throw(0);
				//fprintf(stderr, "Authentication by password failed.\n");
				//goto shutdown;
				//MessageBox(_T("验证登陆失败"));
			}
		}
		//若密码为空
		///*
		else
		{
			while (rc = libssh2_userauth_keyboard_interactive(session, m_sshconfig.username, NULL) == LIBSSH2_ERROR_EAGAIN)
				if (rc)
				{
					*ERROR_Info = _T("验证登陆失败");
					throw(0);
					//fprintf(stderr, "Authentication by password failed.\n");
					//goto shutdown;
					//MessageBox(_T("验证登陆失败"));
				}

		}
		//*/
		char * userauthlist = libssh2_userauth_list(session, m_sshconfig.username, strlen(m_sshconfig.username));

		//打开通道
		///*
		while ((channel = libssh2_channel_open_session(session)) == NULL&&libssh2_session_last_error(session, NULL, NULL, 0)LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		//channel = libssh2_channel_open_session(session);
		if (channel == NULL)
		{
			*ERROR_Info = _T("打开通道失败");
			throw(0);
		}


		//发送指令
		//rc = libssh2_channel_write(channel, m_sshconfig.commandline, sizeof(m_sshconfig.commandline));
		///*
		while ((rc = libssh2_channel_exec(channel, m_sshconfig.commandline)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		Sleep(500);
		//*/
		if (rc != 0) {
			*ERROR_Info = _T("发送指令失败");
			throw(0);
		}
	}
	catch (...)
	{

		//关闭通道
		while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		if (rc == 0)
		{
			exitcode = libssh2_channel_get_exit_status(channel);
			libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
		}

		libssh2_channel_free(channel);
		channel = NULL;

		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(sssock);
		libssh2_exit();
		return false;
	}
	return true;
}


bool CSSH::SSH_Close()
{
	try
	{
		///*
		//关闭通道
		while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		if (rc == 0)
		{
			exitcode = libssh2_channel_get_exit_status(channel);
			libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
		}

		libssh2_channel_free(channel);
		channel = NULL;

		if (session !=NULL)
		{
			libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
			libssh2_session_free(session);
		}		
		closesocket(sssock);
		libssh2_exit();
		//*/
	}
	catch (...)
	{

		//关闭通道
		while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		if (rc == 0)
		{
			exitcode = libssh2_channel_get_exit_status(channel);
			libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
		}

		libssh2_channel_free(channel);
		channel = NULL;

		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(sssock);
		libssh2_exit();
		return false;
	}
	return true;
}


// 带登录用户名及密码登录SSH
bool CSSH::SSH_Query(ssh_config m_sshconfig, CString *ERROR_Info, CString *Result, CString *err_result, int sleep_time)
{
	//unsigned long hostaddr = 0;
	CString csData;
	*Result = _T("");
	*err_result = _T("");
	bool Link;

	try
	{
		//打开socket连接库
		err = WSAStartup(MAKEWORD(2, 0), &wsadata);
		//创建socket套字
		sssock = socket(AF_INET, SOCK_STREAM, 0);

		if (err != 0)
		{
			csData.Format(_T("%d"), err);
			*ERROR_Info = _T("打开socket连接库失败:") + csData;
			throw(0);
		}

		sin.sin_family = AF_INET;
		sin.sin_port = htons(22);
		InetPton(sin.sin_family, CString(m_sshconfig.hostname), &sin.sin_addr.s_addr);
		if (connect(sssock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
		{
			*ERROR_Info = _T("连接到ssh服务器失败:");
			Link = false;
			throw(0);
		}
		Link = true;

		//初始化
		session = libssh2_session_init();

		while ((rc = libssh2_session_handshake(session, sssock)) == LIBSSH2_ERROR_EAGAIN);
		if (rc)
		{
			csData.Format(_T("%d"), rc);
			*ERROR_Info = _T("socket和session握手通信失败:") + csData;
			throw(0);
			//fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
			//MessageBox(_T("socket和session握手通信失败"));
		}

		nh = libssh2_knownhost_init(session);


		//若密码不为空
		if (strlen(m_sshconfig.password) != 0)
		{
			while ((rc = libssh2_userauth_password(session, m_sshconfig.username, m_sshconfig.password)) == LIBSSH2_ERROR_EAGAIN);
			//while ((rc = libssh2_userauth_password_ex(session, m_sshconfig.username, strlen(m_sshconfig.username), NULL, 0, NULL)) == LIBSSH2_ERROR_EAGAIN);
			if (rc)
			{
				*ERROR_Info = _T("验证登陆失败");
				throw(0);
			}
		}
		//若密码为空
		///*
		else
		{
			while (rc = libssh2_userauth_keyboard_interactive(session, m_sshconfig.username, NULL) == LIBSSH2_ERROR_EAGAIN)
				if (rc)
				{
					*ERROR_Info = _T("验证登陆失败");
					throw(0);
				}

		}
		//*/
		//char * userauthlist = libssh2_userauth_list(session, m_sshconfig.username, strlen(m_sshconfig.username));

		//打开通道
		///*
		while ((channel = libssh2_channel_open_session(session)) == NULL&&libssh2_session_last_error(session, NULL, NULL, 0)LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		
		if (channel == NULL)
		{
			*ERROR_Info = _T("打开通道失败");
			throw(0);
		}


		//发送指令
		rc = libssh2_channel_write(channel, m_sshconfig.commandline, sizeof(m_sshconfig.commandline));
		///*
		while ((rc = libssh2_channel_exec(channel, m_sshconfig.commandline)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		if (rc != 0) {
			*ERROR_Info = _T("发送指令失败");
			throw(0);
		}

		Sleep(sleep_time);


		///*
		//读数据
		for (;;)
		{
			//loop until we block
			int rrc;
			int rrrc;
			do
			{
				char buffer[0x4000];
				char buffer2[0x4000];
				memset(buffer, '\0', sizeof(buffer));
				memset(buffer2, '\0', sizeof(buffer2));
				rrc = libssh2_channel_read(channel, buffer, sizeof(buffer));
				rrrc = libssh2_channel_read_stderr(channel, buffer2, sizeof(buffer2));
				if (rrc > 0)
				{
					int i;
					bytecount += rrc;
					//fprintf(stderr, "We read:\n");
					if (rrrc > 0)
					{
						for (i = 0; i < rrrc; ++i)
						{
							*err_result = *err_result + buffer[i];
						}
					}
					for (i = 0; i < rrc; ++i)
					{
						*Result = *Result + buffer[i];
					}
				}
				else
				{
					if (rrc != LIBSSH2_ERROR_EAGAIN);
					break;
				}
			} while (rrc > 0);

			// this is due to blocking that would occur otherwise so we loop on this condition

			if (rrc == LIBSSH2_ERROR_EAGAIN)
			{
				waitsocket(sssock, session);
			}
			else
				break;
		}
		//*/

		///*
		//关闭通道
		while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		if (rc == 0)
		{
			exitcode = libssh2_channel_get_exit_status(channel);
			libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
		}

		libssh2_channel_free(channel);
		channel = NULL;

		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(sssock);
		libssh2_exit();
		//*/
	}
	catch (...)
	{

		if (Link == true)
		{
			//关闭通道
			while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
			{
				waitsocket(sssock, session);
			}

			if (rc == 0)
			{
				exitcode = libssh2_channel_get_exit_status(channel);
				libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
			}

			libssh2_channel_free(channel);
			channel = NULL;

			libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
			libssh2_session_free(session);
			closesocket(sssock);
			libssh2_exit();
		}
		else
		{
			closesocket(sssock);
		}
		return false;
	}
	return true;
}


bool CSSH::SSH_Send_Command(ssh_config m_sshconfig, CString *ERROR_Info)
{
	CString csData;

	//打开socket连接库
	err = WSAStartup(MAKEWORD(2, 0), &wsadata);
	//创建socket套字
	sssock = socket(AF_INET, SOCK_STREAM, 0);

	try
	{

		if (err != 0)
		{
			csData.Format(_T("%d"), err);
			*ERROR_Info = _T("打开socket连接库失败:") + csData;
			throw(0);
		}

		sin.sin_family = AF_INET;
		sin.sin_port = htons(22);
		InetPton(sin.sin_family, CString(m_sshconfig.hostname), &sin.sin_addr.s_addr);
		if (connect(sssock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0)
		{
			*ERROR_Info = _T("连接到ssh服务器失败:");
			throw(0);
		}

		//初始化
		session = libssh2_session_init();


		/* ... start it up. This will trade welcome banners, exchange keys,
		* and setup crypto, compression, and MAC layers
		*/
		while ((rc = libssh2_session_handshake(session, sssock)) == LIBSSH2_ERROR_EAGAIN);
		if (rc)
		{
			csData.Format(_T("%d"), rc);
			*ERROR_Info = _T("socket和session握手通信失败:") + csData;
			throw(0);
		}

		nh = libssh2_knownhost_init(session);


		//若密码不为空
		if (strlen(m_sshconfig.password) != 0)
		{
			while ((rc = libssh2_userauth_password(session, m_sshconfig.username, m_sshconfig.password)) == LIBSSH2_ERROR_EAGAIN);
			//while ((rc = libssh2_userauth_password_ex(session, m_sshconfig.username, strlen(m_sshconfig.username), NULL, 0, NULL)) == LIBSSH2_ERROR_EAGAIN);
			if (rc)
			{
				*ERROR_Info = _T("验证登陆失败");
				throw(0);
			}
		}
		//若密码为空
		///*
		else
		{
			while (rc = libssh2_userauth_keyboard_interactive(session, m_sshconfig.username, NULL) == LIBSSH2_ERROR_EAGAIN)
				if (rc)
				{
					*ERROR_Info = _T("验证登陆失败");
					throw(0);
				}

		}
		//*/
		//char * userauthlist = libssh2_userauth_list(session, m_sshconfig.username, strlen(m_sshconfig.username));

		//打开通道
		///*
		while ((channel = libssh2_channel_open_session(session)) == NULL&&libssh2_session_last_error(session, NULL, NULL, 0)LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		if (channel == NULL)
		{
			*ERROR_Info = _T("打开通道失败");
			throw(0);
		}


		//发送指令
		rc = libssh2_channel_write(channel, m_sshconfig.commandline, sizeof(m_sshconfig.commandline));
		///*
		while ((rc = libssh2_channel_exec(channel, m_sshconfig.commandline)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}
		//*/
		if (rc != 0) {
			*ERROR_Info = _T("发送指令失败");
			throw(0);
		}
	}
	catch (...)
	{

		//关闭通道
		while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
		{
			waitsocket(sssock, session);
		}

		if (rc == 0)
		{
			exitcode = libssh2_channel_get_exit_status(channel);
			libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
		}

		libssh2_channel_free(channel);
		channel = NULL;

		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(sssock);
		libssh2_exit();
		return false;
	}
	return true;
}
