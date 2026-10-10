---
name: update-method-pattern
description: Autonomous workflow and tools to audit, discover, and update broken IL2CPP method patterns and pointer indices in Nazi Mod across Pixel Gun 3D updates.
---

# Update Method Pattern Skill

Use this skill when Pixel Gun 3D updates invalidate IL2CPP class patterns or method pointer indices (`Pointer<...>` in `src/runtime/game/import/PointerFunctions.cpp`).

---

## 1. Quick Start Workflow

When game updates break method patterns:

1. **Generate Dumps**:
   Run the mod using **`x64-Debug-NoFeature`** to generate CS dump files into `out/build/x64-Debug-NoFeature/dumps/` (or specify `--dump-dir`).
   *Note: This process must be done by the developer manually. The tool scans all 130+ `*.dll.cs` dump files in the directory. If you are a AI agent make sure to tell the developer about this step.*

2. **Audit Broken Pointers**:
   ```bash
   python script/method_pattern_tool.py audit
   ```
   This verifies every `Pointer<...>` in `PointerFunctions.cpp` against the dumps and flags out-of-bounds indices, mismatched counts, or unmapped classes.

3. **Resolve Obfuscated Classes**:
   If an obfuscated class name shifted:
   ```bash
   python script/method_pattern_tool.py resolve-class ContentKeyRegister
   ```

4. **Inspect Method Tables**:
   Inspect slots around the broken method index (or search by name/signature):
   ```bash
   python script/method_pattern_tool.py inspect-methods ContentKeyRegister --index 0x58 --window 6
   ```

5. **Match Signatures Automatically**:
   ```bash
   python script/method_pattern_tool.py match-signature ContentKeyRegister --return-type "HashSet" --params "String*"
   ```

6. **Generate Pointer C++ Code**:
   ```bash
   python script/method_pattern_tool.py gen-pointer ContentKeyRegister 0x58 --name GetRegisterList
   ```

7. **Sync Header & Build**:
   ```bash
   python script/PointerFunctionsGen.py
   cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && cmake --build --preset x64-Debug-NoFeature'
   ```

---

## 2. Core Concepts & Method Matching Strategies

### Strategy A: Plain Name Matching (Unobfuscated Classes)
- **Engine Classes** (`UnityEngine.*.dll.cs`, `mscorlib.dll.cs`):
  Classes like `Screen`, `Physics`, `Transform`, `Application`, `GameObject` have readable names and method names.
  Example pattern:
  ```cpp
  Pointer<RaycastHit*> Raycast(
      "Physics",
      "Raycast",
      MethodType::Method,
      0x0
  );
  ```
  `0x0` denotes the 1st overload matching name `"Raycast"`.

### Strategy B: Index-Based Matching (Obfuscated Classes)
- **Obfuscated Game Classes** (`Assembly-CSharp.dll.cs`):
  Class names (e.g. `一丝且丙三丁丕丏丒`) and method names (e.g. `丕丏与丂`) are Chinese Unicode or obfuscated identifiers.
- They are resolved by `ClassFinder.cpp` using `FieldPattern` lists.
- Methods are identified by method index (e.g. `0x58`) via `MethodType::Index`:
  ```cpp
  Pointer<IL2CPP::List<IL2CPP::String*>*> GetRegisterList(
      "ContentKeyRegister",
      "",
      MethodType::Index,
      0x58
  );
  ```
- **Method Shift Rule**:
  When game updates insert a new method into a class at slot $N$, all subsequent methods $M \ge N$ shift forward by $+K$ (where $K$ is the number of inserted methods).
  Always check neighboring methods in the dump to verify slot alignment!

### Strategy C: Signature Pattern Matching
- When index shifts are unknown, match by return type, parameter types, parameter count, and static flag.
- **Hidden `this` Parameter**:
  In non-static C++ pointer functions, the first argument in `Pointer<...>(...)` is often the instance pointer (`void* instance`, `Player_o* this_ptr`). In C# dump files, instance methods do not declare `this` in the parameter list. The tool handles this translation automatically.

---

## 3. Formatting Conventions

Nazi Mod requires chopped-down multi-line parameter formatting for all pointer declarations in `PointerFunctions.cpp`:

```cpp
Pointer<ReturnType> FunctionName(
    "ClassName",
    "MethodName",
    MethodType::Index,
    0x58
);
```

- Each argument must be on its own line indented with 4 spaces.
- Closing parenthesis and semicolon `);` on a newline.
- After editing `PointerFunctions.cpp`, always run `python script/PointerFunctionsGen.py` to regenerate `PointerFunctions.hpp`.

---

## 4. Troubleshooting Guide

- **Index out of range**:
  The class's method count changed. Run `python script/method_pattern_tool.py inspect-methods <Class>` to check new `methods.Count` and inspect the method table.
- **Class not found**:
  Verify the `FieldPattern` in `ClassFinder.cpp`. Game updates may have added or removed fields. Run `python script/method_pattern_tool.py resolve-class <Class>`.
- **Regex timeout in `PointerFunctionsGen.py`**:
  Nested C++ template return types like `Pointer<IL2CPP::Array<IL2CPP::Object*>*>` require bracket-balanced parsing. Ensure `PointerFunctionsGen.py` uses balanced regex.
