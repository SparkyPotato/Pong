#pragma once
#include <Minimal.h>

class Exception : public std::exception
{
public:
	Exception(int lineNumber, const char* fileName) noexcept;
	const char* what() const noexcept override;
	int GetLineNumber() const noexcept { return lineNumber; }
	const std::string GetFileName() const noexcept { return fileName; }
	virtual const char* GetExceptionType() const noexcept;
	std::string GetFormattedString() const noexcept;

private:
	int lineNumber;
	std::string fileName;

protected:
	mutable std::string buffer;
};

