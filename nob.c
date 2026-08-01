#define NOB_IMPLEMENTATION
#include "nob.h"
#include <string.h>

// #define RELEASE

int main(int argc, char **argv)
{
	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = {0};

	if (!nob_mkdir_if_not_exists("build"))
		return 1;

	Nob_File_Paths children = {0};
	if (!nob_read_entire_dir("src", &children))
		return 1;

#ifdef RELEASE
	nob_log(NOB_INFO, "BUILDING APPLICATION FOR RELEASE.");

	nob_cmd_append(&cmd, "clang");
	nob_cmd_append(&cmd, "-o", "build/game_release");
	nob_cmd_append(&cmd, "-DRELEASE");

	for (size_t i = 0; i < children.count; ++i)
	{
		const char *filename = children.items[i];

		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
			continue;
		if (strcmp(filename, "main.c") == 0)
			continue;

		if (nob_sv_end_with(nob_sv_from_cstr(filename), ".c"))
		{
			const char *file_path = nob_temp_sprintf("src/%s", filename);
			nob_cmd_append(&cmd, file_path);
		}
	}

	nob_cmd_append(&cmd, "./lib/rl/libraylib.a");
	nob_cmd_append(&cmd, "-std=c99", "-Wall", "-Wextra", "-Wpedantic", "-O3");
	nob_cmd_append(&cmd, "-Ilib", "-Ilib/rl/", "-Ilib/./");
	nob_cmd_append(&cmd, "-framework", "Cocoa");
	nob_cmd_append(&cmd, "-framework", "OpenGL");
	nob_cmd_append(&cmd, "-framework", "IOKit");
	nob_cmd_append(&cmd, "-framework", "CoreVideo");

	if (!nob_cmd_run_sync(cmd))
		return 1;

#else
	nob_log(NOB_INFO, "BUILDING DEVELOPMENT VERSION.");

	nob_cmd_append(&cmd, "clang");
	nob_cmd_append(&cmd, "-o", "build/libplug.so");

	for (size_t i = 0; i < children.count; ++i)
	{
		const char *filename = children.items[i];

		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
			continue;

		if (strcmp(filename, "main.c") == 0)
			continue;

		if (nob_sv_end_with(nob_sv_from_cstr(filename), ".c"))
		{
			const char *file_path = nob_temp_sprintf("src/%s", filename);
			nob_cmd_append(&cmd, file_path);
		}
	}

	nob_cmd_append(&cmd, "./lib/rl/libraylib.dylib");
	nob_cmd_append(&cmd, "-std=c99", "-Wall", "-Wextra", "-Wpedantic");
	nob_cmd_append(&cmd, "-shared", "-fPIC");
	nob_cmd_append(&cmd, "-Ilib", "-Ilib/rl/", "-Ilib/./");
	nob_cmd_append(&cmd, "-framework", "Cocoa");
	nob_cmd_append(&cmd, "-framework", "OpenGL");
	nob_cmd_append(&cmd, "-framework", "IOKit");
	nob_cmd_append(&cmd, "-framework", "CoreVideo");

	if (!nob_cmd_run_sync(cmd))
		return 1;

	cmd.count = 0;

	nob_cmd_append(&cmd, "clang");
	nob_cmd_append(&cmd, "-o", "build/game");
	nob_cmd_append(&cmd, "src/main.c");
	nob_cmd_append(&cmd, "./lib/rl/libraylib.dylib");
	nob_cmd_append(&cmd, "-std=c99", "-Wall", "-Wextra", "-Wpedantic");
	nob_cmd_append(&cmd, "-Ilib", "-Ilib/rl/", "-Ilib/./");
	nob_cmd_append(&cmd, "-framework", "Cocoa");
	nob_cmd_append(&cmd, "-framework", "OpenGL");
	nob_cmd_append(&cmd, "-framework", "IOKit");
	nob_cmd_append(&cmd, "-framework", "CoreVideo");

	if (!nob_cmd_run_sync(cmd))
		return 1;
#endif

	nob_cmd_free(cmd);
	nob_da_free(children);
	return 0;
}
