#if !defined(ErrorCode_define)
#define ErrorCode_define

//�������б�
//2101, �޸�ҩƷ��Ϣ
//2102, ����ҩƷ��Ϣ
//2103, ɾ��ҩƷ��Ϣ
//2104, �޸�ҩƷ��ַ
//2105, ����ҩƷ��ַ
//2106, ���ҩƷ��ַ
//2201, �����豸����ʱ��
//2202, ����������ӡ������
//2203, ������ת��ӡ������
//2204, ���ð�װ������
//2401, ҩƷ����
//2402, ��������

//////////////////////////////////////////////////////////////////////////
/*�û�����ģ�鷵�ش�������*/
#define ER_LIST_USERS					100001	//�г��û���Ϣʧ��
#define ER_ADD_USER						100002	//�����û�ʧ��
#define ER_OLD_PASSWORD					100003	//����˶Դ��� 
#define ER_LIST_GROUP					100004	//�г��û�ʧ��
#define ER_USER_EXISTED					100005	//�û��Ѿ����� 
#define ER_IKEY_BOUND					100006	//��iKey�Ѿ��󶨵������û�

/*���ܹ�ģ�鷵�ش�������*/
#define ER_OPEN_DEVICE				110001 	//��ikeyʧ��
#define ER_CLOSE_DEVICE				110002	//�ر�ikeyʧ��
#define ER_VERIFY_USER_PIN			110003	//�û�PIN��У��ʧ��
#define ER_HANDLE_IS_NULL			110004	//�豸���Ϊ��
#define ER_CREATE_APP_DIR			110005	//����Ӧ�ó���Ŀ¼ʧ��
#define ER_CREATE_DATA_FILE			110006	//���������ļ�ʧ��
#define ER_CREATE_MD5_HMAC_KEY		110007	//����MD5_HMAC_KEY�ļ�ʧ��
#define ER_MD5_HMAC_AUTHENTICATE	110008	//ǩ��MD5_HMACʧ��
#define ER_OPEN_DIR					110009	//��Ŀ¼ʧ��
#define ER_GET_USER					110010	//ȡ���û���ʧ��

#define ER_GET_LICENSE				111001	//��ȡ���ܹ����ʧ��
#define ER_QUERY_FEATURE			111002	//��ѯ������ʧ��
#define ER_QUERY_RESPONSE			111003	//��ѯ��Ӧ����

/*ϵͳ����ģ�鷵�ش�������*/
#define ER_NO_CONFIG_FILE	120001	//û�������ļ�
#define ER_CONFIG_FILE		120002	//�����ļ�����
#define ER_LOAD_CONFIG_FILE	120003	//���������ļ�ʧ��
#define ER_UPDATE_CONFIG	120004	//���������ļ�ʧ��

/*��������ģ�鷵�ش�������*/
#define ER_NO_PATIENT_INFO				130001	//û�л�����Ϣ
#define ER_NO_MEDICINE_INFO				130002	//û��ҩƷ��Ϣ
#define ER_SAVE_PRESCRIPTION			130003	//���洦��ʧ��
#define ER_SAVE_PATIENT					130004	//���没��ʧ��
#define ER_SAVE_PRESCRIPTION_MEDICINE	130005	//����ҩƷʧ��
#define ER_INPUT_PRESCRIPTION			130006	//¼�봦��ʧ��
#define ER_QUERY_PRESCRIPTION			130007	//������ѯʧ��
#define ER_GET_PATIENT					130008	//��ȡ������Ϣʧ��
#define ER_GET_PRESCRIPTION_MEDICINE	130009	//��ȡ����ҩƷ��Ϣʧ��
#define ER_MODIFY_PRESCRIPTION			130010	//�޸Ĵ���ʧ��
#define ER_MODIFY_PRESCRIPTION_MEDICINE	130011	//�޸Ĵ���ҩƷ��Ϣʧ��
#define ER_MODIFY_PRESCRIPTION_PATIENT	130012	//�޸Ĵ���������Ϣʧ��
#define ER_DEL_PRESCRIPTION				130013	//ɾ������ʧ��
#define ER_DEL_PRESCRIPTION_MEDICINE	130014	//ɾ������ҩƷ��Ϣʧ��
#define ER_DEL_PATIENT					130015	//ɾ��������Ϣʧ��

/*���ݿ����ģ�鷵�ش�������*/
#define ER_DB_INIT						140001	//���ݿ��ʼ��ʧ��
#define ER_DB_CONNECT					140002	//���ݿ�����ʧ��
#define WM_NO_SQLTEXT					140003	//�����豸״̬�����߳�ʧ��
#define ER_EXECUTE_SQL					140004	//ִ��SQL������
#define WM_DATA_END						140005  //����������һ�е���һ��
#define ER_NO_DATASET					140006	//���ݼ�Ϊ��
#define ER_DB_NOT_OPEN					140007  //���ݿ�û�д�
#define ER_SERVER_STATE					140008	//���ݿ����������״̬����
#define ER_DB_PARAM						140009	//�������ݿ���Ϊ��
#define ER_DB_BACKUP					140010	//�������ݿ����
#define ER_DB_RECOVER					140011	//�ָ����ݿ����
#define ER_UPDATE_TABLE					140012	//���±�ʧ��

/*������ӡ��ģ�鷵�ش�������*/
#define ER_PRINTER_PORT_NUM				150001	//��ӡ���˿ںŴ���
#define ER_OPEN_PRINTER_PORT			150002	//��ӡ���˿ڴ�ʧ��
#define ER_NO_BASKET_ID					150003	//û�����ӿ���
#define ER_PRINT_PRESCRIPTION			150004	//��ӡ����ʧ��
#define ER_PRESCRIPTION_ID				150005	//ͨ�������Ŵ�ӡ����ʧ��
#define ER_PRINT_HETEROMORPHISM			150006	//��ӡ����ҩƷ��Ϣʧ��

#define ER_NO_PAPER						150007	//ֽ��
#define ER_LESS_PAPER					150008	//ֽ����
#define ER_CUT					        150009	//�е�����
#define ER_HOT					        150010	//����ͷ�¶ȹ���
#define ER_NO_BASKET_NUMBER             150011  //û�������������

//CThermalPrinterStar
//#define ER_PRINTER_PORT_NUM				151001	//��ӡ���˿ںŴ���
//#define ER_OPEN_PRINTER_PORT			151002	//��ӡ���˿ڴ�ʧ��
#define WN_NOT_OPEN_PORT				151003	//��ӡ���˿�δ��
#define ER_PRINT						151004	//��ӡʧ��
#define ER_GET_PRINTER_STATE			151005	//��ȡ��ӡ��״̬ʧ��
#define ER_SEND_COMMAND					151006	//���ʹ�ӡ��ָ��ʧ��

#define ER_PORT_NUM				159001	//�������˿ںŴ���
#define ER_OPEN_PORT			159002	//�������˿ڴ�ʧ��
#define ER_WRITE_DATA			159003	//дͨѶ����ʧ��
#define ER_READ_DATA			159004	//���˿�����ʧ��
#define ER_NOT_OPEN_PORT		159005	//�������˿�û�д�
#define ER_CREATE_LISTEN_THREAD	159006	//���������������߳�ʧ��
#define ER_WRITE_CARDATA        159007  //дIC������ʧ��

/*PLCģ�鷵�ش�������*/
#define ER_MEDICINE_POSITION			160001	//ҩƷλ����Ϣ����
#define ER_MEDICINE_COUNT				160002	//ҩƷ��Ŀ����
#define ER_SUSPEND_LISTEN_THREAD		160003	//��ֹ�豸״̬�����߳�ʧ��
#define ER_CLOSE_LISTEN_THREAD			160004	//�ر��豸״̬�����߳̾��ʧ��
#define ER_NO_PLC						160005  //û��PLCʵ��
#define ER_NOT_OPEN_PLC					160006  //PLC����û�д�
#define ER_PLC_SENDBASKETFINISH         160007  //�ȴ�PLC������������źų�ʱ
//////////////////////////////////////////////////////////////////////////

#define FILLLIST_EMPTY                   170001		//��ҩ����Ϊ��
#define FILLMED_ERROR                    170002     //��ҩʧ��
#define FINISHFILLMED_ERROR              170003     //��ɼ�ҩʧ��

#define FAILED_TO_CREATE_THREAD			 180001     //�����߳�ʧ��
#define FAILED_TO_RESUME_THREAD			 180002		//�ָ��߳�ʧ��
#define FAILED_TO_SUSPEND_THREAD		 180003		//�ж��߳�ʧ��
#define FAILED_TO_CLOSE_HANDLE			 180004		//�ر��߳�ʧ��
#define NOT_ENOUGH_MEDICINE				 180005		//ҩƷ��������
#define CAN_NOT_FIND_NORMAL_ID           180006		//��ѯ�����������д���ID
#define CAN_NOT_FIND_ABNORMAL_ID         180007		//��ѯ�����쳣���д���ID
#define READ_BASKET_OVER_TIME			 180008     //�����ӳ�ʱ
#define READ_BASKET_ERROR				 180009     //��ȡ�������ӱ��
#define DIS_ERROR						 1801       //��ҩʧ�ܲ�����
#define MACHINE_DISABLE					 180010     //�豸δ����
#define MACHINE_FETAL_ERROR				 180011     //�豸���ع���
#define DISTRIBUTE_NUM_ERROR			 180012     //��ҩ��������
#define NO_FIT_PRESCRIPTION				 180013		//û�з��������Ĵ�����¼


/*ҩƷ�����෵�ش�������*/
#define ERROR_MedicineManage_Initialize              210001  //��ʼ������ҩƷλ��״̬ʧ��
#define ERROR_MedicineManage_PopMedicinLocation      210002  //��ҩʱ�޸�ҩƷλ��״̬ʧ��
#define ERROR_MedicineManage_PushMedicinLocation     210003  //��ҩʱ�޸�ҩƷλ��״̬ʧ��
#define ERROR_MedicineManage_AddressSearch           210004  //����ҩƷ�ĵ�ַ�б�ʧ��
#define ERROR_MedicineManage_GetMedicineCount        210005  //��ȡҩƷ��ǰ��ʣ�������ʧ��
#define ERROR_MedicineManage_FastSearch              210006  //������ٲ���ʧ��
#define ERROR_MedicineManage_ListSearch              210007  //����ҩƷ�б�ʧ��
#define ERROR_MedicineManage_LookMedicine            210008  //����ҩƷ��ϸ��Ϣʧ��
#define ERROR_MedicineManage_ModifyMedicine          210009  //�޸�ҩƷ��ϸ��Ϣʧ��
#define ERROR_MedicineManage_AddMedicine             210010  //����ҩƷ��ϸ��Ϣʧ��
#define ERROR_MedicineManage_DeleteMedicine          210011  //ɾ��ҩƷ��ϸ��Ϣʧ��
#define ERROR_MedicineManage_ClearLocation           210012  //���ҩƷ������ַ��Ϣʧ��
#define ERROR_MedicineManage_ClearAllLocation        210013  //���ҩƷ���е�ַ��Ϣʧ��
#define ERROR_MedicineManage_getMaxmedID             210014  //��ȡҩƷ���ID��ʧ��
#define ERROR_MedicineManage_getmedID                210015  //��ȡҩƷID��ʧ��
#define ERROR_MedicineManage_light_position          210016  //��ȡָʾ�ƺ�ʧ��
#define ERROR_MedicineManage_MedicineOverdue         210017  //��ǰҩƷ�Ѿ�����
#define ERROR_MedicineManage_modifyMedicinLocation   210018  //�޸�ҩƷλ��״̬����
#define ERROR_MedicineManage_QueryMedInfo_Client     210019  //��ѯ�ͻ�������ҩƷ��Ϣʧ��
#define ERROR_MedicineManage_QueryMedleavingPercent  210020  //��ѯҩƷʣ��ٷֱ�ʧ��
#define ERROR_MedicineManage_findDeviceinfo          210021  //��ѯ�豸��Ϣʧ��
#define ERROR_MedicineManage_saveDeviceinfo          210022  //�����豸��Ϣʧ��
#define ERROR_MedicineManage_setMachineState         210023  //�����豸״̬ʧ��
#define ERROR_MedicineManage_getAllMachineState      210024  //��ȡ�����豸״̬ʧ��
#define ERROR_MedicineManage_getMachineState         210025  //��ȡ�豸״̬ʧ��
#define ERROR_MedicineManage_getWindowfalg           210026  //��ȡ���ڱ���״̬ʧ��
#define ERROR_MedicineManage_setWindowfalg           210027  //���ô���״̬ʧ��
#define ERROR_MedicineManage_getmachinefalg          210028  //��ȡ�豸״̬ʧ��
#define ERROR_MedicineManage_getMedWeight            210029  //��ȡҩƷȨ��ʧ��
#define ERROR_MedicineManage_getWindowCount          210030  //��ȡ������ʧ��
   

/*ȫ��ͳ���෵�ش�������*/
#define ERROR_Statistic_SumLayMedicinCount     220001  //�����ܰ�ҩ����ʧ��
#define ERROR_Statistic_SumPrescriptionCount   220002  //�����ܴ�������ʧ��
#define ERROR_Statistic_SumDeviceRunTime       220003  //�������豸����ʱ������ʧ��
#define ERROR_Statistic_SumLogCount            220004  //�����ܵ�¼����ʧ��
#define ERROR_Statistic_SumPrintCount          220005  //������������ӡ����ʧ��
#define ERROR_Statistic_SumSatoPrintCount      220006  //��������ת��ӡ����ʧ��
#define ERROR_Statistic_SumPackCount           220007  //�����ܰ�װ����ʧ��
#define ERROR_Statistic_AddMedicinSortd        220008  //����ҩƷ����ͳ��ʧ��
#define ERROR_Statistic_AddLocation            220009  //����λ��ͳ��ʧ��
#define ERROR_Statistic_GetMedicinSort         220010  //���ҩƷ����ͳ��ʧ��
#define ERROR_Statistic_GetLocation            220011  //�����ַ����ͳ��ʧ��
#define ERROR_Statistic_Initialize             220012  //��ʼ��ȫ��ͳ������ʧ��
#define ERROR_Statistic_SetBooking             220013  //����Ԥ����ʧ��
#define ERROR_Statistic_GetBooking             220014  //���Ԥ����ʧ��

/*���ϵͳ�෵�ش�������*/
#define ERROR_Monitor_CreatThead        230001  //�����豸����߳�ʧ��
#define ERROE_Monitor_GetCameraState    230002  //���������ʧ��
#define ERROE_Monitor_GetScannerState   230003  //����ɨ����ʧ��
#define ERROE_Monitor_GetPLCState       230004  //����PLCʧ��
#define ERROE_Monitor_GetPrintState     230005  //���Ӵ�ӡ��ʧ��

/*�������뷵�ش�������*/
#define ERROR_Import_Medicine          240001  //��������ҩƷʧ��
#define ERROR_Import_Prescription      240002  //�������봦��ʧ��

//////////////////////////////////////////////////////////////////////////

#endif
