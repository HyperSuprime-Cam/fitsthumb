#include "CommandLine.h"
#include <stdio.h>
#include <string.h>

namespace
{

const CmdLn::SOption* 
SearchOption(char cShortOption, const CmdLn::SOption* options);

const CmdLn::SOption* 
SearchOption(const char* pszLongOption, const CmdLn::SOption* options);

bool
IsOptionArg(const char* psz);


} // anonymous


//___________________________________________________________________________
bool
CmdLn::EnumOptions(
	int     argc,
	char**  argv,
	const SOption* options,
	bool (* pfnEnumProc)(const SOption& option, const char* pszArg, void* pUser),
	void*   pUser,
	int*    pNewArgc
){
	int newArgc = 1;

	bool bEndOptions = false;

	for(int i = 1; i < argc; ++i){
		char* theArg = argv[i];

		// '-' で始まらないか、もしくは '-' だけ
		if(theArg[0] != '-' || theArg[1] == '\0' || bEndOptions){
			// オプションでない引数として登録
			argv[newArgc++] = theArg;
		}
		// '-' で始まるが、'--' ではない
		else if(theArg[1] != '-'){
			// ショートオプション
			for(char* it = theArg + 1;
				*it != '\0'; ++it)
			{
				// オプションを検索
				const SOption* pOption = ::SearchOption(*it, options);
				if(pOption == NULL){
					printf("invalid option: -%c\n", *it);
					return false;
				}

				const char* pszOptArg = NULL;
				if(pOption->eArg != NO_ARG){
					// 引数をもつオプションなので、探す
					if(*(it+1) == '\0' && ::IsOptionArg(argv[i+1])){
						pszOptArg = argv[++i];
					}
					if(! pszOptArg && pOption->eArg == HAS_ARG){
						printf("lack of argument: -%c\n", *it);
						return false;
					}
				}

				// EnumProc 呼び出し
				if(! (*pfnEnumProc)(*pOption, pszOptArg, pUser)){
					return false;
				}
			}
		}
		// '--' だけ
		else if(theArg[2] == '\0'){
			// この後ろはすべて非オプションとして扱う
			bEndOptions = true;
		}
		// '--' で始まるオプション
		else{
			const char* pszOptArg = NULL;
			// '=' があれば、その後ろは引数
			for(char* it = theArg + 2;
				*it != '\0'; ++it)
			{
				if(*it == '='){
					pszOptArg = it+1;
					*it = '\0';
					break;
				}
			}

			// オプションを検索
			const SOption* pOption = ::SearchOption(theArg + 2, options);
			if(pOption == NULL){
				printf("invalid option: %s\n",
					theArg
				);
				return false;
			}

			// 引数のないオプションなのにあったら駄目
			if(pOption->eArg == NO_ARG && pszOptArg){
				printf(
					"option does not take argument: %s\n",
					theArg
				);
				return false;
			}
			// 引数をとるオプションだけどまだ引数はない
			else{
				// 引数を探す
				if(! pszOptArg && ::IsOptionArg(argv[i+1])){
					pszOptArg = argv[++i];
				}
				if(! pszOptArg && pOption->eArg == HAS_ARG){
					printf(
						"lack of argument: %s\n",
						theArg
					);
					return false;
				}
			}

			// EnumProc 呼び出し
			if(! (*pfnEnumProc)(*pOption, pszOptArg, pUser)){
				return false;
			}
		}

	// next i
	}

	argv[newArgc] = NULL;
	*pNewArgc = newArgc;
	return true;
}


//___________________________________________________________________________
void
CmdLn::PrintOptions(
	const CmdLn::SOption* options
){
	for(; options->pszDescription; ++options){
		if(options->cShort){
			printf("-%c", options->cShort);
			if(options->pszLong){
				printf(", ");
			}
		}
		if(options->pszLong){
			printf("--%s", options->pszLong);
			if(options->eArg == OPT_ARG){
				printf("(=%s)", options->pszArgName);
			}
			else if(options->eArg == HAS_ARG){
				printf("=%s", options->pszArgName);
			}
		}
		printf("\n  %s\n", options->pszDescription);
	}
}


namespace
{

const CmdLn::SOption* 
SearchOption(char cShortOption, const CmdLn::SOption* options)
{
	for(; options->pszDescription; ++options){
		if(options->cShort == cShortOption) return options;
	}

	return NULL;
}


const CmdLn::SOption* 
SearchOption(const char* pszLongOption, const CmdLn::SOption* options)
{
	int len = strlen(pszLongOption);
	const CmdLn::SOption* result = NULL;

	for(; options->pszDescription; ++options){
		if(0 == strncmp(options->pszLong, pszLongOption, len)){
			if(! result) result = options;
			else return NULL;
		}
	}

	return result;
}


bool
IsOptionArg(const char* psz)
{
	if(psz == NULL) return false;

	// '-' で始まらないか、もしくは '-' だけ
	return psz[0] != '-' || psz[1] == '\0';	
}


} // anonymous



// eof
