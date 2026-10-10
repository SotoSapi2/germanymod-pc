# Guide: Updating and Fixing Patterns

When Pixel Gun 3D receives an update, the game code changes: memory addresses move, class structures shift, and obfuscated names get scrambled. As a result, the patterns used by Nazi Mod to locate classes and functions can break.

While some broken patterns will display an error message on the console during startup, others may silently fail or crash the game. This guide explains how pattern matching works under the hood and provides beginner-friendly, step-by-step instructions for updating broken class and method patterns.

---

## 1. Preparing for Debugging

Before doing anything, switch your CMake configuration / preset to **`x64-Debug-NoFeature`**. This builds Nazi Mod with cheat features disabled (`DISABLE_CHEAT_FEATURE=ON`), preventing execution of code paths that rely on broken pointers while keeping the console and logger active:

1. In Visual Studio, select the **`x64-Debug-NoFeature`** preset from the configuration dropdown (or configure with `cmake --preset x64-Debug-NoFeature`).
2. Build the project (**Ctrl + Shift + B** or `cmake --build --preset x64-Debug-NoFeature`).

> [!TIP]
> With cheat features disabled via `x64-Debug-NoFeature`, you can safely inject the mod, inspect the console log to see which patterns failed, and run the built-in assembly dumper.

---

## 2. How Pattern Matching Works

Pixel Gun 3D is compiled using **Unity IL2CPP**. In IL2CPP games:
- C# game scripts are translated into native C++ code and compiled into `GameAssembly.dll`.
- Every game update recompiles this DLL, which means **memory addresses (virtual addresses and offsets) change completely**.
- Many class and method names are **obfuscated** (renamed into random Chinese characters or meaningless symbols like `翠風語夢海`).

If we hardcoded memory addresses or obfuscated names, the mod would break after every minor patch. Instead, Nazi Mod uses **Pattern Matching** to find classes and functions dynamically at runtime:

### Class Matching (`FieldPattern`)
Even if an obfuscated class changes its name, the types and ordering of its member fields (for example: `int`, `string`, `bool`, ``List`1``) and its namespace usually stay identical. Nazi Mod inspects loaded classes in the game assembly and finds the class whose field types and namespace match this unique fingerprint.

### Method Matching (`Pointer<...>`)
Once a class is resolved, Nazi Mod locates methods using one of three strategies:
1. **By Name:** Searches for the method by its exact name (best for unobfuscated methods like `IsDead` or `get_position`).
2. **By Signature (`SignaturePattern`):** Matches the method's access modifier, return type, name (or wildcard `nullptr` if obfuscated), and parameter types.
3. **By Index:** Accesses the method directly by its position/slot number in the class's method table (best for engine classes like Unity's `Transform` or `Physics`).

---

## 3. Generating Assembly Dumps

Nazi Mod includes an integrated dumper that extracts class structures, fields, and method signatures directly into readable C#-like files.

1. Build and run Nazi Mod using the **x64-Debug-NoFeature** (or **x64-Debug**) configuration.
2. In-game, open the Nazi Mod menu (default key: `Insert`).
3. Navigate to the **Debug** tab (the flask icon).
4. Click the **Run Dumper** button.
5. The dump files will be generated in `out\build\<preset>\dumps` (e.g. `out\build\x64-Debug-NoFeature\dumps` or `out\build\x64-Debug\dumps` relative to the project root, or in `dumps` next to your loader executable file).
6. **Optional:** Open the `dumps` folder in Visual Studio Code for fast multi-file search (`Ctrl + Shift + F`).

---

## 4. Updating Class Patterns

When a class pattern fails, you will see an unresolved queue error in the console during startup, for example:
```
Type: PATTERN
Keyname: WeaponSkinManager
MethodCount: 118
Pattern: {"Single", "String", ...}
```

### Steps to Fix:
1. Open [`src/runtime/game/import/ClassFinder.cpp`](src/runtime/game/import/ClassFinder.cpp).
2. Locate the failed pattern in the `FieldPatterns` namespace or the `AddQueue` call in `DoAssemblyCSharpQueue()`.
3. Open your generated dump files and search for the class:
   - If the class name was previously known, search for it by name.
   - If the class is obfuscated, search for unique field types or strings associated with that class.
4. Above the class fields in the dump, the dumper automatically generates the exact `FieldPattern`:
   ```csharp
   // FieldPattern: {"Single", "String", "Comparison`1", "Dictionary`2", "Dictionary`2", ...};
   ```
5. Copy the updated pattern list into `FieldPatterns` in `ClassFinder.cpp`.
   - **Note on `nullptr`:** If a field type is obfuscated or unknown, use `nullptr` as a wildcard (e.g. `{"Int32", nullptr, "String"}`).
6. Check the `AddQueue` call for the class:
   ```cpp
   AddQueue("WeaponSkinManager", "", &FieldPatterns::WeaponSkinManager, 118);
   ```
   If a method count parameter is specified (like `118`), check the total number of methods in the dumped class. If the game added or removed methods, update this number to match the new method count (or remove the count argument if exact method count checking is unnecessary).

---

## 5. Updating Method Patterns

Methods in Nazi Mod are wrapped using the `Pointer<...>` template. These are primarily defined in [`src/runtime/game/import/PointerFunctions.cpp`](src/runtime/game/import/PointerFunctions.cpp).

### Important: The Hidden `this` Parameter
In C#, instance (non-static) methods implicitly receive `this`. In IL2CPP's compiled C++ code, this is passed as the first parameter: an `IL2CPP::Object*` pointer.

Suppose you find these methods in your dump:
```csharp
public class ZombieEntity
{
    // SignaturePattern: {"public", "bool", "IsDead", {}};
    // Index: 0x0 Offset: 0x10f2b40 VA: 0x7ffa73fb2b40
    public bool IsDead() { }

    // SignaturePattern: {"public", "bool", nullptr, {"PlayerDamageable"}};
    // Index: 0x1 Offset: 0x6d9830  VA: 0x7ffa73599830
    public bool 翠風語夢海(PlayerDamageable target) { }
}
```

In your C++ `Pointer` definition:
- For **instance methods**, the C++ signature starts with `IL2CPP::Object* instance`:
  ```cpp
  // Takes instance as the first parameter
  Pointer<bool(IL2CPP::Object* instance)> IsDead("ZombieEntity", "IsDead");

  // Takes instance then the actual target parameter
  Pointer<bool(IL2CPP::Object* instance, IL2CPP::Object* instance)> IsEnemyTo("ZombieEntity", ...);
  ```
- For **static methods** (such as `Screen.get_width()`), do **NOT** include `this`:
  ```cpp
  Pointer<int()> GetWidth("Screen", "get_width");
  ```
---

### Method Matching Strategies

#### Strategy 1: By Name Pattern
Used for methods with clean, unobfuscated names that do not change across updates.

```cpp
namespace PlayerDamageable
{
    // Finds "IsDead" by name
    Pointer<bool(IL2CPP::Object* player)> IsDead(
        "PlayerDamageable",
        "IsDead"
    );

    // Finds "IsDead" by name, then offsets 3 slots backward in the method table
    Pointer<bool(IL2CPP::Object* player, IL2CPP::Object* target)> IsEnemyTo(
        "PlayerDamageable",
        "IsDead",
        -3
    );
}
```

**How it breaks & how to fix:**
- If the method name became obfuscated, convert it to a **Signature Pattern** (Strategy 2).
- If an `indexOffset` (like `-3`) was used and the game update added or removed methods in between, the offset will point to the wrong method. Check the dump to recalculate the offset, or switch to a signature pattern.

---

#### Strategy 2: By Signature Pattern
Used when method names are obfuscated (e.g. Chinese characters or random strings), or when multiple overloaded methods share the same name.

A signature pattern defines:
```cpp
IL2CPP::SignaturePattern{ modifier, returnType, methodName, { parameterTypes... } }
```

- **`modifier`**: Access modifier string (e.g. `"public"`, `"private"`, `"internal"`, `"internal static"`). Use `nullptr` as a wildcard to match any modifier.
- **`returnType`**: Return type name (e.g. `"bool"`, `"Void"`, `"Single"`, `"Int32"`, `"String"`, `"PlayerDamageable"`). Use `nullptr` if obfuscated.
- **`methodName`**: Exact method name as a string. Use `nullptr` if the method name is obfuscated!
- **`parameterTypes`**: Vector of parameter type names matching the C# arguments:
  - If a parameter is an enum, use `"ENUM"`.
  - If a parameter type is obfuscated, use `nullptr`.
  - If the method takes no parameters, pass `{}`.
- **`indexOffset` (optional)**: An integer offset from the matched method (defaults to `0`).

##### Example from Dump to C++:
In the dump file:
```csharp
// SignaturePattern: {"public", "bool", nullptr, {"PlayerDamageable"}};
// Index: 0x1 Offset: 0x6d9830 VA: 0x7ffa73599830
public bool 翠風語夢海(PlayerDamageable target) { }
```

In [`PointerFunctions.cpp`](src/runtime/game/import/PointerFunctions.cpp):
```cpp
namespace ZombieEntity
{
    Pointer<bool(IL2CPP::Object* self, IL2CPP::Object* target)> IsEnemyTo(
        "ZombieEntity",
        {"public", "bool", nullptr, {"PlayerDamageable"}}
    );
}
```

> [!TIP]
> The dump file already writes out the exact `// SignaturePattern:` line above each method! In most cases, you can simply copy and paste that line into your C++ `Pointer` definition.

---

#### Strategy 3: By Ordering Index Pattern
Used when you want to access a method by its exact position (slot index) in the class's method list.

```cpp
namespace Physics
{
    // Accesses method at index 0x15 (21 in decimal) in the Physics class
    Pointer<bool(Ray ray, RaycastHit* info, float maxDistance)> Raycast(
        "Physics",
        0x15
    );
}
```

**When to use:**
- Primarily used for Unity engine classes (`UnityEngine.Physics`, `UnityEngine.Transform`, `UnityEngine.GameObject`, `UnityEngine.Screen`, etc.).
- Engine classes often have dozens of overloads with similar signatures (e.g. multiple `Physics.Raycast` overloads), making name lookup ambiguous.
- Engine class method tables are usually very stable across minor game updates.

**How to update:**
1. Open the dump file and locate the class (e.g., `UnityEngine.Physics`).
2. Find the specific method overload that matches your pointer's parameters and return type.
3. Check the `// Index: 0x...` comment generated by the dumper directly above that method:
   ```csharp
   // Index: 0x15 Offset: 0x19a2e0 VA: 0x7ffa7305a2e0
   public static bool Raycast(Ray ray, out RaycastHit hitInfo, float maxDistance) { }
   ```
4. Update the hexadecimal index in [`PointerFunctions.cpp`](src/runtime/game/import/PointerFunctions.cpp).

---

## 7. Updating Fields

In C#, classes contain member variables (fields) such as `isShotGun`, `_bodyAimCollider`, or `lifeTime`. In Nazi Mod, we don't always create full C++ class definitions for every game structure. Instead, we dynamically read and write fields on objects at runtime using helper functions provided by `IL2CPP::Object*`.

When a game update occurs, fields may be renamed, obfuscated, or rearranged. This section explains how fields are accessed and how to update them when they break.

---

### How Fields are Accessed in Code

There are two primary ways to access an object's fields:

#### 1. By Name (`GetFieldRef<T>("fieldName")`)
Directly looks up the field by its string name and returns a reference that can be read or modified:
```cpp
// Read or modify boolean field "isShotGun"
_this->GetFieldRef<bool>("isShotGun") = true;

// Retrieve an object reference field
IL2CPP::Object* bodyCollider = player->GetFieldRef<IL2CPP::Object*>("_bodyAimCollider");
```

You can also pass an optional `indexOffset` relative to a named field:
```cpp
// Finds "myPlayerMoveC", then offsets 10 fields forward in the field list
IL2CPP::Object* weaponSounds = _this->GetFieldPtr<IL2CPP::Object*>("myPlayerMoveC", 10);
```

#### 2. By Index (`GetFieldRef<T>(index)`)
Directly accesses the field by its position (slot index) in the class:
```cpp
// Reads color from field slot 0x27
Color crosshairColor = _this->GetFieldRef<Color>(0x27);

// Writes a string into field slot 0x8
_this->GetFieldRef<IL2CPP::String*>(0x8) = IL2CPP::String::Create("a-bomb_mini");
```

#### 3. Static Fields (`Class::GetField`)
For `static` fields that belong to the class rather than an individual object instance:
```cpp
// Accesses static field at index 0x1b and gets its instance pointer
auto expController = GetClass("ExperienceController")->GetField(0x1b)->GetValue<IL2CPP::Object*>(nullptr);
```

---

### Locating Fields in the Dump

In your generated assembly dump files, locate the class you are working on. The dumper prints all fields along with their index and byte offset:

```csharp
public class WeaponSounds
{
    // Fields
    private GameObject myPlayerMoveC; // Index: 0x5 Offset: 0x28
    public bool isShotGun;            // Index: 0x12 Offset: 0x48
    public float shotgunMinDamage;    // Index: 0x13 Offset: 0x4c
    public Color crosshairColor;      // Index: 0x27 Offset: 0x80
}
```

Notice the comment next to every field:
- **`Index: 0x...`**: The slot number of the field in the class (in hexadecimal).
- **`Offset: 0x...`**: The actual memory byte offset from the start of the object.

---

### Steps to Fix Broken Fields

#### Case 1: Name-Based Lookup Failed (`NoSuchMember`)
If the game updated and a field name was renamed or obfuscated:
1. Open the dump file and locate the class.
2. Find the field by searching for adjacent known fields or checking its type and purpose.
3. If the field received a new name, update the string in your C++ call:
   ```diff
   - _this->GetFieldRef<bool>("isShotGun") = true;
   + _this->GetFieldRef<bool>("newShotGunName") = true;
   ```
4. If the field name is now obfuscated (e.g. non-ASCII or scrambled), switch to using its index instead:
   ```cpp
   _this->GetFieldRef<bool>(0x12) = true;
   ```

#### Case 2: Index-Based Field Points to the Wrong Data
If an index-based lookup (e.g. `GetFieldRef<Color>(0x27)`) points to the wrong field or crashes:
1. When developers add or remove fields in a class, all subsequent field indices shift.
2. Find the class in the dump file and locate the target field.
3. Check the updated `// Index: 0x...` comment generated by the dumper.
4. Update the hexadecimal index in C++:
   ```diff
   - Color crosshairColor = _this->GetFieldRef<Color>(0x27);
   + Color crosshairColor = _this->GetFieldRef<Color>(0x29);
   ```

#### Case 3: Relative Offset Shifted
If a relative offset was used (e.g. `GetFieldPtr("myPlayerMoveC", 10)`):
1. In the dump file, count how many field slots are between the anchor field (`myPlayerMoveC`) and the target field.
2. If new fields were inserted in between, recalculate the index difference and update the offset number, or consider switching to direct name or index access.

---

### Type Matching Cheat Sheet

Always make sure the C++ template type `<T>` matches the field type in the dump:

| C# Dump Type               | C++ Template Type `<T>`          | Example                                                 |
| :------------------------- | :------------------------------- | :------------------------------------------------------ |
| `bool`                     | `bool`                           | `obj->GetFieldRef<bool>("isEnabled")`                   |
| `int` / `Int32`            | `int`                            | `obj->GetFieldRef<int>(0x4)`                            |
| `float` / `Single`         | `float`                          | `obj->GetFieldRef<float>("lifeTime")`                   |
| `string` / `String`        | `IL2CPP::String*`                | `obj->GetFieldRef<IL2CPP::String*>("name")`             |
| Class instance / reference | `IL2CPP::Object*`                | `obj->GetFieldRef<IL2CPP::Object*>("myPlayerMoveC")`    |
| `List<T>`                  | `IL2CPP::List<IL2CPP::Object*>*` | `obj->GetFieldRef<IL2CPP::List<IL2CPP::Object*>*>(0x0)` |
| `Vector3`                  | `Vector3`                        | `obj->GetFieldRef<Vector3>("position")`                 |
| `Color`                    | `Color`                          | `obj->GetFieldRef<Color>(0x27)`                         |
