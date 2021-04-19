#include "SerialPort.h"
#include "DataTrans.h"

using namespace std;

extern int Filerecv_num;
extern int Mode;
char path_recv[1000];
serialPort::serialPort()
{
	fd = -1;
}
bool serialPort::OpenPort(const char* dev)
{

	char* _dev = new char[32];
	strcpy(_dev, dev);
	fd = open(_dev, O_RDWR);          //| O_NOCTTY | O_NDELAY   
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return false;
	}

	int DTR_flag;
	DTR_flag = TIOCM_DTR;
	ioctl(fd, TIOCMBIS, &DTR_flag); //Set RTS pin
	return true;

}
int serialPort::setup(int speed, int flow_ctrl, int databits, int stopbits, int parity)
{

	int   i;
	int   status;
	struct termios options;
	/*tcgetattr(fd,&options)�õ���fdָ��������ز������������Ǳ�����options,�ú��������Բ��������Ƿ���ȷ���ô����Ƿ���õȡ������óɹ�����������ֵΪ0��������ʧ�ܣ���������ֵΪ1.
	*/
	if (tcgetattr(fd, &options) != 0)
	{
		perror("SetupSerial 1");
		return (false);
	}

	//���ô������벨���ʺ����������  
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
	{
		if (speed == name_arr[i])
		{
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}

	//�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���  
	options.c_cflag |= CLOCAL;
	//�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������  
	options.c_cflag |= CREAD;

	//��������������  
	switch (flow_ctrl)
	{

	case 0://��ʹ��������  
		options.c_cflag &= ~CRTSCTS;
		break;

	case 1://ʹ��Ӳ��������  
		options.c_cflag |= CRTSCTS;
		break;
	case 2://ʹ�����������  
		options.c_cflag |= IXON | IXOFF | IXANY;
		break;
	}
	//��������λ  
	//����������־λ  
	options.c_cflag &= ~CSIZE;
	switch (databits)
	{
	case 5:
		options.c_cflag |= CS5;
		break;
	case 6:
		options.c_cflag |= CS6;
		break;
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "Unsupported data size\n");
		return (false);
	}
	//����У��λ  
	switch (parity)
	{
	case 'n':
	case 'N': //����żУ��λ��  
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O'://����Ϊ��У��      
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E'://����ΪżУ��    
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's':
	case 'S': //����Ϊ�ո�   
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported parity\n");
		return (false);
	}
	// ����ֹͣλ   
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB; break;
	case 2:
		options.c_cflag |= CSTOPB; break;
	default:
		fprintf(stderr, "Unsupported stop bits\n");
		return (false);
	}

	//�޸����ģʽ��ԭʼ�������  
	options.c_oflag &= ~OPOST;

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	//options.c_lflag &= ~(ISIG | ICANON);  

	//���õȴ�ʱ�����С�����ַ�  
	options.c_cc[VTIME] = 1; /* ��ȡһ���ַ��ȴ�1*(1/10)s */
	options.c_cc[VMIN] = 1; /* ��ȡ�ַ������ٸ���Ϊ1 */
	//�����������������������ݣ����ǲ��ٶ�ȡ ˢ���յ������ݵ��ǲ���  
	tcflush(fd, TCIFLUSH);

	//�������� (���޸ĺ��termios�������õ������У�  
	if (tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("com set error!\n");
		return (false);
	}
	return (true);
}

int serialPort::readBuffer(uint8_t* buffer, int size, int num)
{
	int i;
	//unsigned char buff[1024];
	int nread = read(fd, buffer, size); //�����ֽ���
	for (i = 0; i < nread; i++)
	{
		ofstream OutFile;
		if (Filerecv_num >= num)
		{
			switch (Mode)
			{
			case 1:
				sprintf(path_recv, "test%d.txt", Filerecv_num - num);
				break;
			case 2:
				sprintf(path_recv, "test%d.txt", 20000 + Filerecv_num - num);
				break;
			default:
				perror("Mode error!");
				break;
			}
		}
		else
		{
			switch (Mode)
			{
			case 1:
				sprintf(path_recv, "%d.txt", Filerecv_num);
				break;
			case 2:
				sprintf(path_recv, "%d.txt", 20000 + Filerecv_num);
				break;
			default:
				perror("Mode error!");
				break;
			}
		}

		OutFile.open(path_recv, ios::app);

		if (buffer[i] == 'D')
		{
			Data_recv(++Filerecv_num);
		}
		else
		{
			OutFile << (char)buffer[i];
		}
		OutFile.close();
		//printf("%.*s", 8, buff);
	}
	return 1;
}


int serialPort::writeBuffer(uint8_t* buffer, int size)
{
	return write(fd, buffer, size);
}
uint8_t serialPort::getchar()
{
	uint8_t t;
	read(fd, &t, 1);
	return t;
}
void serialPort::ClosePort()
{
	close(fd);
}