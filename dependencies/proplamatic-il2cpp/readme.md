# Proplamatic-lL2CPP
Work-in-progress Il2cpp resolver library used for modding windows and android (coming later) IL2CPP unity game inspired by ByNameModding.

This library was  used for my own private project. But i make it public because why not? 

**Keep in mind, this library is still work-in-progress.** Unexpected error might occured.
For any help or bug report dm me on discord: @soto_sapi1

### Features
- Ease to use (hopefully)
- Runtime Dumper
- Pattern scan for obfuscated IL2CPP game

## Example usage

**Initializing the library**
```C++ 
#include <IL2CPP.hpp>

void Example()
{
	IL2CPP::INIT()
}
```

**Running runtime dumper**
```C++ 
#include <IL2CPP.hpp>

void Example()
{
	std::string dumpFolderPath = "C:\\dumps";
	IL2CPP::Dumper::StartDumper(dumpFolderPath);
}

```
**Getter and wrapper example**
```C++ 
#include <iostream>
#include <IL2CPP.hpp>

IL2CPP::Wrapper::Method<void()> Foo;
IL2CPP::Wrapper::Method<IL2CPP::Object*()> GetJoe;

void Example()
{
	// Getting assembly image
	IL2CPP::Domain* domain = IL2CPP::CurrentDomain();
	IL2CPP::Image* assemblyCShrap_image = domain->OpenAssembly("Assembly-CShrap.dll");
	
	// Getting class with its fullname
	IL2CPP::Class* joeController_class = assemblyCShrap_image->GetClass("Game.JoeController");
	
	// Getting method
	Foo = joeController_class->GetMethod("Foo");
	
	// You can get methodInfo with index relative to class method table
	IL2CPP::MethodInfo* GetJoe_info = joeController_class->GetMethod(0x7);
	GetJoe = GetJoe_info;
	
	// Calling gathered methods
	Foo();
	IL2CPP::Object* joe = GetJoe();
	
	// Getting method pointer
	void* GetJoe_ptr = GetJoe_info->GetPointer();
	std::cout << "GetJoe pointer: " << GetJoe_ptr << std::endl;
		
	// Getting object class
	IL2CPP:Class* joe_class = joe->GetClass();
	std::cout << "joe class: " << joe_class->GetFullName() << std::endl;
	
	// Getting object variable/field
	bool isJoeAlive = joe->GetFieldRef<bool>("isJoeAlive");
	std::cout << "is joe alive: " << isJoeAlive << std::endl;
	joe->GetFieldRef<bool>("isJoeAlive") = true;
	
	// You can get field with index relative to joe class field table
	joe->GetFieldRef<float>(0x23) = 999; // imagine this is joe hp var

	// Getting static field
	IL2CPP::FieldInfo* isControllerRunning_info = joeController_class->GetField("isControllerRunning");
	
	if(isControllerRunning_info->IsStatic())
	{
		bool isControllerRunning = isControllerRunning_info ->GetValue<bool>(nullptr);
		std::cout << "is controller running: " << isControllerRunning << std::endl;
	}
}
```
**Pattern scan example**
```C++
#include <IL2CPP.hpp>

// You can get pattern for both class and method from the dumper.
FieldPattern moneyManager_pattern {"Int32", nullptr, "List`1", "String"};

SignaturePattern AddMoney_pattern {"internal static", nullptr, nullptr, {"String", nullptr}};
IL2CPP::Wrapper::Method<void(IL2CPP::String* username, int amount)> AddMoney;

void Example()  
{
	// Class mapping
	IL2CPP::Domain* domain = IL2CPP::CurrentDomain();
	IL2CPP::Image* assemblyCShrap_image = domain->OpenAssembly("Assembly-CShrap.dll");
	
	std::string moneyManager_namespace = "Framework";		
	std::string moneyManager_keyname = "MoneyManager";		
	
	bool mappingSuccess = false;

	try
	{
		// Specify which image will be scanned by the mapper.
		IL2CPP::ClassMapping::AddImageToScan(assemblyCShrap_image);
		
		IL2CPP::AddQueue(moneyManager_keyname, moneyManager_namespace, &moneyManager_pattern);
		IL2CPP::ClassMapping::StartMapping();
		
		mappingSucces = true;
	}
	catch(IL2CPP::Exception::UnresolvedMappingQueue& err) 
	{
		// UnresolvedMappingQueue exception will be thrown if any queue is failed to be mapped.
		std::cout << err.what() << std::endl;
	}

	if(mappingSuccess)
	{
		IL2CPP::Class* moneyManager_class = IL2CPP::ClassMapper::GetClass(moneyManager_keyname); 
	
		AddMoney = moneyManager_class->GetMethodByPattern(AddMoney_pattern);
		AddMoney(IL2CPP::String::Create("DiggerBeater47"), 999999);
	}
}
```
### How to include
Yeah go figure it out by yourself

### TODO
* Android support
* More implementation
* Fully document the library
