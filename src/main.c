#include "plug.c"

#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const char *libplug_file = "build/libplug.so";
void *libplug = NULL;
plug_init_fun plug_init = NULL;
plug_update_fun plug_update = NULL;
plug_should_exit_fun plug_should_exit = NULL;
plug_should_reload_fun plug_should_reload = NULL;
plug_exit_fun plug_exit = NULL;
Plug plug = {0};

void *load_function(void *lib, const char *name, bool *err)
{
	void *fun = dlsym(lib, name);
	if (fun == NULL)
	{
		fprintf(stderr, "Error: Could not find %s", name);
		*err = true;
	}
	return fun;
}

bool reload_libplug(void)
{
	if (libplug != NULL)
		dlclose(libplug);
	libplug = dlopen(libplug_file, RTLD_NOW);

	bool err = false;

	if (libplug == NULL)
	{
		fprintf(stderr, "Error: Could not load %s:\n %s", libplug_file,
		        dlerror());
	}

	plug_init = load_function(libplug, "plug_init", &err);
	plug_update = load_function(libplug, "plug_update", &err);
	plug_should_exit = load_function(libplug, "plug_should_exit", &err);
	plug_should_reload = load_function(libplug, "plug_should_reload", &err);
	plug_exit = load_function(libplug, "plug_exit", &err);

	return !err;
}

int main(void)
{
	if (!reload_libplug())
		return EXIT_FAILURE;

	plug_init(&plug, false);

	while (!plug_should_exit(&plug))
	{
		if (plug_should_reload(&plug))
		{
			if (!reload_libplug())
				return EXIT_FAILURE;
			else
				printf(
				    "Sucessfully reloaded application with current state.\n");
		}

		plug_update(&plug);
	}

	plug_exit(&plug);

	return 0;
}
