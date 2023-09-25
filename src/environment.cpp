namespace Environment {
	#include "environment/system.cpp"
	#include "environment/utilities.cpp"
	#include "environment/math.cpp"
	#include "environment/time.cpp"

	void Bind(Parser* parser) {
		System::Bind(parser->environment, parser);
		Utilities::Bind(parser->environment, parser);
		Math::Bind(parser->environment, parser);
		Time::Bind(parser->environment, parser);
	}
}