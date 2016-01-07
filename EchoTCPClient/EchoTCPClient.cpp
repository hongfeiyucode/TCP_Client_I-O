// EchoTCPClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SocketFrame.h"
#include <iostream>
using namespace std;
int tcp_client_fun_echo(FILE *fp,SOCKET s);
#define ECHOPORT "7210"
int main(int argc, char* argv[])
{
	CSocketFrame frame;
	int iResult;
	SOCKET ClientSocket;
	char *ip="127.0.0.1";

	cout << "EchoTCPClient��������ʼ������������������ӡ�����\n";
	
	//Windows Sockets Dll��ʼ��
	frame.start_up();
	
	//�����ͻ��˵���ʽ�׽��֣������������������
    printf("���ӽ����ɹ����Ƿ��������ͬ��ʱ�䣿(Y/N) ");
	ClientSocket = frame.tcp_client( ip, ECHOPORT );
	if ( ClientSocket == -1 )
		return -1;

	//��ʼ��������ķ��������
	iResult = tcp_client_fun_echo(stdin,ClientSocket);

	frame.quit( ClientSocket );
    return iResult; 
}

/********************************************************
��������tcp_client_fun_echo
���������FILE *fp:ָ��FILE���͵Ķ���
          SOCKET s:�ͻ��˵������׽���
���������0���ɹ���-1��ʧ��
���ܣ�tcp_client����ͻ��˵ľ��幦�ܺ���
*********************************************************/
int tcp_client_fun_echo(FILE *fp,SOCKET s)
{
	int iResult;
    char sendline[MAXLINE],recvline[MAXLINE];
	memset(sendline,0,MAXLINE);
	memset(recvline,0,MAXLINE);

	//ѭ�������û����������ݣ������շ��������ص�Ӧ��ֱ���û�����"Q"����
	while(fgets(sendline,MAXLINE,fp)!=NULL)
	{
		if( *sendline == 'n'|| *sendline == 'N'){
			printf("�Ͽ����� �˳�����!\n");
			// ���ݷ��ͽ���������shutdown()�����������ٷ������ݣ���ʱ�ͻ����Կ��Խ�������
			iResult = shutdown(s, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				printf("shutdown failed with error: %d\n", WSAGetLastError());
			}
			return 0;
		}
		iResult = send(s,sendline,strlen(sendline),0);
		if(iResult == SOCKET_ERROR)
		{
			printf("send �������ô��󣬴���ţ� %ld\n", WSAGetLastError());
			return -1;
		}

	    memset(recvline,0,MAXLINE);
		iResult = recv( s, recvline, MAXLINE,0 ) ;
		if (iResult > 0)
		{
			printf("�������ͬ��ʱ��ɹ�����ǰʱ��Ϊ��%s \r\n", recvline);
			printf("\n------------------------\n�Ƿ�����������ͬ��ʱ�䣿(Y/N) ");
		}
		else{
			if ( iResult ==0 )
               printf("��������ֹ!\n");
			else
 	            printf("recv �������ô��󣬴����: %d\n", WSAGetLastError());
			break;
		}
		memset(sendline,0,MAXLINE);
	}
	return iResult;
}