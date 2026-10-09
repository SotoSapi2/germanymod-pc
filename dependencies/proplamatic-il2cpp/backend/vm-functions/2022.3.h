#if defined(IL2CPP_DEFINE_VM_FUNCTIONS)
void INIT(HMODULE gameAssemblyHandle)
{
    auto mono_class_instance_size_ptr = (uintptr_t)GetProcAddress(gameAssemblyHandle, "mono_class_instance_size");

    int32_t offsetBuffer = *reinterpret_cast<int32_t*>(mono_class_instance_size_ptr + 10);
    uintptr_t nextInstructionAddr = mono_class_instance_size_ptr + 14;
    uintptr_t absoluteTargetFunction = nextInstructionAddr + offsetBuffer;

    Class::Init = (bool(*)(Il2CppClass*))absoluteTargetFunction;
}
#endif