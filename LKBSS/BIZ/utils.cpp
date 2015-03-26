
#include "stdafx.h"
#include "Utils.h"
UserInfo userinfo;
DatabaseParam DBparam;
DatabaseParam BoxDBparam;
ADODatabaseParam ADODBparam;
unsigned char desKey[8] = {'l','k','s','j','k','j','m','i'};    
string Utils::itos(int i){
		stringstream s;
        s << i;
        return s.str();
	}

string Utils::dtos(double d){
		stringstream s;
        s << d;
        return s.str();
	}
int Utils::insertSystemlog(string oper_id,string event_log,string result_log)
{
	CDBManage  *db=new CDBManage();
	string sql="insert into system_log("
		               "oper_id,"
					   "user_id,"
					   "user_name,"
					   "event,"
					   "result,"
					   "time"
					   ")values("
			           "'"+oper_id+"',"
			           +"'"+userinfo.user_id+"',"
		               +"'"+userinfo.user_name+"',"
			           +"'"+event_log+"',"
			           +"'"+result_log+"',"
					   + "now()"
					   +")";
	if(0==db->execSQL(sql))
	{
		delete db;
	    return 0;
	}
	else
	{
		delete db;
		return -1;
	}
}

int Utils::lookSystemlog(string time_log,list<SystemlogInfo>* pinputLog)
{
	CDBManage  *db=new CDBManage();
	string sql="select oper_id,user_id,user_name,event,result ,time from system_log where time like'"+time_log+"%' order by time desc";
	if(0==db->execSQL(sql))
	{
		while(!db->isEnd())
		{
			SystemlogInfo syslog;
			syslog.oper_id=db->getFieldValueByIndex(0);
			syslog.user_id=db->getFieldValueByIndex(1);
			syslog.user_name=db->getFieldValueByIndex(2);
			syslog.event_log=db->getFieldValueByIndex(3);
			syslog.result_log=db->getFieldValueByIndex(4);
			syslog.time_log=db->getFieldValueByIndex(5);
			pinputLog->push_back(syslog);
			db->nextRow();
		}
		db->freeRecord();
		delete db;
	    return 0;
	}
	else
	{
		delete db;
		return -1;
	}
}

string   Utils::ConvertHZToPY(char   *as_HzString) 
{ 
  static   int   li_SecPosValue[]={1601,1637,1833,2078,2274,2302,2433,2594,2787,3106,3212,3472,3635,3722,3730,3858,4027,4086,4390,4558,4684,4925,5249}; 
  static   char*   lc_FirstLetter[]     =   { "A",   "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "W", "X", "Y", "Z"}; 
  static   char*   ls_SecondSecTable   = 
"CJWGNSPGCGNE[Y[BTYYZDXYKYGT[JNNJQMBSGZSCYJSYY[PGKBZGY[YWJKGKLJYWKPJQHY[W[DZLSGMRYPYWWCCKZNKYYGTTNJJNYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJTXDTTSQZYCDXXHGCK[PHFFSS[YBGXLPPBYLL[HLXS[ZM[JHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZC[J[WQJBYZPXGZNZCPWHKXHQKMWFBPBYDTJZZKQHY " 
"LYGXFPTYJYYZPSZLFCHMQSHGMXXSXJ[[DCSBBQBEFSJYHXWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDKDYLHGYMYLCXPY[JNDQJWXQXFYYFJLEJPZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQYRBCJTHZTQFRXQHXMJJCJLXQGJMSHZKBSWYEMYLTXFSYDSWLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLCLQKXLHXJRZJMFQHXHWY " 
"WSBHTRXXGLHQHFNM[YKLDYXZPYLGG[MTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHSXLZCGHPXZHZNYTDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDNLKJYKJSYCJLKWHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPZJOQLCDHJJYSPRCHN[NNLHLYYQYHWZPTCZGWWMZFFJQQQQYXACLBHKDJXDGMMY " 
"DJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYD[FCXYHLXCHYZJQ[[QAGMNYXPFRKSSBJLYXYSYGLNSCMHZWWMNZJJLXXHCHSY[[TTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLY[DCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYKELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQH[PQ[QSCFYMNDMGBWHWLGSLLYSDLMLXPTHMJHWLJZYHZJXHTXJLHXRSWLWZJCBXMHZQXSDZP " 
"MGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHB[GDMYCNQQWLQHJJ[YWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSL[HTZKZJECXJCJNMFBY[SFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJCBCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJX[YZGWKYPSGXFZFCDKHJGXDLQFSGDSLQWZKXTMHSBGZMJZRGLYJB " 
"PMLMSXLZJQQHZYJCZYDJWBMYKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCXFPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHGJLJAXFGFJZSLCFDQZLCLGJDJCSNZLLJPJQDCCLCJXMYZFTSXGCGSBRZXJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZMYCNQSYCYHBHGXKAMTX " 
"YXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKY[CJDDYYGYWRHJRTGZNYQLDKLJSZZGZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJSBBSGGHFJLYPMZJNLYYWDQSHZXTYYWHMZYHYWDBXBTLMSYYYFSXJC[DXXLHJHF[SXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMG[YDXMJGDHCDYZBFFALLZTDLTFXMXQZDNGWQDBDCZJDXBZGS " 
"QQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSBRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJ[GYPZLTCSMCNBTJBQFKTHBYZGKPBBYMTDSSXTBNPDKLEYCJNYDDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJBBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJYHLG[DJLSLYGKDZPZXJ " 
"YYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZBYTDKYXJY[CNRJMPDJGKLCLJBCTBJDDBBLBLCZQRPPXJCJLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRP[QQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQXYFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYYDYJZZHQPDSZGLSTJBCKBXYQZJSGPSXQZQZRQTBDKYXZK " 
"HHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQYDFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJPFCZLCLZXZDMXMPHJSGZGSZZQLYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZMALBXYFBPNLSFHTGJWEJJXXGLLJSTGSHJQLZFKCGNNNSZFDEQ " 
"FHBSAQTGYLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCAFYWYXQHZ "; 
  string   result   =   ""; 
  int   H,L,W; 
  unsigned   stringlen   =   strlen(as_HzString); 
  for(int     i   =   0;   i   <   stringlen;   i++   )   
  { 
        H   =   (unsigned   char)(as_HzString[i]); 
        L   =   (unsigned   char)(as_HzString[i+1]); 
        if(H   <   0xA1   ||   L   <   0xA1){ 
			if (H < 0x80)
			{
				result += as_HzString[i];
				continue;
			}else 
			{
				i++;
				continue;
			}
        } 
        else   W   =   (H   -   160)   *   100   +   L   -   160; 
        if(W   >   1600   &&   W   <   5590)   
		{ 
              for(int j = 22; j >= 0;   j--) 
                      if(W   >=   li_SecPosValue[j])   { 
                          result   +=   lc_FirstLetter[j]; 
                          i++; 
                          break; 
                      } 
              continue; 
        } 
        else     
		{ 
          i++; 
          W   =   (   H   -   160   -   56   )*94   +   L   -   161; 
          if(W   >=   0   &&   W   <=   3007) 
                result   +=   ls_SecondSecTable[W]; 
          else   { 
              result   +=   (char)H; 
              result   +=   (char)L; 
          } 
		} 
  } 
  return   result; 
} 

string Utils::ConvertToStandString(string source)
{
	string rlt;
	CString tmp = source.c_str();
	tmp.Replace("'","");
	tmp.Replace("\"","");
	tmp.Replace("\\","");
	rlt = tmp;
	return rlt;
}


