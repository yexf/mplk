#include "wxf_str.h"
#include <stdarg.h>
#include <stdio.h>

#pragma warning(disable:4996)

wxf_str operator + (const wxf_str & a, const wxf_str & b)
{
	wxf_str tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

wxf_str operator + (const wxf_str & a, const char* b)
{
	wxf_str tmp;
	wxf_str::size_type b_len = static_cast<wxf_str::size_type>( strlen(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

wxf_str operator + (const char* a, const wxf_str & b)
{
	wxf_str tmp;
	wxf_str::size_type a_len = static_cast<wxf_str::size_type>( strlen(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}

// Error value for find primitive
const wxf_str::size_type wxf_str::npos = static_cast< wxf_str::size_type >(-1);
wxf_str::Rep wxf_str::nullrep_ = { 0, 0, { '\0' } };


void wxf_str::resize( size_type cap )
{
	if (cap > capacity())
	{
		wxf_str tmp;
		tmp.init(0, cap);
		swap(tmp);
	}
	else
	{
		set_size(0);
	}
}

//扩容,保持数据不变
void wxf_str::reserve (size_type cap)
{
	if (cap > capacity())
	{
		wxf_str tmp;
		tmp.init(length(), cap);
		memcpy(tmp.start(), data(), length());
		swap(tmp);
	}
}

//重新初始化
wxf_str& wxf_str::assign(const char* str, size_type len)
{
	size_type cap = capacity();
	if (len > cap || cap > 3*(len + 8))
	{
		wxf_str tmp;
		tmp.init(len);
		memcpy(tmp.start(), str, len);
		swap(tmp);
	}
	else
	{
		memmove(start(), str, len);	
		set_size(len);
	}
	return *this;
}

//在后面添加
wxf_str& wxf_str::append(const char* str, size_type len)
{
	size_type newsize = length() + len;
	if (newsize > capacity())
	{
		reserve (newsize + capacity());
	}
	memmove(finish(), str, len);
	set_size(newsize);
	return *this;
}
//格式化字符串
int wxf_str::format(const char* fmt, ...)
{
	// if the format string is NULL ,return 
	if (fmt == NULL)
	{
		return -1;
	}
		
	va_list argList;

	// Set va_list to the beginning of optional arguments
	va_start(argList, fmt);

	const char * ptr = fmt;
	char * str = NULL;

	//save the max len of the formatstring
	int    nMaxLen = 0;

	while(*ptr != '\0')
	{
		str = NULL;

		if(*ptr == '%')
		{
			switch(*(ptr+1))
			{
			case 's':
			case 'S':
				str = va_arg(argList,char*);

				if( NULL == str)
					nMaxLen ++;
				else
					nMaxLen += strlen(str);
				ptr++;
				break;

			case 'c':
			case 'C':
				va_arg(argList,char);
				nMaxLen +=2;
				ptr++;
				break;

			case 'd':
			case 'D':
				va_arg(argList, int);
				nMaxLen +=11;
				ptr++;
				break;

			case 'u':
			case 'U':
				va_arg(argList, unsigned int);
				nMaxLen +=10;
				ptr++;
				break;

			case 'l':
			case 'L':
				ptr++;
				if(*(ptr+1) == 'd')
				{
					va_arg(argList, long);
					nMaxLen +=11;
				}
				else if(*(ptr+1) == 'u')
				{
					va_arg(argList, unsigned long);
					nMaxLen +=10;
				}
				ptr++;
				break;
			case 'f':
			case 'F':
				va_arg(argList, double);
				nMaxLen += 31;
				ptr++;
				break;
			case 'x':
			case 'X':
				va_arg(argList, void*);
				nMaxLen += 2*sizeof(void*);
				ptr++;
				break;
			default:
				nMaxLen+=1;
			}
		} //  if(*ptr == '%')
		else
		{
			nMaxLen +=1;
		}
		// Increment pointer..
		ptr++;
	}
	va_end(argList);

	nMaxLen += 255;       // 防止特殊情况长度计算错误；
    wxf_str strDes(""); 
	strDes.reserve(nMaxLen); 

	try{
		va_start(argList, fmt);  
		vsprintf(strDes.data(), fmt, argList);
		va_end(argList);
		swap(strDes);
	}
	catch(...)
	{
		return 0;
	}

	return nMaxLen;
}

wxf_str wxf_str::substr( size_type start, size_type end /*= size()*/ )
{
	assert(start <= end);
	if (end > size())
	{
		end = size();
	}
	size_type len = end - start;
	return wxf_str(data()+start,len);
}

wxf_str wxf_str::split( size_t Pos, bool IsLast, bool DelSplit)
{
	if (Pos >= size())
	{
		return *this;
	}
	if(DelSplit)
	{
		if (IsLast)
		{
			return substr(Pos+1);
		}
		else
		{
			return substr(0, Pos);
		}
	}
	else
	{
		if (IsLast)
		{
			return substr(Pos);
		}
		else
		{
			return substr(0, Pos+1);
		}
	}
}

wxf_str &wxf_str::assign_addend( const char *str,const char ch )
{
	size_t iLen;
	iLen = strlen(str);
	assign(str,iLen);
	if (str[iLen-1] != ch)
	{
		append(&ch,1);
	}
	return *this;
}

wxf_str &wxf_str::assign_delend( const char *str,const char ch )
{
	size_t iLen;
	iLen = strlen(str);
	assign(str,iLen-1);
	if (str[iLen-1] != ch)
	{
		append(&ch,1);
	}
	return *this;
}

wxf_str & wxf_str::term_end()
{
	while (data()[size()-1] == ' ' ||
		data()[size()-1] == '\t' )
	{
		set_size(size()-1);
	}
	return *this;
}

wxf_str & wxf_str::term_begin()
{
	int pos = 0;
	while (data()[pos] == ' ' ||
		data()[pos] == '\t' )
	{
		pos++;
	}
	assign(data()+pos,size()-pos);
	return *this;
}
