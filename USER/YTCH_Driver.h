

#define TIME_OUT_LIMIT    1000   // 100ms

#define SUCCESS         0       // ��ȡ����д����ȷ
#define WRONG_LENGTH   -1       // ������������ݳ��ȴ���
#define WRONG_RESPONSE   -2     //���յ�����Ļ�Ӧ ���������� CRC�����
#define EXCEPTION_RESPONSE   -3 // ����������ָ����� �����ַ�����
#define TIME_OUT             -4 // ����һ��ʱ��û���յ��ظ��� �ظ�������ȷ�Ļظ��ʹ���ظ�

#define PROFILE_POSITION_MODE  1   // �滮λ��ģʽ
#define PROFILE_VELOCITY_MODE  3   // �滮�ٶ�ģʽ
#define HOMING_MODE            6   // ����
#define POSITION_MODE          -1  // λ��ģʽ,�޹켣�滮
#define VELOCITY_MODE          -2  // �ٶ�ģʽ,�޹켣�滮
#define CURRENT_MODE           -3  // ����ģʽ,Ť��ģʽ
#define STEP_DIRECTION_MODE    -6  // ���巽�����

//״̬��λ
#define TARGET_REACHED_BIT 0x0400


/*  Function Codes 
   ---------------
  defined in the canopen DS301 
*/
#define NMT	   0x0
#define SYNC       0x1
#define TIME_STAMP 0x2
#define PDO1tx     0x3
#define PDO1rx     0x4
#define PDO2tx     0x5
#define PDO2rx     0x6
#define PDO3tx     0x7
#define PDO3rx     0x8
#define PDO4tx     0x9
#define PDO4rx     0xA
#define SDOtx      0xB
#define SDOrx      0xC
#define NODE_GUARD 0xE
#define LSS 	   0xF


int8_t Read_Driver_Reg(uint8_t dev_addr,uint16_t index, uint8_t subindex ,uint8_t *data, uint8_t len);
int8_t Write_Driver_Reg(uint8_t dev_addr,uint16_t index, uint8_t subindex ,uint8_t *data, uint8_t len);

int8_t Enable_Driver(uint8_t dev_addr);
int8_t Disable_Driver(uint8_t dev_addr);
int8_t Read_Statusword(uint8_t dev_addr,uint16_t *statusword);
int8_t Switch_Driver_Mode(uint8_t dev_addr,int8_t mode);
int8_t PVM_Set_Speed(uint8_t dev_addr,int32_t speed);
int8_t PPM_Set_Position(uint8_t dev_addr,int32_t position,uint8_t relative);
int8_t Read_Position(uint8_t dev_addr,int32_t *position);
