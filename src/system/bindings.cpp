namespace Bindings {
	#include "environment/system.cpp"
	#include "environment/primatives.cpp"
	#include "environment/utilities.cpp"
	#include "environment/file.cpp"
	#include "environment/math.cpp"
	#include "environment/time.cpp"

	void Bind(Runtime* runtime) {
		System::Bind(runtime);
		Primatives::Bind(runtime);
		Utilities::Bind(runtime);
		File::Bind(runtime);
		Math::Bind(runtime);
		Time::Bind(runtime);
	}
}