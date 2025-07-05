#pragma once


template<typename T>
inline SDK::UObject* SpawnObjectInternal(SDK::TSubclassOf<SDK::UObject> objcls, SDK::UObject* outerobj, const char* objclsname)
{
	if (objcls == nullptr) {
		std::string wcn = std::string(WidgetClassName);
		AttemptLoadClass(wcn.substr(wcn.find_first_of('U') + 1).c_str());
		objcls = T::StaticClass();
	}

	return
}