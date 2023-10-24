/* Amiga Port Kit (APK)
 *
 * (c) Robin Southern - github.com/betajaen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>

#include <dos/dos.h>
#include <workbench/workbench.h>
#include <inline/exec.h>
#include <inline/dos.h>

#define MIN_STACK_SIZE 65536U

int __nocommandline = 1;
int __initlibraries = 0;
extern char  *__commandline;
struct DosLibrary* DOSBase = NULL;
struct IntuitionBase* IntuitionBase = NULL;
struct GfxBase* GfxBase = NULL;
extern struct WBStartup* _WBenchMsg;

extern int apk_main();

int main(void) {
    int rv;

	if ((DOSBase = (struct DosLibrary*)OpenLibrary("dos.library", 33)) == NULL) {
		return RETURN_FAIL;
	}

	struct Task*thisTask = FindTask(NULL);
	ULONG currentStack=(ULONG) thisTask->tc_SPUpper-(ULONG)thisTask->tc_SPLower;

	if (currentStack < MIN_STACK_SIZE) {

		if (_WBenchMsg) {
			if ((IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 33)) != NULL) {
				EasyStruct str;
				str.es_StructSize = sizeof(EasyStruct);
				str.es_Flags = 0;
				str.es_GadgetFormat = (CONST_STRPTR)"OK";
				str.es_TextFormat = (CONST_STRPTR)"Not enough stack space!\n\n%ld bytes given.\n\nPlease increase it to at least %ld bytes\nin the Workbench Information Window.";
				str.es_Title = (CONST_STRPTR)"Cruise";

				EasyRequest(NULL, &str, NULL, currentStack, MIN_STACK_SIZE);
				CloseLibrary((struct Library*)IntuitionBase);
			}
		}
		else {
			Printf("Not enough stack space!\n\n%ld bytes given.\n\nPlease run the command \"stack %lu\" before running this program from the CLI.\n", currentStack, MIN_STACK_SIZE);
		}

		CloseLibrary((struct Library*)DOSBase);

		return RETURN_FAIL;
	}

	if ((IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 33)) == NULL) {
		CloseLibrary((struct Library*)DOSBase);
		return RETURN_FAIL;
	}

	if ((GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 33)) == NULL) {
		CloseLibrary((struct Library*)IntuitionBase);
		CloseLibrary((struct Library*)DOSBase);
		return RETURN_FAIL;
	}

	rv = apk_main();

	if (_WBenchMsg) {
		EasyStruct str;
		str.es_StructSize = sizeof(EasyStruct);
		str.es_Flags = 0;
		str.es_GadgetFormat = (CONST_STRPTR)"OK";
		str.es_TextFormat = (CONST_STRPTR)"Thanks for playing!";
		str.es_Title = (CONST_STRPTR)"Cruise";

		EasyRequest(NULL, &str, NULL);
	}
	else {
		PutStr("Thanks for playing!\n");
	}

	CloseLibrary((struct Library*)GfxBase);
	CloseLibrary((struct Library*)IntuitionBase);
	CloseLibrary((struct Library*)DOSBase);

	return rv;
}