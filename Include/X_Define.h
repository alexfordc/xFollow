#ifndef X_DEFINE_HEADER
#define X_DEFINE_HEADER


///ȫ���ɽ�
#define OST_AllTraded '0'
///���ֳɽ�
#define OST_PartTradedQueueing '1'
///δ�ɽ�
#define OST_NoTradeQueueing '2'
///���ڶ�����
#define OST_NotQueueing '3'
///����
#define OST_Canceled '4'
///ʧ��
#define OST_Failed '5'


struct x_stuUserLogin
{
	int id;
	int apiID;
	char ip1[16];
	int port1;
	char ip2[16];
	int port2;
	char ip3[16];
	int port3;
	char brokerID[11];
	bool isFollow;
	char accountID[32];
	char password[32];
};


#endif //X_DEFINE_HEADER
