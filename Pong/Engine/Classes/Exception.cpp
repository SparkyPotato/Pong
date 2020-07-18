#include "Exception.h"

Exception::Exception(int lineNumber, const char* fileName) noexcept
	:
	lineNumber(lineNumber),
	fileName(fileName)
{

}

const char* Exception::what() const noexcept
{
	std::ostringstream outputStream;
	outputStream << GetExceptionType() << std::endl
				 << GetFormattedString();

	buffer = outputStream.str();
	return buffer.c_str();
}

const char* Exception::GetExceptionType() const noexcept
{
	return "Global Exception";
}

std::string Exception::GetFormattedString() const noexcept
{
	std::ostringstream outputStream;
	outputStream << "File: " << GetFileName() << std::endl
				 << "Line: " << GetLineNumber();

	return outputStream.str();
}