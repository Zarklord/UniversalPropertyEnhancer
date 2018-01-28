#pragma once
#include <Spore\Cheats.h>
class TestingCheat : public ArgScript::ICommand {
public:
	virtual void ParseLine(const ArgScript::Line& line) override;
};

