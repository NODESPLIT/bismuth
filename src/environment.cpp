namespace Environment {
	#include "environment/system.cpp"
	#include "environment/utilities.cpp"
	#include "environment/math.cpp"

	void Bind(Parser* parser) {
		System::Bind(parser->environment, parser);
		Utilities::Bind(parser->environment);
		Math::Bind(parser->environment);
	}
}