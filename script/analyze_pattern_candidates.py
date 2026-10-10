import re
from pathlib import Path
from method_pattern_tool import parse_class_finder_mappings, find_class_in_dumps, POINTER_FUNCTIONS_PATH, find_dump_dir

dump_dir = find_dump_dir()

mappings = parse_class_finder_mappings()
with open(POINTER_FUNCTIONS_PATH, "r", encoding="utf-8") as f:
    cpp_code = f.read()

# Pattern for raw index pointers: Pointer<...> Name("Class", 0x..);
ptr_regex = re.compile(
    r'Pointer<((?:[^<>]|<(?:[^<>]|<[^<>]*>)*>)+)>\s+([A-Za-z0-9_]+)\s*\(\s*"([^"]+)"\s*,\s*(0x[0-9a-fA-F]+|[0-9]+)\s*\);',
    re.DOTALL
)

print(f"{'Variable':<30} | {'Class':<20} | {'Index':<6} | {'Uniqueness / Status'}")
print("-" * 90)

classes_cache = {}

results = []

for m in ptr_regex.finditer(cpp_code):
    sig = m.group(1).strip()
    var_name = m.group(2).strip()
    class_key = m.group(3).strip()
    idx_str = m.group(4).strip()
    idx = int(idx_str, 16) if idx_str.startswith("0x") else int(idx_str)

    if class_key not in classes_cache:
        classes_cache[class_key] = find_class_in_dumps(dump_dir, class_key, mappings)
    cls = classes_cache[class_key]
    if not cls:
        print(f"{var_name:<30} | {class_key:<20} | {idx_str:<6} | Class not found in dump")
        continue

    # Find the method at idx
    target_meth = next((x for x in cls.methods if x.index == idx), None)
    if not target_meth:
        print(f"{var_name:<30} | {class_key:<20} | {idx_str:<6} | Method index not in dump")
        continue

    # Check how many methods in this class match target_meth's signature pattern
    # Pattern format: modifier, typeName, methodName, parameterTypeList
    # For obfuscated methods, methodName is None/nullptr. For unobfuscated (or named), it can be methodName.
    # Let's test a signature pattern with nullptr for methodName:
    # { modifier, return_type, nullptr, params }
    same_sig_no_name = [
        x for x in cls.methods
        if x.modifier == target_meth.modifier and x.return_type == target_meth.return_type and x.params == target_meth.params
    ]

    # With methodName (if unobfuscated)
    is_obfuscated = any(ord(c) > 127 for c in target_meth.name) or not target_meth.name
    same_sig_with_name = [
        x for x in cls.methods
        if x.name == target_meth.name and x.params == target_meth.params
    ]

    results.append({
        "var_name": var_name,
        "class_key": class_key,
        "idx_str": idx_str,
        "idx": idx,
        "method": target_meth,
        "is_obfuscated": is_obfuscated,
        "match_no_name_count": len(same_sig_no_name),
        "match_with_name_count": len(same_sig_with_name)
    })

# Output categorized
print("\n=== UNIQUE BY SIGNATURE (Can be directly replaced by SignaturePattern, NO index offset needed!) ===")
for r in results:
    if r["match_no_name_count"] == 1:
        m = r["method"]
        print(f"{r['var_name']:<30} | {r['class_key']:<20} | {r['idx_str']:<6} | {m.sig_pattern}")

print("\n=== UNIQUE BY METHOD NAME (Unobfuscated Engine / Named Class) ===")
for r in results:
    if not r["is_obfuscated"] and r["match_with_name_count"] == 1:
        m = r["method"]
        print(f"{r['var_name']:<30} | {r['class_key']:<20} | {r['idx_str']:<6} | Name: \"{m.name}\"")

print("\n=== MULTIPLE MATCHES (SignaturePattern with offset OR requires index) ===")
for r in results:
    if r["match_no_name_count"] > 1 and (r["is_obfuscated"] or r["match_with_name_count"] > 1):
        m = r["method"]
        print(f"{r['var_name']:<30} | {r['class_key']:<20} | {r['idx_str']:<6} | {r['match_no_name_count']} matches | {m.sig_pattern}")
