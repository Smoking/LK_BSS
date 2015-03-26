#if !defined(ErrorCode_define)
#define ErrorCode_define

//操作码列表
//2101, 修改药品信息
//2102, 新增药品信息
//2103, 删除药品信息
//2104, 修改药品地址
//2105, 新增药品地址
//2106, 清空药品地址
//2201, 设置设备运行时间
//2202, 设置热敏打印机次数
//2203, 设置热转打印机次数
//2204, 设置包装机次数
//2401, 药品导入
//2402, 处方导入

//////////////////////////////////////////////////////////////////////////
/*用户管理模块返回错误类型*/
#define ER_LIST_USERS					100001	//列出用户信息失败
#define ER_ADD_USER						100002	//新增用户失败
#define ER_OLD_PASSWORD					100003	//密码核对错误 
#define ER_LIST_GROUP					100004	//列出用户失败
#define ER_USER_EXISTED					100005	//用户已经存在 
#define ER_IKEY_BOUND					100006	//此iKey已经绑定到其他用户

/*加密狗模块返回错误类型*/
#define ER_OPEN_DEVICE				110001 	//打开ikey失败
#define ER_CLOSE_DEVICE				110002	//关闭ikey失败
#define ER_VERIFY_USER_PIN			110003	//用户PIN码校验失败
#define ER_HANDLE_IS_NULL			110004	//设备句柄为空
#define ER_CREATE_APP_DIR			110005	//创建应用程序目录失败
#define ER_CREATE_DATA_FILE			110006	//创建数据文件失败
#define ER_CREATE_MD5_HMAC_KEY		110007	//创建MD5_HMAC_KEY文件失败
#define ER_MD5_HMAC_AUTHENTICATE	110008	//签名MD5_HMAC失败
#define ER_OPEN_DIR					110009	//打开目录失败
#define ER_GET_USER					110010	//取得用户名失败

#define ER_GET_LICENSE				111001	//获取加密狗许可失败
#define ER_QUERY_FEATURE			111002	//查询特征码失败
#define ER_QUERY_RESPONSE			111003	//查询响应错误

/*系统配置模块返回错误类型*/
#define ER_NO_CONFIG_FILE	120001	//没有配置文件
#define ER_CONFIG_FILE		120002	//配置文件错误
#define ER_LOAD_CONFIG_FILE	120003	//加载配置文件失败
#define ER_UPDATE_CONFIG	120004	//更新配置文件失败

/*处方管理模块返回错误类型*/
#define ER_NO_PATIENT_INFO				130001	//没有患者信息
#define ER_NO_MEDICINE_INFO				130002	//没有药品信息
#define ER_SAVE_PRESCRIPTION			130003	//保存处方失败
#define ER_SAVE_PATIENT					130004	//保存病人失败
#define ER_SAVE_PRESCRIPTION_MEDICINE	130005	//保存药品失败
#define ER_INPUT_PRESCRIPTION			130006	//录入处方失败
#define ER_QUERY_PRESCRIPTION			130007	//处方查询失败
#define ER_GET_PATIENT					130008	//获取病人信息失败
#define ER_GET_PRESCRIPTION_MEDICINE	130009	//获取处方药品信息失败
#define ER_MODIFY_PRESCRIPTION			130010	//修改处方失败
#define ER_MODIFY_PRESCRIPTION_MEDICINE	130011	//修改处方药品信息失败
#define ER_MODIFY_PRESCRIPTION_PATIENT	130012	//修改处方病人信息失败
#define ER_DEL_PRESCRIPTION				130013	//删除处方失败
#define ER_DEL_PRESCRIPTION_MEDICINE	130014	//删除处方药品信息失败
#define ER_DEL_PATIENT					130015	//删除病人信息失败

/*数据库管理模块返回错误类型*/
#define ER_DB_INIT						140001	//数据库初始化失败
#define ER_DB_CONNECT					140002	//数据库连接失败
#define WM_NO_SQLTEXT					140003	//创建设备状态监听线程失败
#define ER_EXECUTE_SQL					140004	//执行SQL语句错误
#define WM_DATA_END						140005  //结果集中最后一行的下一行
#define ER_NO_DATASET					140006	//数据集为空
#define ER_DB_NOT_OPEN					140007  //数据库没有打开
#define ER_SERVER_STATE					140008	//数据库服务器连接状态错误
#define ER_DB_PARAM						140009	//备份数据库名为空
#define ER_DB_BACKUP					140010	//备份数据库出错
#define ER_DB_RECOVER					140011	//恢复数据库出错
#define ER_UPDATE_TABLE					140012	//更新表失败

/*热敏打印机模块返回错误类型*/
#define ER_PRINTER_PORT_NUM				150001	//打印机端口号错误
#define ER_OPEN_PRINTER_PORT			150002	//打印机端口打开失败
#define ER_NO_BASKET_ID					150003	//没有篮子卡号
#define ER_PRINT_PRESCRIPTION			150004	//打印处方失败
#define ER_PRESCRIPTION_ID				150005	//通过处方号打印处方失败
#define ER_PRINT_HETEROMORPHISM			150006	//打印异形药品信息失败

#define ER_NO_PAPER						150007	//纸尽
#define ER_LESS_PAPER					150008	//纸将尽
#define ER_CUT					        150009	//切刀错误
#define ER_HOT					        150010	//热敏头温度过高
#define ER_NO_BASKET_NUMBER             150011  //没有输入篮子序号

//CThermalPrinterStar
//#define ER_PRINTER_PORT_NUM				151001	//打印机端口号错误
//#define ER_OPEN_PRINTER_PORT			151002	//打印机端口打开失败
#define WN_NOT_OPEN_PORT				151003	//打印机端口未打开
#define ER_PRINT						151004	//打印失败
#define ER_GET_PRINTER_STATE			151005	//获取打印机状态失败
#define ER_SEND_COMMAND					151006	//发送打印机指令失败

#define ER_PORT_NUM				159001	//读卡器端口号错误
#define ER_OPEN_PORT			159002	//读卡器端口打开失败
#define ER_WRITE_DATA			159003	//写通讯数据失败
#define ER_READ_DATA			159004	//读端口数据失败
#define ER_NOT_OPEN_PORT		159005	//读卡器端口没有打开
#define ER_CREATE_LISTEN_THREAD	159006	//创建读卡器监听线程失败
#define ER_WRITE_CARDATA        159007  //写IC卡数据失败

/*PLC模块返回错误类型*/
#define ER_MEDICINE_POSITION			160001	//药品位置信息错误
#define ER_MEDICINE_COUNT				160002	//药品数目错误
#define ER_SUSPEND_LISTEN_THREAD		160003	//终止设备状态监听线程失败
#define ER_CLOSE_LISTEN_THREAD			160004	//关闭设备状态监听线程句柄失败
#define ER_NO_PLC						160005  //没有PLC实例
#define ER_NOT_OPEN_PLC					160006  //PLC连接没有打开
#define ER_PLC_SENDBASKETFINISH         160007  //等待PLC送走篮子完成信号超时
//////////////////////////////////////////////////////////////////////////

#define FILLLIST_EMPTY                   170001		//加药队列为空
#define FILLMED_ERROR                    170002     //加药失败
#define FINISHFILLMED_ERROR              170003     //完成加药失败

#define FAILED_TO_CREATE_THREAD			 180001     //创建线程失败
#define FAILED_TO_RESUME_THREAD			 180002		//恢复线程失败
#define FAILED_TO_SUSPEND_THREAD		 180003		//中断线程失败
#define FAILED_TO_CLOSE_HANDLE			 180004		//关闭线程失败
#define NOT_ENOUGH_MEDICINE				 180005		//药品存量不够
#define CAN_NOT_FIND_NORMAL_ID           180006		//查询不到正常队列处方ID
#define CAN_NOT_FIND_ABNORMAL_ID         180007		//查询不到异常队列处方ID
#define READ_BASKET_OVER_TIME			 180008     //读篮子超时
#define READ_BASKET_ERROR				 180009     //读取不到篮子编号
#define DIS_ERROR						 1801       //发药失败操作码
#define MACHINE_DISABLE					 180010     //设备未启用
#define MACHINE_FETAL_ERROR				 180011     //设备严重故障
#define DISTRIBUTE_NUM_ERROR			 180012     //发药数量出错
#define NO_FIT_PRESCRIPTION				 180013		//没有符合条件的处方记录


/*药品管理类返回错误类型*/
#define ERROR_MedicineManage_Initialize              210001  //初始化各类药品位置状态失败
#define ERROR_MedicineManage_PopMedicinLocation      210002  //发药时修改药品位置状态失败
#define ERROR_MedicineManage_PushMedicinLocation     210003  //加药时修改药品位置状态失败
#define ERROR_MedicineManage_AddressSearch           210004  //查找药品的地址列表失败
#define ERROR_MedicineManage_GetMedicineCount        210005  //获取药品当前所剩余的数量失败
#define ERROR_MedicineManage_FastSearch              210006  //编码快速查找失败
#define ERROR_MedicineManage_ListSearch              210007  //查找药品列表失败
#define ERROR_MedicineManage_LookMedicine            210008  //查找药品详细信息失败
#define ERROR_MedicineManage_ModifyMedicine          210009  //修改药品详细信息失败
#define ERROR_MedicineManage_AddMedicine             210010  //新增药品详细信息失败
#define ERROR_MedicineManage_DeleteMedicine          210011  //删除药品详细信息失败
#define ERROR_MedicineManage_ClearLocation           210012  //清空药品单个地址信息失败
#define ERROR_MedicineManage_ClearAllLocation        210013  //清空药品所有地址信息失败
#define ERROR_MedicineManage_getMaxmedID             210014  //获取药品最大ID号失败
#define ERROR_MedicineManage_getmedID                210015  //获取药品ID号失败
#define ERROR_MedicineManage_light_position          210016  //获取指示灯号失败
#define ERROR_MedicineManage_MedicineOverdue         210017  //当前药品已经过期
#define ERROR_MedicineManage_modifyMedicinLocation   210018  //修改药品位置状态出错
#define ERROR_MedicineManage_QueryMedInfo_Client     210019  //查询客户端需求药品信息失败
#define ERROR_MedicineManage_QueryMedleavingPercent  210020  //查询药品剩余百分比失败
#define ERROR_MedicineManage_findDeviceinfo          210021  //查询设备信息失败
#define ERROR_MedicineManage_saveDeviceinfo          210022  //保存设备信息失败
#define ERROR_MedicineManage_setMachineState         210023  //设置设备状态失败
#define ERROR_MedicineManage_getAllMachineState      210024  //获取所有设备状态失败
#define ERROR_MedicineManage_getMachineState         210025  //获取设备状态失败
#define ERROR_MedicineManage_getWindowfalg           210026  //获取窗口报警状态失败
#define ERROR_MedicineManage_setWindowfalg           210027  //设置窗口状态失败
#define ERROR_MedicineManage_getmachinefalg          210028  //获取设备状态失败
#define ERROR_MedicineManage_getMedWeight            210029  //获取药品权重失败
#define ERROR_MedicineManage_getWindowCount          210030  //获取窗口数失败
   

/*全局统计类返回错误类型*/
#define ERROR_Statistic_SumLayMedicinCount     220001  //保存总摆药次数失败
#define ERROR_Statistic_SumPrescriptionCount   220002  //保存总处方数量失败
#define ERROR_Statistic_SumDeviceRunTime       220003  //保存总设备运行时间数据失败
#define ERROR_Statistic_SumLogCount            220004  //保存总登录次数失败
#define ERROR_Statistic_SumPrintCount          220005  //保存总热敏打印次数失败
#define ERROR_Statistic_SumSatoPrintCount      220006  //保存总热转打印次数失败
#define ERROR_Statistic_SumPackCount           220007  //保存总包装次数失败
#define ERROR_Statistic_AddMedicinSortd        220008  //保存药品分类统计失败
#define ERROR_Statistic_AddLocation            220009  //保存位置统计失败
#define ERROR_Statistic_GetMedicinSort         220010  //输出药品分类统计失败
#define ERROR_Statistic_GetLocation            220011  //输出地址分类统计失败
#define ERROR_Statistic_Initialize             220012  //初始化全局统计数据失败
#define ERROR_Statistic_SetBooking             220013  //设置预警量失败
#define ERROR_Statistic_GetBooking             220014  //输出预警量失败

/*监控系统类返回错误类型*/
#define ERROR_Monitor_CreatThead        230001  //创建设备监控线程失败
#define ERROE_Monitor_GetCameraState    230002  //连接摄像机失败
#define ERROE_Monitor_GetScannerState   230003  //连接扫描仪失败
#define ERROE_Monitor_GetPLCState       230004  //连接PLC失败
#define ERROE_Monitor_GetPrintState     230005  //连接打印机失败

/*批量导入返回错误类型*/
#define ERROR_Import_Medicine          240001  //批量导入药品失败
#define ERROR_Import_Prescription      240002  //批量导入处方失败

//////////////////////////////////////////////////////////////////////////

#endif
