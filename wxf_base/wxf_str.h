/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_str.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/15
**
** Description:
**************************************************************************************/ 
#ifndef _wxf_str_h_
#define _wxf_str_h_

#include <assert.h>
#include <vector>

/*
   wxf_str is an emulation of a subset of the std::string template.
   Its purpose is to allow compiling TinyXML on compilers with no or poor STL support.
   Only the member functions relevant to the TinyXML project have been implemented.
   The buffer allocation is made by a simplistic power of 2 like mechanism : if we increase
   a string and there's no more room, we allocate a buffer twice as big as we need.
*/
class wxf_str
{
  public :
	// The size type used
  	typedef size_t size_type;
	
	// Error value for find primitive
	static const size_type npos; // = -1;

	// wxf_str empty constructor
	wxf_str () : rep_(&nullrep_)
	{
	}

	// wxf_str copy constructor
	wxf_str ( const wxf_str & copy) : rep_(0)
	{
		init(copy.length());
		memcpy(start(), copy.data(), length());
	}

	// wxf_str constructor, based on a string
	wxf_str ( const char * copy) : rep_(0)
	//wxf_str ( const char * copy) : rep_(0)
	{
		if (copy == NULL)
		{
			quit();
		}
		else
		{
			init( static_cast<size_type>( strlen(copy) ));
			memcpy(start(), copy, length());
		}		
	}

	// wxf_str constructor, based on a string
	explicit wxf_str ( const char * str, size_type len) : rep_(0)
	{
		init(len);
		memcpy(start(), str, len);
	}

	// wxf_str destructor
	~wxf_str ()
	{
		quit();
	}

	wxf_str& operator = (const char * copy)
	{
		return assign( copy, (size_type)strlen(copy));
	}

	wxf_str& operator = (const wxf_str & copy)
	{
		return assign(copy.start(), copy.length());
	}

	// += operator. Maps to append
	wxf_str& operator += (const char * suffix)
	{
		return append(suffix, static_cast<size_type>( strlen(suffix) ));
	}

	// += operator. Maps to append
	wxf_str& operator += (char single)
	{
		return append(&single, 1);
	}

	// += operator. Maps to append
	wxf_str& operator += (const wxf_str & suffix)
	{
		return append(suffix.data(), suffix.length());
	}

	// [] operator
	char& operator [] (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}

	// Convert a wxf_str into a null-terminated char *
	const char * c_str () const { return rep_->str; }

	// Convert a wxf_str into a char * (need not be null terminated).
	char * data () const { return rep_->str; }

	// Return the length of a wxf_str
	size_type length () const { return rep_->size; }

	// Alias for length()
	size_type size () const { return rep_->size; }

	// Checks if a wxf_str is empty
	bool empty () const { return rep_->size == 0; }

	// Return capacity of string
	size_type capacity () const { return rep_->capacity; }

	// single char extraction
	const char& at (size_type index) const { assert( index < length() );return rep_->str[ index ];}

	//clear str,cap set to 0
	void clear () { quit(); init(0,0);}

	//swap data with this
	void swap (wxf_str& other) { Rep* r = rep_;rep_ = other.rep_;other.rep_ = r;	}

	// find a char in a string. Return wxf_str::npos if not found
	size_type find (char lookup) const
	{
		return find(lookup, 0);
	}

	// find a char in a string of last
	size_type find_last (char lookup) const
	{
		return find_last(lookup, size()-1);
	}

	// find a char in a string from an offset. Return wxf_str::npos if not found
	size_type find (char tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		for (const char* p = c_str() + offset; *p != '\0'; ++p)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;	
	}
	
	//find a char in a string from an offset of last.
	size_type find_last (char tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		const char* p = c_str() + offset;
		do 
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		} while (p-- != c_str());
		return npos;	
	}

	/*	Function to reserve a big amount of data when we know we'll need it. Be aware that this
		function DOES NOT clear the content of the wxf_str if any exists.
	*/

	//clear str,cap set to cap
	void resize (size_type cap);

	//enlarge cap
	void reserve (size_type cap);
	
	//reinit
	wxf_str& assign (const char* str, size_type len);

	//add data to string
	wxf_str& append (const char* str, size_type len);

protected:

	void init(size_type sz) { init(sz, sz); }
	void set_size(size_type sz) { rep_->str[ rep_->size = sz ] = '\0'; }
	char* start() const { return rep_->str; }
	char* finish() const { return rep_->str + rep_->size; }

private:
	void init(size_type sz, size_type cap)
	{
		if (cap)
		{
			// Lee: the original form:
			//	rep_ = static_cast<Rep*>(operator new(sizeof(Rep) + cap));
			// doesn't work in some cases of new being overloaded. Switching
			// to the normal allocation, although use an 'int' for systems
			// that are overly picky about structure alignment.
			const size_type bytesNeeded = sizeof(Rep) + cap;
			const size_type intsNeeded = ( bytesNeeded + sizeof(int) - 1 ) / sizeof( int ); 
			rep_ = reinterpret_cast<Rep*>( new int[ intsNeeded ] );

			rep_->str[ rep_->size = sz ] = '\0';
			rep_->capacity = cap;
		}
		else
		{
			rep_ = &nullrep_;
		}
	}

	void quit()
	{
		if (rep_ != &nullrep_)
		{
			// The rep_ is really an array of ints. (see the allocator, above).
			// Cast it back before delete, so the compiler won't incorrectly call destructors.
			delete [] ( reinterpret_cast<int*>( rep_ ) );
		}
	}

	struct Rep
	{
		size_type size, capacity;
		char str[1];
	};

	Rep * rep_;
	static Rep nullrep_;


public:
	operator const char *()	{ return c_str(); }

	//¸ñÊ½»¯×Ö·û´® by yexf
	int format(const char* fmt, ...);

	//È¡×Ó×Ö·û´® by yexf
	wxf_str substr (size_type start, size_type end = npos);

	//·Ö¸î×Ö·û´® by yexf
	wxf_str split(char sp, bool IsLast = false, bool DelSplit = true)
	{
		return split(find(sp),IsLast,DelSplit);
	}
	wxf_str split_last(char sp, bool IsLast = true, bool DelSplit = true)
	{
		return split(find_last(sp),IsLast,DelSplit);
	}

	wxf_str split(size_t Pos, bool IsLast, bool DelSplit);

};

inline bool operator == (const wxf_str & a, const wxf_str & b)
{
	return    ( a.length() == b.length() )				// optimization on some platforms
	       && ( strcmp(a.c_str(), b.c_str()) == 0 );	// actual compare
}
inline bool operator < (const wxf_str & a, const wxf_str & b)
{
	return strcmp(a.c_str(), b.c_str()) < 0;
}

inline bool operator != (const wxf_str & a, const wxf_str & b) { return !(a == b); }
inline bool operator >  (const wxf_str & a, const wxf_str & b) { return b < a; }
inline bool operator <= (const wxf_str & a, const wxf_str & b) { return !(b < a); }
inline bool operator >= (const wxf_str & a, const wxf_str & b) { return !(a < b); }

inline bool operator == (const wxf_str & a, const char* b) { return strcmp(a.c_str(), b) == 0; }
inline bool operator == (const char* a, const wxf_str & b) { return b == a; }
inline bool operator != (const wxf_str & a, const char* b) { return !(a == b); }
inline bool operator != (const char* a, const wxf_str & b) { return !(b == a); }

wxf_str operator + (const wxf_str & a, const wxf_str & b);
wxf_str operator + (const wxf_str & a, const char* b);
wxf_str operator + (const char* a, const wxf_str & b);


#endif /*_wxf_str_h_*/
