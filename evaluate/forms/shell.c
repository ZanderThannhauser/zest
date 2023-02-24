
#include <debug.h>

#include "shell.h"

struct value* evaluate_shell_form(
	struct zebu_complex_command* complex_command)
{
	ENTER;
	
	TODO;
	
	// for each complex command:
		// for each simple command:
			// evaluate expressions, save strings
		// if redirect_in:
			// open
		// for each simple command:
			// fork
				// execvp
			// if last:
				// if redirect_out:
					// open
			// else:
				// pipe
		// wait, return exit code
	
	EXIT;
}
