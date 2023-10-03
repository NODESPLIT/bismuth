namespace Bindings {
	#include "environment/system.cpp"
	#include "environment/utilities.cpp"
	#include "environment/math.cpp"
	#include "environment/time.cpp"

	void Bind(Runtime* runtime) {
		System::Bind(runtime);
		Utilities::Bind(runtime);
		Math::Bind(runtime);
		Time::Bind(runtime);
	}
}