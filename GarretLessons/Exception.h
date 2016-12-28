#pragma once

#include <string>

class Exception
{
public:
	Exception( std::wstring&&file, unsigned int line, std::wstring&& note )
		:
		note( std::move( note ) ),
		file( std::move( file ) ),
		line( line )
	{}
	const std::wstring& GetNote() const
	{
		return note;
	}
	const std::wstring& GetFile() const
	{
		return file;
	}
	unsigned int GetLine() const
	{
		return line;
	}
	std::wstring GetLocation() const
	{
		return std::wstring( L"Line [" ) + std::to_wstring( line ) + L"] in " + file;
	}
	virtual std::wstring GetFullMessage() const = 0;
	virtual std::wstring GetExceptionType() const = 0;
private:
	std::wstring note;
	std::wstring file;
	unsigned int line;
};