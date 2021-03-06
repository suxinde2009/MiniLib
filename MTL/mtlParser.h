#ifndef MTL_PARSER_H_INCLUDED__
#define MTL_PARSER_H_INCLUDED__

#include "mtlString.h"
#include "mtlList.h"
#include "mtlDirectory.h"

#define mtlOpenBracesStr	"([{<\'\""
#define mtlClosedBracesStr	")]}>\'\""
#define mtlWhitespacesStr	" \t\n\r"
#define mtlNewlinesStr		"\n\r"
#define mtlIntStr			"0123456789"
#define mtlFloatStr			"0123456789."
#define mtlHexStr			"0123456789abcdef"
#define mtlBinStr			"01"
#define mtlAlphaStr			"abcdefghijklmnopqrstuvwxyz_"
#define mtlAlphanumStr		"abcdefghijklmnopqrstuvwxyz0123456789_"

class mtlParser
{
private:
	static const char var = '%';

public:
	enum ExpressionResult
	{
		ExpressionNotFound,
		ExpressionInputError,
		ExpressionFound
	};

private:
	mtlChars	m_buffer;
	int			m_reader;

	struct Selection { int start, end; };

private:
	bool				IsEnd(int i) const { return i >= m_buffer.GetSize() || i < 0; }
	int					SkipWhitespaces(int i) const;
	int					BackWhitespaces(int i) const;
	int					SkipNonWhitespaces(int i) const;
	int					BackNonWhitespaces(int i) const;

	int					IndexOfAny(int i, const mtlChars &p_chars, bool caseSensitive = false) const;
	int					IndexOfAnyBack(int i, const mtlChars &p_chars, bool caseSensitive = false) const;

	Selection			WordIndex(int i) const;
	Selection			WordIndexBack(int i) const;
	Selection			LineIndex(int i) const;
	Selection			LineIndexBack(int i) const;

	mtlChars			GetSubstring(Selection s) const;

	//void				RemoveDelimiters(const mtlChars &chars, const mtlChars &delimiters, mtlString &out, bool caseSensitive) const;

	bool				IsFormat(char ch, const mtlChars &format, bool caseSensitive, const bool notState) const;

public:
						mtlParser( void );
	explicit			mtlParser(const mtlChars &p_buffer);

	const mtlChars		&GetBuffer( void ) const;
	void				SetBuffer(const mtlChars &p_buffer);
	static bool			BufferFile(const mtlDirectory &p_file, mtlString &p_buffer);

	char				ReadChar( void );
	char				PeekChar( void ) const;
	void				BackChar( void );

	mtlChars			ReadCharStr( void );
	mtlChars			PeekCharStr( void ) const;

	mtlChars			ReadWord( void );
	mtlChars			PeekWord( void ) const;
	void				BackWord( void );

	mtlChars			ReadFormat(const mtlChars &format, bool caseSensitive = false, bool notState = false);
	mtlChars			PeekFormat(const mtlChars &format, bool caseSensitive = false, bool notState = false) const;

	mtlChars			ReadFormatOne(const mtlChars &format, bool caseSensitive = false, bool notState = false);
	mtlChars			PeekFormatOne(const mtlChars &format, bool caseSensitive = false, bool notState = false) const;

	mtlChars			ReadLine( void );
	mtlChars			PeekLine( void ) const;
	void				BackLine( void );

	mtlChars			ReadTo(const mtlChars &p_str, bool caseSensitive=false);
	mtlChars			ReadTo(char ch, bool caseSensitive=false);
	mtlChars			PeekTo(const mtlChars &p_str, bool caseSensitive=false) const;
	mtlChars			PeekTo(char ch, bool caseSensitive=false) const;
	//void				BackTo(const mtlChars &p_str, bool caseSensitive=false);
	//void				BackTo(char ch, bool caseSensitive=false);
	int					IndexOf(const mtlChars &p_chars, bool caseSensitive=false) const;
	int					IndexOf(char ch, bool caseSensitive=false) const;
	int					IndexOfBack(const mtlChars &p_chars, bool caseSensitive=false) const;
	int					IndexOfBack(char ch, bool caseSensitive=false);

	mtlChars			ReadToAny(const mtlChars &p_chars, bool caseSensitive=false);
	mtlChars			PeekToAny(const mtlChars &p_chars, bool caseSensitive=false) const;
	void				BackToAny(const mtlChars &p_chars, bool caseSensitive=false);
	int					IndexOfAny(const mtlChars &p_chars, bool caseSensitive=false) const;
	int					IndexOfAnyBack(const mtlChars &p_chars, bool caseSensitive=false) const;

	mtlChars			ReadRest( void );
	mtlChars			PeekRest( void ) const;

	bool				IsEnd( void );
	int					GetCharCount( void ) const		{ return m_buffer.GetSize(); }
	int					GetCurrentIndex( void ) const	{ return m_reader; }
	int					GetCharsLeft( void ) const		{ return m_buffer.GetSize() - m_reader; }
	int					GetLineNumber( void ) const;

	void				SkipToStart( void )		{ m_reader = 0; }
	void				SkipToEndChar( void )	{ m_reader = m_buffer.GetSize() - 1; }
	void				SkipToEndWord( void );
	void				SkipToEndLine( void );
	void				SkipToIndex(int index)	{ m_reader = (index < 0) ? m_buffer.GetSize() : index; }

	void				SkipToAny(const mtlChars &p_chars, bool caseSensitive=false);
	void				SkipToAnyBack(const mtlChars &p_chars, bool caseSensitive=false);

	void				SkipWhitespaces( void ) { m_reader = SkipWhitespaces(m_reader); }
	void				BackWhitespaces( void ) { m_reader = BackWhitespaces(m_reader); }

	mtlChars			ReadRaw(int count);
	mtlChars			PeekRaw(int count) const;
	void				BackRaw(int count);

	ExpressionResult	Match(const mtlChars &expr, mtlList<mtlChars> &out, bool revert_on_fail = true);
	ExpressionResult	MatchAll(const mtlChars &expr, mtlList<mtlChars> &out, bool revert_on_fail = true);
};

#endif
