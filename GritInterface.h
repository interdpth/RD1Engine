#pragma once
#include "Grit.h"
#include "cli.h"
#include "fi.h"
#include "FreeImage.h"
class GritInterface
{
public:
	bool grit_parse(GritRec *gr, const strvec &args);
	 bool grit_parse_shared(GritRec *gr, const strvec &args);
	

	int grit_parse_cprs(const char *key, const strvec &args);
	bool grit_parse_mapsel_format(const char *key, MapselFormat *pmf);
	void args_set_mode(GritShared *grs, const strvec &args, const strvec &paths);
	bool grit_parse_pal(GritRec *gr, const strvec &args);
	bool grit_parse_gfx(GritRec *gr, const strvec &args);
	bool grit_parse_map(GritRec *gr, const strvec &args);
	bool grit_parse_file(GritRec *gr, const strvec &args);
	int grit_parse_log(GritRec *gr, const strvec &args);

	bool grit_load_ext_tiles(GritRec *gr);
	bool grit_save_ext_tiles(GritRec *gr);

	void args_gather(strvec &args, int argc, char **argv);
	bool args_validate(const strvec &args, const strvec &fpaths);


	bool run_prep(GritRec *gr, const char *fpath);
	int run_individual(GritRec *gr, const strvec &args, const strvec &fpaths);
	int run_shared(GritRec *gr, const strvec &args, const strvec &fpaths);
	GritRec* run_main(int argc, char **argv);
	GritInterface(GritRec *gr);
	~GritInterface();
};

