#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0		//MPU6050��I2C�ӻ���ַ


void MPU6050_WaitEVent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t TimeOut = 10000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		TimeOut--;
		if(TimeOut == 0)
		{
			break;
		}
	}
}
//MPU6050д�Ĵ���
//ָ����ַд,������������ʱ��ͼ
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);//��ʼ
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�����ev_5
	//Ӳ�����ʹӻ���ַ
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//�ȴ��¼�EV6
	//���ͼĴ�����ַ
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//�ȴ�EV8�¼�
	//��������
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//�ȴ�EV8_2�¼�
	
	I2C_GenerateSTOP(I2C2, ENABLE);//����
	
}

//���Ĵ���--ָ����ַ��
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	I2C_GenerateSTART(I2C2, ENABLE);//��ʼ
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//�ȴ�����ev_5
	//Ӳ�����ʹӻ���ַ
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//�ȴ��¼�EV6
	//���ͼĴ�����ַ
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//�ȴ�EV8�¼�
	//Ӳ��I2C�����ظ���ʼ����
	I2C_GenerateSTART(I2C2, ENABLE);										
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//�ȴ�EV5
	//Ӳ��I2C���ʹӻ���ַ������Ϊ����
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);		
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		//�ȴ�EV6
		//�ڽ������һ���ֽ�֮ǰ��ǰ��Ӧ��ʧ��
	I2C_AcknowledgeConfig(I2C2, DISABLE);								
	I2C_GenerateSTOP(I2C2, ENABLE);											//�ڽ������һ���ֽ�֮ǰ��ǰ����ֹͣ����
	
	MPU6050_WaitEVent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//�ȴ�EV7
	Data = I2C_ReceiveData(I2C2);											//�������ݼĴ���
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);			
	return Data;
}

//MPU6050��ʼ��
void MPU6050_Init(void)
{
	//����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//gpio��ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct); 
	//I2c��ʼ��
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 50000;//ʱ���ٶ�50khz
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;//ʱ��ռ�ձȣ��ͣ��� = 2
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;//�����ַ�ӻ�ģʽ����Ч
	I2C_Init(I2C2, &I2C_InitStruct);
	//I2Cʹ��
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//��Դ����Ĵ���1��ȡ��˯��ģʽ��ѡ��ʱ��ԴΪX��������
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		//��Դ����Ĵ���2������Ĭ��ֵ0���������������
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//�����ʷ�Ƶ�Ĵ��������ò�����
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//���üĴ���������DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//���������üĴ�����ѡ��������Ϊ��2000��/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	//���ٶȼ����üĴ�����ѡ��������Ϊ��16g
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);		//����WHO_AM_I�Ĵ�����ֵ
}

/**
  * ��    ����MPU6050��ȡ����
  * ��    ����AccX AccY AccZ ���ٶȼ�X��Y��Z������ݣ�ʹ�������������ʽ���أ���Χ��-32768~32767
  * ��    ����GyroX GyroY GyroZ ������X��Y��Z������ݣ�ʹ�������������ʽ���أ���Χ��-32768~32767
  * �� �� ֵ����
  */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//�������ݸ�8λ�͵�8λ�ı���
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//��ȡ���ٶȼ�X��ĸ�8λ����
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//��ȡ���ٶȼ�X��ĵ�8λ����
	*AccX = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);		//��ȡ���ٶȼ�Y��ĸ�8λ����
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);		//��ȡ���ٶȼ�Y��ĵ�8λ����
	*AccY = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);		//��ȡ���ٶȼ�Z��ĸ�8λ����
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);		//��ȡ���ٶȼ�Z��ĵ�8λ����
	*AccZ = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);		//��ȡ������X��ĸ�8λ����
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);		//��ȡ������X��ĵ�8λ����
	*GyroX = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);		//��ȡ������Y��ĸ�8λ����
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);		//��ȡ������Y��ĵ�8λ����
	*GyroY = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);		//��ȡ������Z��ĸ�8λ����
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);		//��ȡ������Z��ĵ�8λ����
	*GyroZ = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
}
