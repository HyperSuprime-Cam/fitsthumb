#ifndef CommandLine_hpp__
#define CommandLine_hpp__


namespace CmdLn
{


// Used as a value of SOption::eArg
enum { NO_ARG, HAS_ARG, OPT_ARG };

struct SOption
{
	char        cShort;
	const char* pszLong;
	int         eArg;
	const char* pszArgName;
	const char* pszDescription;
};

bool
EnumOptions(
	int     argc,
	char**  argv,
	const SOption* options,
	bool (* pfnEnumProc)(const SOption& option, const char* pszArg, void* pUser),
	void*   pUser,
	int*    pNewArgc
);

void
PrintOptions(
	const SOption* options
);


} // namespace CmdLn
#endif //CommandLine_hpp__
