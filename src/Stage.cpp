#include "header.h"
#include "templates.h"
#include "Stage.h"

Stage::Stage(const std::string& text_file): lines()
{
	this->lines.reserve(utility::NUMBER_OF_CREATURES);
	std::ifstream file_reader(text_file.c_str());
	std::string line;
	while (std::getline(file_reader, line))
		this->lines.push_back(line);
}

Stage::~Stage()
{

}

unsigned int Stage::Load(PsyhshEngine& engine)
{
	unsigned int background_colour = std::strtoul(this->lines.at(0).c_str(), 0, 16);
	for (std::size_t i = 1; i < utility::NUMBER_OF_CREATURES + 1; i++)
	{
		std::string line;
		try
		{
			line = this->lines.at(i);
		}
		catch (const std::exception& exc)
		{
			std::cerr << "Stage file was less than " << utility::NUMBER_OF_CREATURES << " lines. Leaving the remainder as they were...\n";
			break;
		}
		if (line.at(0) == '#')
			continue;
		std::vector<std::string> tokens = utility::split_string(line, ", ");
		std::string positionx_str = tokens.at(0);
		std::string positiony_str = tokens.at(1);
		std::string speed_str = tokens.at(2);
		std::string aggro_range_str = tokens.at(3);
		std::string activation_time_str = tokens.at(4);

		Creature* creature = nullptr;
		try
		{
			creature = engine.GetCreatures().at(i);
		}
		catch (const std::exception& exc)
		{
			std::cerr << "Stage file was too long. Only " << utility::NUMBER_OF_CREATURES << " creatures are supported. Loading what's available...\n";
			break;
		}
		creature->SetHealth(100);
		creature->SetPosition({ std::strtoul(positionx_str.c_str(), 0, 10), std::strtoul(positiony_str.c_str(), 0, 10)});
		creature->SetSpeed(std::strtoul(speed_str.c_str(), 0, 10));
		creature->SetAggroRange(std::strtoul(aggro_range_str.c_str(), 0, 10));
		creature->SetActivationTime(std::stoi(activation_time_str.c_str(), 0, 10));
	}
	return background_colour;
}
