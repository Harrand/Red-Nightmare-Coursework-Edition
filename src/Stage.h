#ifndef STAGE_H
#define STAGE_H
#include "PsyhshEngine.h"
#include <fstream>
#include <string>
#include <vector>

class Stage
{
public:
	Stage(const std::string& text_file);
	~Stage();
	unsigned int Load(PsyhshEngine& engine);
private:
	std::vector<std::string> lines;
};

#endif