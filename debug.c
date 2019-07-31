#include "debug.h"

extern UART_HandleTypeDef huart2;

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

/*
在调试有时需要通过uart传入参数
此函数是从一个字符串里寻找字符串后面的一个整数并返回
例如buf[] = "size 10" buf_dir[] = "size" 返回整数10
找不到时返回def
*/
int32_t dbg_seek_arg(char *buf, char *buf_dir, int32_t def)
{
	uint32_t i;
	char *p_dir;
	uint32_t loc_dir_head;
	uint32_t loc_dir_tail;
	uint32_t i_dir_len;
	char *p_dir_space;
	uint32_t i_dir_space;
	uint32_t loc_dir_space;
	char c_num[6] = {0};
	uint32_t i_num;

	i_dir_len = eLIBs_strlen(buf_dir);

	/*  seek tail  */
	p_dir = eLIBs_strstr(buf, buf_dir);
	if(p_dir)
	{
		__wrn("seek %s succ\r\n",buf_dir);
		loc_dir_head = p_dir - buf;
		//"up " loc_dir_tail is " " space location
		loc_dir_tail = loc_dir_head + i_dir_len;
		__wrn("seek buf loc_dir_head = %d\r\n", loc_dir_head);
		__wrn("seek buf loc_dir_tail = %d\r\n", loc_dir_tail);
		//i_dir_space = eLIBs_strstr(buf + loc_dir_tail + 1," ") - buf;
		if(isLetter(buf[loc_dir_tail + 1]))
		{
			i_num = 1;
			__wrn("i_num = %d\r\n", i_num);
			return i_num;
		}
		p_dir_space = eLIBs_strstr(buf + loc_dir_tail + 1, " ");
		if(p_dir_space)
		{
			loc_dir_space = p_dir_space - buf;
			__wrn("seek buf i_space = %d\r\n", loc_dir_space);
		}
		
		for(i=0;i<loc_dir_space-loc_dir_tail-1;i++)
		{
			c_num[i] = buf[i + loc_dir_head + i_dir_len + 1]; 
		}
		__wrn("c_num = %s\r\n", c_num);
		i_num = eLIBs_atoi((const char *)c_num);
		__wrn("i_num = %d\r\n", i_num);
		return i_num;
	}	
	return def;
}
